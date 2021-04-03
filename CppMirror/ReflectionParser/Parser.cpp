#include "Parser.h"
#include "CursorType.h"
#include "Util.h"
#include <algorithm>
#include <clang-c/Index.h>

using namespace Mirror;

/* Checks if one of the attributes specify reflection. */
bool isReflected(const std::vector<Annotation>& attributes) {
	for (const Annotation& annotation : attributes) {
		if (annotation.name == "Reflect")
			return true;
	}
	return false;
}

AccessSpecifier getAccessSpecifier(CX_CXXAccessSpecifier accessSpecifier) {
	if (accessSpecifier == CX_CXXAccessSpecifier::CX_CXXPrivate)
		return AccessSpecifier::PRIVATE;
	else if (accessSpecifier == CX_CXXAccessSpecifier::CX_CXXProtected)
		return AccessSpecifier::PROTECTED;
	else if (accessSpecifier == CX_CXXAccessSpecifier::CX_CXXPublic)
		return AccessSpecifier::PUBLIC;
	return AccessSpecifier::INVALID;
}

const std::vector<ClassInfo>& Parser::getClasses() const {
	return classes;
}

const std::vector<EnumInfo>& Parser::getEnumerations() const {
	return enumerations;
}

void filterOutUnusedEnums(std::vector<EnumInfo>& enums, const std::vector<ClassInfo>& classes) {
	std::vector<std::size_t> removeList;
	for (std::size_t i = 0; i < enums.size(); i++) {
		bool remove = true;
		for (const ClassInfo& currentClass : classes) {
			for (const Property& prop : currentClass.properties) {
				if (prop.type.isEnumeration && prop.type.name == enums[i].name) {
					remove = false;
					break;
				}
			}
			if (!remove) break;
		}
		if (remove) removeList.push_back(i);
	}
	std::reverse(removeList.begin(), removeList.end());
	for (std::size_t& index : removeList) {
		enums.erase(enums.begin() + index);
	}
}

/* Expects a Cursor for a class. Parses through all children and looks for signs of abstract class. */
bool isAbstractClass(const Cursor& child) {
	for (const Cursor& member : child.getChildren()) {
		auto attributes = member.getAnnotateAttributes();
		CXCursorKind memberKind = member.getKind();
		if (memberKind == CXCursor_CXXMethod) {	// Function
			if (member.isPureVirtualMethod()) {
				return true;
			}
		}
		else if (memberKind == CXCursor_Destructor) {
			if (member.isPureVirtualMethod()) {
				return true;
			}
		}
		/*else if (memberKind == CXCursor_Constructor) {
			AccessSpecifier accessSpecifier = getAccessSpecifier(member.getAccessSpecifier());
			std::size_t argumentCount = member.getArguments().size();
			if (accessSpecifier != AccessSpecifier::PUBLIC && argumentCount == 0) { // If default constuctor is not public that class is effectively abstract (but not technically)
				return true;
			}
		}*/
	}
	return false;
}

void Parser::parseTranslationUnit(const char* target, const std::vector<const char*>& cmdArguments, std::size_t& typeIDCounter) {
	// Check if target has valid extension
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit = clang_parseTranslationUnit(
		index,
		target,
		cmdArguments.data(),
		cmdArguments.size(),
		nullptr, 0,
		CXTranslationUnit_None
	);
	if (unit == nullptr)
	{
		std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
		exit(-1);
	}
	CXCursor cursor = clang_getTranslationUnitCursor(unit);

	parse(cursor, typeIDCounter);
	filterOutUnusedEnums(enumerations, classes);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
}

void Parser::parseEnum(Cursor cursor, std::string nameSpace) {
	EnumInfo enumeration;
	enumeration.name = nameSpace + cursor.getSpelling();
	enumeration.sourceFile = cursor.getSourceFile();
	for (const Cursor& member : cursor.getChildren()) {
		if (member.getKind() == CXCursor_EnumConstantDecl) {
			enumeration.constants.push_back(enumeration.name + "::" + member.getSpelling());
			enumeration.prettyConstants.push_back(member.getSpelling());
		}
	}
	enumerations.push_back(enumeration);
	propertyParser.registerEnum(enumeration.name);
}

/* Parses through all children of the given cursor and generates meta reflection headers for each class with the annotated attribute for reflection.
   For example: If Example.hpp contains a reflected class named ExampleClass an ExampleClass.generated.hpp will be created containing reflection data and
   functions for ExampleClass.
*/
void Parser::parse(Cursor cursor, std::size_t& typeIDCounter, std::string nameSpace) {
	for (const Cursor& child : cursor.getChildren()) {
		CXCursorKind kind = child.getKind();
		if (kind == CXCursor_Namespace) {
			// Parse and look for classes in the namespace
			parse(child, typeIDCounter, child.getSpelling() + "::");
		}
		else if (child.isDefinition() && kind == CXCursor_EnumDecl) {
			parseEnum(child, nameSpace);
		}
		else if (child.isDefinition() && (kind == CXCursor_ClassDecl || kind == CXCursor_StructDecl)) {
			// Get Annotate Attributes
			auto attributes = child.getAnnotateAttributes();
			if (!isReflected(attributes)) continue; // Only builds classes who specify themselves as reflected

			bool skip = false;
			// Check if this class has already been added
			std::string classSpelling = child.getSpelling();
			for (const ClassInfo& clazz : classes) {
				if (clazz.fullName == nameSpace + classSpelling)
					skip = true; // Class already defined, skip
			}
			if (skip) continue;

			std::size_t classTypeID = typeIDCounter++;
			ClassInfo& currentClass = classes.emplace_back(classTypeID);
			currentClass.name = classSpelling;
			currentClass.fullName = nameSpace + classSpelling;
			currentClass.sourceFile = child.getSourceFile();
			currentClass.annotatedAttributes = attributes;
			currentClass.isAbstract = isAbstractClass(child) || currentClass.hasAnnotation("Abstract");

			std::vector<Property>& properties = currentClass.properties;
			std::vector<Function>& functions = currentClass.functions;

			for (const Cursor& member : child.getChildren()) {
				auto attributes = member.getAnnotateAttributes();
				CXCursorKind memberKind = member.getKind();
				if (memberKind == CXCursor_CXXBaseSpecifier) {
					currentClass.baseClasses.push_back(Type{ member.getType().getSpelling() });
				}
				else if (member.isDefinition() && memberKind == CXCursor_EnumDecl) {
					parseEnum(member, currentClass.fullName + "::");
				}
				else {
					if (!isReflected(attributes)) continue; // Only builds class members who specify themselves as reflected
					if (memberKind == CXCursor_FieldDecl || memberKind == CXCursor_VarDecl) {	// Variable (Both static and nonstatic)
						Property& currentProperty = properties.emplace_back();
						currentProperty.name = member.getSpelling();
						currentProperty.isStatic = (memberKind == CXCursor_VarDecl);
						currentProperty.accessSpecifier = getAccessSpecifier(member.getAccessSpecifier());
						currentProperty.annotatedAttributes = attributes;
						currentProperty.type = propertyParser.parse(member.getType().getSpelling());
					}
					else if (memberKind == CXCursor_CXXMethod) {	// Function
						Function& currentFunction = functions.emplace_back();
						currentFunction.name = member.getSpelling();
						currentFunction.returnType = propertyParser.parse(member.getReturnType().getSpelling());
						currentFunction.isConst = member.isConstMethod();
						currentFunction.isStatic = member.isStaticMethod();
						currentFunction.accessSpecifier = getAccessSpecifier(member.getAccessSpecifier());
						currentFunction.annotatedAttributes = attributes;

						// Parameters
						std::vector<Variable>& parameters = currentFunction.parameters;
						std::vector<Cursor> arguments = member.getArguments();
						for (unsigned int i = 0; i < arguments.size(); i++) {
							Variable& currentParameter = parameters.emplace_back();
							currentParameter.name = arguments[i].getSpelling();
							currentParameter.type = propertyParser.parse(arguments[i].getType().getSpelling());
						}
					}
					else if (memberKind != CXCursor_AnnotateAttr) {
						std::cout << "\tWarning: Unknown member " << member.getDisplayName() << ", kind: " << memberKind << "\n";
					}
				}
			} // End of member
		} // End of class
	} // End of all children
}