#include "Printer.h"
#include "Util.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>

#include <map>


using namespace Mirror;

static const std::string LIBRARY_NAME_SPACE = "Mirror";
static const std::string INCLUDE_DIRECTORY_NAME = "ReflectionParser";
static const std::string endml = "\\\n"; // Represents the end of a macro line

struct ArrayType {
	ArrayType(std::string name, std::string sizeFunc) : name(name), sizeFunc(sizeFunc) {}
	std::string name; // Name of the array type
	std::string sizeFunc; // Name of the function that will return the size of the array type
};

static std::vector<ArrayType> arrayTypes = { // Suggestion. Not used.
	ArrayType("std::vector",	"size()"),
	ArrayType("std::array",		"size()")
};

bool isArrayType(const VariableType& type) {
	if (type.isArray) return true;
	for (const ArrayType& arrayType : arrayTypes) {
		if (type.name == arrayType.name) return true;
	}
	return false;
}

std::string getArrayTypeSize(const Property& prop) {
	if (prop.type.isArray) {
		return std::to_string(prop.type.arraySize);
	}
	else {
		for (const ArrayType& arrayType : arrayTypes) {
			if (prop.type.name == arrayType.name) {
				return prop.name + "." + arrayType.sizeFunc;
			}
		}
	}
	return "0";
}

std::string Printer::printTabs(std::size_t amount) const {
	std::stringstream os;
	for (std::size_t i = 0; i < amount; i++) {
		os << "\t";
	}
	return os.str();
}

/* Returns the full name of a variables type. Example: const std::string[5] */
std::string printVariableType(const VariableType& type, int printArrayAsPointer = false) {
	std::ostringstream result;

	if (type.isConst)
		result << "const ";
	result << type.name;

	if (type.isTemplate())
		result << "<";
	for (std::size_t i = 0; i < type.templateParams.size(); i++) {
		result << printVariableType(type.templateParams[i]);
		if (i != type.templateParams.size() - 1)
			result << ", ";
	}
	if (type.isTemplate())
		result << ">";

	if (type.isReference)
		result << "&";
	if (type.isPointer)
		result << "*";
	if (type.isArray && printArrayAsPointer == true)
		result << "*";
	if (!printArrayAsPointer && type.isArray) {
		result << "[";
		result << std::to_string(type.arraySize);
		result << "]";
	}

	return result.str();
}

/* Returns a string of all parameters with type + name without any new lines with comma between every parameter. */
std::string printParameters(const std::vector<Variable>& parameters) {
	std::ostringstream result;

	for (std::size_t i = 0; i < parameters.size(); i++) {
		const Variable& parameter = parameters[i];
		std::string name = "param" + std::to_string(i);
		result << printVariableType(parameter.type, true);
		result << " " << name;
		if (i != parameters.size() - 1)
			result << ", ";
	}

	return result.str();
}

std::string printAccessSpecifier(AccessSpecifier specifier) {
	if (specifier == AccessSpecifier::PRIVATE)
		return LIBRARY_NAME_SPACE + "::AccessSpecifier::PRIVATE";
	else if (specifier == AccessSpecifier::PROTECTED)
		return LIBRARY_NAME_SPACE + "::AccessSpecifier::PROTECTED";
	else if (specifier == AccessSpecifier::PUBLIC)
		return LIBRARY_NAME_SPACE + "::AccessSpecifier::PUBLIC";
	return LIBRARY_NAME_SPACE + "::AccessSpecifier::INVALID";
}

std::string printAnnotatedAttributes(std::vector<Annotation> attributes) {
	std::stringstream sstream;
	sstream << "{";
	for (std::size_t i = 0; i < attributes.size(); i++) {
		sstream << LIBRARY_NAME_SPACE << "::Annotation{\"" << attributes[i].name << "\", ";
		sstream << '{';
		for (std::size_t ii = 0; ii < attributes[i].values.size(); ii++) {
			sstream << '"' << attributes[i].values[ii] << '"';
			if (ii != attributes[i].values.size() - 1) // Delimiter
				sstream << ", ";
		}
		sstream << "}}";
		if (i != attributes.size() - 1) // Delimiter
			sstream << ", ";
	}
	sstream << "}";
	return sstream.str();
}

/*  Returns a string of the names of all parameters without any new lines with comma between every parameter. */
std::string printParameterPack(std::size_t parameterAmount) {
	std::ostringstream result;
	if (parameterAmount != 0)
		result << ", ";

	for (std::size_t i = 0; i < parameterAmount; i++) {
		result << "param" + std::to_string(i);
		if (i != parameterAmount - 1)
			result << ", ";
	}

	return result.str();
}

std::string printTemplateArguments(std::size_t amount, bool printTypename = false) {
	std::ostringstream result;
	for (std::size_t i = 0; i < amount; i++) {
		if (printTypename)
			result << "typename ";
		result << "Arg" << std::to_string(i);
		if (!printTypename)
			result << " arg" << std::to_string(i);
		if (i != amount - 1)
			result << ", ";
	}
	return result.str();
}

/* Converts bool to std::string */
std::string toString(bool boolean) {
	if (boolean)
		return "true";
	return "false";
}

/* Returns true if they are equal. */
bool equalParameters(const std::vector<Variable>& leftVec, const std::vector<Variable>& rightVec) {
	if (leftVec.size() != rightVec.size()) return false;
	for (std::size_t i = 0; i < leftVec.size(); i++) {
		if (leftVec[i].type != rightVec[i].type) return false;
	}
	return true;
}

/* Removes any copies of parameter combinations. */
std::vector<std::vector<Variable>> getUniqueParameters(std::vector<std::vector<Variable>> parameters) {
	std::vector<std::vector<Variable>> result;

	while (!parameters.empty()) {
		bool add = true;
		for (const std::vector<Variable>& other : result) {
			if (equalParameters(parameters[0], other))
				add = false;
		}
		if (add) result.push_back(parameters[0]);
		parameters.erase(parameters.begin());
	}

	return result;
}

/* Returns a vector containing the number of parameters the functions require, without any duplicates. */
std::vector<std::size_t> getUniqueParameterAmount(std::vector<std::vector<Variable>> parameters) {
	std::vector<std::size_t> result;

	while (!parameters.empty()) {
		bool add = true;
		for (const std::size_t& amount : result) {
			if (amount == parameters[0].size())
				add = false;
		}
		if (add) result.push_back(parameters[0].size());
		parameters.erase(parameters.begin());
	}

	return result;
}

std::string generateClassFileName(std::string sourceFile, std::string className) {
	//std::size_t extensionIndex = sourceFile.find_last_of(".");
	//sourceFile.insert(extensionIndex, "." + className + ".generated");
	//sourceFile.insert(extensionIndex, + ".generated");
	//return sourceFile;
	std::string filePath = sourceFile.substr(0, sourceFile.find_last_of('/'));
	return filePath + "/" + className + ".generated.h";
}










void Printer::print(const std::vector<ClassInfo>& classes) {
	for (const ClassInfo& currentClass : classes) {
		auto baseClasses = getAllBaseClasses(currentClass, classes);
		auto directBaseClasses = getDirectBaseClasses(currentClass, classes);
		printClass(currentClass, baseClasses, directBaseClasses);
	}
	printReflectionPolymorph(classes);
}

void Printer::printEnums(const std::vector<EnumInfo>& enumerations) {
	std::ostringstream ostream;
	ostream << "#include <cstddef>" << std::endl;
	ostream << "#include <string>" << std::endl;
	ostream << "#include <vector>" << std::endl;
	ostream << "#include <stdexcept>" << std::endl;
	//
	std::vector<std::string> includeFiles;
	for (const EnumInfo& currentEnum : enumerations) {
		includeFiles.push_back(currentEnum.sourceFile);
	}
	std::sort(includeFiles.begin(), includeFiles.end());
	includeFiles.erase(std::unique(includeFiles.begin(), includeFiles.end()), includeFiles.end());
	for (const std::string& includeFile : includeFiles) {
		ostream << "#include \"" << includeFile << "\"" << std::endl;
	}
	//
	ostream << std::endl;

	ostream << "namespace " << LIBRARY_NAME_SPACE << "{" << std::endl;

	ostream << "\t" << "std::vector<std::string> getEnumConstantNames(std::string enumFullName) {" << std::endl;
	for (const EnumInfo& currentEnum : enumerations) {
		ostream << "\t" << "\t" << "if (enumFullName == \"" << currentEnum.name << "\") {" << std::endl;
		ostream << "\t" << "\t" << "\t" << "return {";
		for (std::size_t i = 0; i < currentEnum.prettyConstants.size(); i++) {
			ostream << "\"" << currentEnum.prettyConstants[i] << "\"";
			if (i != currentEnum.prettyConstants.size() - 1)
				ostream << ", ";
		}
		ostream << "};" << std::endl;
		ostream << "\t" << "\t}" << std::endl;
	}
	ostream << "\t" << "\t" << "throw std::invalid_argument(\"getEnumConstantNames::ERROR The given Enum name either does not belong to any existing Enum or the Enum is not reflected.\");" << std::endl;
	ostream << "\t" << "}" << std::endl;
	// setEnumConstant
	ostream << "\t" << "template <typename T>" << std::endl;
	ostream << "\t" << "void setEnumConstant(T& value, std::size_t index) {" << std::endl;
	ostream << "\t" << "\t" << "throw std::invalid_argument(\"setEnumConstant::ERROR this enum type is not reflected.\");" << std::endl;
	ostream << "\t" << "}" << std::endl;

	for (const EnumInfo& currentEnum : enumerations) {
		ostream << "\t" << "template <>" << std::endl;
		ostream << "\t" << "void setEnumConstant<" << currentEnum.name << ">(" << currentEnum.name << "& value, std::size_t index) {" << std::endl;
		for (std::size_t i = 0; i < currentEnum.constants.size(); i++) {
			ostream << "\t" << "\t" << "if (index == " << i << ") {" << std::endl;
			ostream << "\t" << "\t" << "\t" << "value = " << currentEnum.constants[i] << ";" << std::endl;
			ostream << "\t" << "\t" << "\t" << "return;" << std::endl;
			ostream << "\t" << "\t" << "}" << std::endl;
		}
		ostream << "\t" << "\t" << "throw std::out_of_range(\"setEnumConstant::ERROR the given index is out of range!\");" << std::endl;
		ostream << "\t" << "}" << std::endl;
	}
	ostream << "}";
	printHeaderFile(target + "/ReflectionEnum.generated.h", ostream);
}


void Printer::printHeaderFile(std::string fileName, std::ostringstream& osstream) {
	if (validHeaderExtension(fileName)) {
		std::ostringstream temp;
		temp << "#ifndef REFLECTION_PARSER\n";
		temp << "#pragma once\n";
		temp << osstream.str() << std::endl;
		temp << "#endif";
		printFile(fileName, temp);

		// Clear ostringstream
		osstream.str("");
		osstream.clear();
	}
	else {
		std::cout << "Parser: Invalid Header extension\n";
	}
}

void Printer::printSourceFile(std::string fileName, std::ostringstream& osstream) {
	if (validSourceExtension(fileName)) {
		printFile(fileName, osstream);
	}
	else {
		std::cout << "Parser: Invalid Source extension\n";
	}
}

void Printer::printFile(std::string fileName, std::ostringstream& osstream) {
	if (!osstream.str().empty()) {
		// Print reflection data for last source file
		// Add includes

		// Read file
		std::stringstream fileContent;
		std::ifstream infile(fileName, std::ios::in);
		if (infile.is_open()) {
			fileContent << infile.rdbuf();
		}
		infile.close();

		// Check if file contains different content
		bool writeToFile = false;
		auto str = osstream.str();
		auto fileStr = fileContent.str();

		if (fileStr.size() != str.size()) {
			writeToFile = true;
		}
		else {
			for (std::size_t i = 0; i < fileStr.size(); i++) {
				if (fileStr[i] != str[i]) {
					writeToFile = true;
					break;
				}
			}
		}

		if (writeToFile) {
			// Write to file
			std::ofstream outfile(fileName, std::ofstream::out | std::ofstream::trunc);
			outfile << osstream.str();
			outfile.close();
			std::cout << "New file: " << fileName << " (" << osstream.str().size() << ")\n";
		}

		// Clear ostringstream
		osstream.str("");
		osstream.clear();
	}
}

bool isComponentType(const ClassInfo& currentClass, const std::vector<ClassInfo>& baseClasses) {
	for (const ClassInfo& base : baseClasses) {
		if (base.name == "Component") return true;
	}
	return currentClass.name == "Component";
}

void Printer::printClass(const ClassInfo& currentClass, const std::vector<ClassInfo>& baseClasses, const std::vector<ClassInfo>& directBaseClasses) {
	std::string sourceFile = currentClass.sourceFile;
	std::ostringstream fileInput;

	bool newProperty = false;
	bool newFunction = false;
	bool newParameter = false;

	// Includes
	fileInput << "#include <" + INCLUDE_DIRECTORY_NAME + "/ReflectionParser.h>\n";
	fileInput << "#include <cstddef>\n";
	fileInput << "#include <stdexcept>\n";
	fileInput << "#include <type_traits>\n";
	fileInput << "#include <tuple>\n";
	fileInput << "#include <vector>\n";
	fileInput << "\n";
	// Macro undefine
	fileInput << "#ifdef GENERATED_BODY\n";
	fileInput << "#undef GENERATED_BODY\n";
	fileInput << "#endif\n";
	// Macro definition
	fileInput << "#define GENERATED_BODY(...)" << endml;
	// private
	fileInput << "private:" << endml;

	// getTypeImpl
	fileInput << "static " << LIBRARY_NAME_SPACE << "::Class createType() {" << endml;
	fileInput << printTabs(1) << LIBRARY_NAME_SPACE << "::Class newClass(" + std::to_string(currentClass.typeID) + ");" << endml;
	fileInput << printTabs(1) << "newClass.name = \"" << currentClass.fullName << "\";" << endml;
	fileInput << printTabs(1) << "newClass.size = sizeof(" << currentClass.fullName << ");" << endml;
	fileInput << printTabs(1) << "newClass.isAbstract = " << toString(currentClass.isAbstract) << ";" << endml;
	//fileInput << printTabs(1) << "newClass.size = sizeof(" + currentClass.fullName + ");" << endml;
	fileInput << printTabs(1) << "newClass.annotatedAttributes = " << printAnnotatedAttributes(currentClass.annotatedAttributes) << ";" << endml;

	// Base classes
	for (const Type& baseClass : currentClass.baseClasses) {
		fileInput << printTabs(1) << "newClass.baseClasses.push_back(" + LIBRARY_NAME_SPACE + "::Type{ \"" + baseClass.name + "\", sizeof(" << baseClass.name << ") });" << endml;
	}
	// Properties
	for (const Property& currentProperty : getProperties(currentClass, baseClasses)) {
		printProperty(currentProperty, fileInput, newProperty);
	}
	// Functions
	for (const Function& currentFunction : getFunctions(currentClass, baseClasses)) {
		printFunction(currentFunction, fileInput, newFunction, newParameter);
	}
	fileInput << printTabs(1) << "return newClass;" << endml;
	fileInput << "}" << endml;

	// protected
	fileInput << "protected:" << endml;
	// getValue
	printGetValue(currentClass, directBaseClasses, fileInput);
	// setValue
	printSetValue(currentClass, directBaseClasses, fileInput);
	// onUpdate
	printOnUpdate(currentClass, directBaseClasses, fileInput);
	// invoke
	printInvoke(currentClass, directBaseClasses, fileInput);
	// friends
	fileInput << "friend " + LIBRARY_NAME_SPACE + "::Function;" << endml;
	fileInput << "friend " + LIBRARY_NAME_SPACE + "::Property;" << endml;
	// static getTypeImpl
	fileInput << "static " + LIBRARY_NAME_SPACE + "::Class getTypeImpl() {" << endml;
	fileInput << "\tstatic " + LIBRARY_NAME_SPACE + "::Class type = createType();" << endml;
	fileInput << "\treturn type;" << endml;
	fileInput << "}" << endml;
	// static hasProperty
	fileInput << "static bool hasProperty(std::string propertyName) {" << endml;
	fileInput << "\t" + LIBRARY_NAME_SPACE + "::Class type = getTypeImpl();" << endml;
	fileInput << "\tfor (const " + LIBRARY_NAME_SPACE + "::Property& prop : type.properties)" << endml;
	fileInput << "\t\tif (prop.name == propertyName) return true;" << endml;
	fileInput << "\treturn false;" << endml;
	fileInput << "}" << endml;

	// public
	fileInput << "public:" << endml;
	// static getClassTypeID
	fileInput << "static constexpr std::size_t getClassTypeID() {" << endml;
	fileInput << "\t" << "return " << currentClass.typeID << ";" << endml;
	fileInput << "}" << endml;
	// static getBaseTypeIDs
	fileInput << "static std::vector<std::size_t> getBaseTypeIDs() {" << endml;
	fileInput << "\t" << "return std::vector<std::size_t>{";
	for (std::size_t i = 0; i < baseClasses.size(); i++) {
		fileInput << baseClasses[i].name << "::getClassTypeID()";
		if (i != baseClasses.size()-1)
			fileInput << ",";
	}
	fileInput << "};" << endml;
	fileInput << "}" << endml;
	// static getClassType
	fileInput << "static " + LIBRARY_NAME_SPACE + "::Class getClassType() {" << endml;
	fileInput << "\tstatic " + LIBRARY_NAME_SPACE + "::Class type = getTypeImpl();" << endml;
	fileInput << "\treturn type;" << endml;
	fileInput << "}" << endml;
	fileInput << "public:" << endml;
	// virtual getType
	if (isComponentType(currentClass, baseClasses)) {
		fileInput << "virtual " + LIBRARY_NAME_SPACE + "::Class getType() {" << endml;
		fileInput << "\treturn getClassType();" << endml;
		fileInput << "}" << endml;
	}
	// virtual getTypeID
	if (isComponentType(currentClass, baseClasses)) {
		fileInput << "virtual std::size_t getTypeID() {" << endml;
		fileInput << printTabs(1) << "return " << currentClass.typeID << ";" << endml;
		fileInput << "}" << endml;
	}
	// getArraySize
	printGetArraySize(currentClass, directBaseClasses, fileInput);
	// getPointer
	printGetPointer(currentClass, directBaseClasses, fileInput);
	// serialize
	fileInput << "void serialize(std::ostream& os) const {" << endml;
	//
	fileInput << printTabs(1) << "std::vector<" << LIBRARY_NAME_SPACE << "::Property> properties;" << endml;
	fileInput << printTabs(1) << LIBRARY_NAME_SPACE << "::Property newProperty;" << endml;
	for (const Property& prop : currentClass.properties) {
		if (prop.type.isPointer || prop.type.isReference || prop.type.isConst) continue;
		fileInput << printTabs(1) << "newProperty = {};" << endml;
		printPropertyDetails(prop, "newProperty", fileInput);
		fileInput << printTabs(1) << "properties.push_back(newProperty);" << endml;
	}
	//
	fileInput << printTabs(1) + LIBRARY_NAME_SPACE + "::serializeProperties(os, properties";
	for (const Property& prop : currentClass.properties) {
		if (prop.type.isPointer || prop.type.isReference || prop.type.isConst) continue;
		fileInput << ", " << prop.name;
	}
	fileInput << ");" << endml;
	// Call serialize in all direct base classes (chaining)
	for (const ClassInfo& baseClass : directBaseClasses) {
		fileInput << printTabs(1) << baseClass.fullName << "::serialize(os);" << endml;
	}
	fileInput << "}" << endml;
	// deserialize
	fileInput << "void deserialize(std::istream& is) {" << endml;
	//
	fileInput << printTabs(1) << "std::vector<" << LIBRARY_NAME_SPACE << "::Property> properties;" << endml;
	fileInput << printTabs(1) << LIBRARY_NAME_SPACE << "::Property newProperty;" << endml;
	for (const Property& prop : currentClass.properties) {
		if (prop.type.isPointer || prop.type.isReference || prop.type.isConst) continue;
		fileInput << printTabs(1) << "newProperty = {};" << endml;
		printPropertyDetails(prop, "newProperty", fileInput);
		fileInput << printTabs(1) << "properties.push_back(newProperty);" << endml;
	}
	//
	fileInput << printTabs(1) + LIBRARY_NAME_SPACE + "::deserializeProperties(is, properties";
	for (const Property& prop : currentClass.properties) {
		if (prop.type.isPointer || prop.type.isReference || prop.type.isConst) continue;
		fileInput << ", " << prop.name;
	}
	fileInput << "); " << endml;
	// Call deserialize in all direct base classes (chaining)
	for (const ClassInfo& baseClass : directBaseClasses) {
		fileInput << printTabs(1) << baseClass.fullName << "::deserialize(is);" << endml;
	}
	for (const Property& prop : currentClass.properties) {
		if (!prop.type.isPointer && !prop.type.isReference && !prop.type.isConst) {
			for (const std::string& value : prop.getAnnotationValue("Update")) {
				fileInput << printTabs(1) << value << ";" << endml;
			}
		}
	}
	fileInput << "}";
	

	// Print new class
	printHeaderFile(generateClassFileName(sourceFile, currentClass.name), fileInput);
}

void Printer::printReflectionPolymorph(const std::vector<ClassInfo>& classes) {
	std::ostringstream fileInput;

	std::vector<std::string> includeFiles;
	for (const ClassInfo& clazz : classes) {
		const std::string& includeFile = clazz.sourceFile;
		if (std::find(includeFiles.begin(), includeFiles.end(), includeFile) == includeFiles.end())
			includeFiles.push_back(includeFile);
	}

	fileInput << "#include <" + INCLUDE_DIRECTORY_NAME + "/ReflectionTypes.h>" << std::endl;
	fileInput << "#include <" + INCLUDE_DIRECTORY_NAME + "/TypeList.h>" << std::endl;
	fileInput << "#include <stdexcept>" << std::endl;
	fileInput << std::endl;
	for (const std::string& includeFile : includeFiles) {
		fileInput << "#include \"" + includeFile + "\"" << std::endl;
	}
	fileInput << std::endl;
	// namespace
	fileInput << "namespace " + LIBRARY_NAME_SPACE + " {" << std::endl;
	// createComponent
	fileInput << "template<typename ClassType>" << std::endl;
	fileInput << "inline ClassType* createInstance(std::size_t typeID, void* dest) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << printTabs(1);
		if (i != 0)
			fileInput << "else ";
		if (classes[i].isAbstract) {
			fileInput << "if (typeID == " << classes[i].typeID << ") {" << std::endl;
			fileInput << printTabs(1) << "throw std::invalid_argument(\"createInstance::ERROR Unable to create instance of abstract class\");" << std::endl;
			fileInput << printTabs(1) << "}" << std::endl;
		}
		else {
			fileInput << "if (typeID == " << classes[i].typeID << ") {" << std::endl;
			fileInput << printTabs(2) << "return new(dest) " << classes[i].fullName << "();" << std::endl;
			fileInput << printTabs(1) << "}" << std::endl;
		}
	}
	fileInput << printTabs(1) << "throw std::invalid_argument(\"createInstance::ERROR Unable to create instance of unknown class\");" << std::endl;
	fileInput << "}" << std::endl;
	// getName
	fileInput << "inline std::string getName(std::size_t typeID) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << printTabs(1);
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " << classes[i].typeID << ") {" << std::endl;
		fileInput << printTabs(2) << "return \"" << classes[i].fullName << "\";" << std::endl;
		fileInput << printTabs(1) << "}" << std::endl;
	}
	fileInput << printTabs(1) << "throw std::invalid_argument(\"getName::ERROR Unknown class\");" << std::endl;
	fileInput << "}" << std::endl;
	// getSize
	fileInput << "inline std::size_t getSize(std::size_t typeID) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << printTabs(1);
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " << classes[i].typeID << ") {" << std::endl;
		fileInput << printTabs(2) << "return sizeof(" << classes[i].fullName << ");" << std::endl;
		fileInput << printTabs(1) << "}" << std::endl;
	}
	fileInput << printTabs(1) << "throw std::invalid_argument(\"getName::ERROR Unknown class\");" << std::endl;
	fileInput << "}" << std::endl;
	// polyGetDerivedTypeIDs
	fileInput << "inline std::vector<std::size_t> polyGetDerivedTypeIDs(std::size_t typeID) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " << classes[i].typeID << ") {" << std::endl;
		fileInput << "\t\t" << "return std::vector<std::size_t>{";
		std::vector<ClassInfo> derivedClasses = getDerivedClasses(classes[i], classes);
		for (std::size_t i2 = 0; i2 < derivedClasses.size(); i2++) {
			fileInput << derivedClasses[i2].fullName << "::getClassTypeID()";
			if (i2 != derivedClasses.size() - 1)
				fileInput << ", ";
		}
		fileInput << "};" << std::endl;
		fileInput << "\t" << "}" << std::endl;
	}
	fileInput << "\t" << "return std::vector<std::size_t>{};" << std::endl;
	fileInput << "}" << std::endl;
	// polyInvoke
	fileInput << "/* Converts the given pointer to the derived type. Calls invoke on the function instance with the casted pointer. */" << std::endl;
	fileInput << "template<typename ClassType, typename... Args>" << std::endl;
	fileInput << "void polyInvoke(" + LIBRARY_NAME_SPACE + "::Function fun, ClassType* instance, std::size_t typeID, Args... args) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return fun.invoke(reinterpret_cast<" + classes[i].fullName + "*>(instance), args...);" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"polyInvoke::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// polyGetValue
	fileInput << "template<typename T, typename ClassType>" << std::endl;
	fileInput << "T polyGetValue(" + LIBRARY_NAME_SPACE + "::Property prop, ClassType* instance, std::size_t typeID) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return prop.getValue<T>(reinterpret_cast<" + classes[i].fullName + "*>(instance));" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"polyGetValue::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// polyGetArrayValue
	fileInput << "template<typename T, typename ClassType>" << std::endl;
	fileInput << "std::vector<T> polyGetArrayValue(" + LIBRARY_NAME_SPACE + "::Property prop, ClassType* instance, std::size_t typeID) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return prop.getArrayValue<T>(reinterpret_cast<" + classes[i].fullName + "*>(instance));" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"polyGetArrayValue::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// polyGetArrayElementValue
	fileInput << "template<typename T, typename ClassType>" << std::endl;
	fileInput << "T polyGetArrayElementValue(" + LIBRARY_NAME_SPACE + "::Property prop, std::size_t index, ClassType* instance, std::size_t typeID) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return prop.getArrayElementValue<T>(reinterpret_cast<" + classes[i].fullName + "*>(instance), index);" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"polyGetArrayElementValue::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// polySetValue
	fileInput << "template<typename T, typename ClassType>" << std::endl;
	fileInput << "void polySetValue(" + LIBRARY_NAME_SPACE + "::Property prop, ClassType* instance, std::size_t typeID, T value) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return prop.setValue(reinterpret_cast<" + classes[i].fullName + "*>(instance), value);" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"polySetValue::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// polySetArrayValue
	fileInput << "template<typename T, std::size_t N, typename ClassType>" << std::endl;
	fileInput << "void polySetArrayValue(" + LIBRARY_NAME_SPACE + "::Property prop, ClassType* instance, std::size_t typeID, T(&value)[N]) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return prop.setArrayValue<T, N>(reinterpret_cast<" + classes[i].fullName + "*>(instance), value);" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"polySetArrayValue::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// polySetArrayElementValue
	fileInput << "template<typename T, typename ClassType>" << std::endl;
	fileInput << "void polySetArrayElementValue(" + LIBRARY_NAME_SPACE + "::Property prop, std::size_t index, ClassType* instance, std::size_t typeID, T value) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return prop.setArrayElementValue(reinterpret_cast<" + classes[i].fullName + "*>(instance), index, value);" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"polySetValue::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// getArraySize
	fileInput << "inline std::size_t getArraySize(" + LIBRARY_NAME_SPACE + "::Property prop, void* instance, std::size_t typeID) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return prop.getArraySize(reinterpret_cast<" + classes[i].fullName + "*>(instance));" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"getArraySize::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// getPointer
	fileInput << "inline void* getPointer(" + LIBRARY_NAME_SPACE + "::Property prop, void* instance, std::size_t typeID) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return prop.getPointer(reinterpret_cast<" + classes[i].fullName + "*>(instance));" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"getPointer::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// getArrayElementPointers
	fileInput << "inline std::vector<void*> getArrayElementPointers(" + LIBRARY_NAME_SPACE + "::Property prop, void* instance, std::size_t typeID) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return prop.getArrayElementPointers(reinterpret_cast<" + classes[i].fullName + "*>(instance));" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"getArrayElementPointers::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// memcpyImpl
	fileInput << "namespace {" << std::endl;
	fileInput << "\t" << "template<typename T>" << std::endl;
	fileInput << "\t" << "std::enable_if_t<std::is_copy_constructible<T>::value, T*> memcpyImpl(void* placement, T* instance) {" << std::endl;
	fileInput << "\t" << "\t" << "return new (placement) T(*instance);" << std::endl;
	fileInput << "\t" << "}" << std::endl;
	fileInput << "\t" << "template<typename T>" << std::endl;
	fileInput << "\t" << "std::enable_if_t<!std::is_copy_constructible<T>::value, T*> memcpyImpl(void* placement, T* instance) {" << std::endl;
	fileInput << "\t" << "\t" << "throw std::invalid_argument(\"memcpy::ERROR typeID pointing to non copy constructible class\");" << std::endl;
	fileInput << "\t" << "}" << std::endl;
	fileInput << "}" << std::endl;
	// memcpy
	fileInput << "/* Places a new copy of the given instance to the given location in memory. Makes use of the copy constructor of the type assigned to the given typeID. */" << std::endl;
	fileInput << "inline void* memcpy(void* placement, void* instance, std::size_t typeID) {" << std::endl;
	fileInput << "\t" << "switch(typeID) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t" << "\t" << "case " << std::to_string(classes[i].typeID) <<  ":" << std::endl;
		fileInput << "\t" << "\t" << "\t" << "return memcpyImpl(placement, static_cast<" << classes[i].fullName << "*>(instance));" << std::endl;
	}
	fileInput << "\t" << "\t" << "default:" << std::endl;
	fileInput << "\t" << "\t" << "\t" << "throw std::invalid_argument(\"memcpy::ERROR Invalid typeID\");" << std::endl;
	fileInput << "\t" << "}" << std::endl;
	fileInput << "}" << std::endl;
	// serialize
	fileInput << "inline void serialize(void* instance, std::size_t typeID, std::ostream& os) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return reinterpret_cast<" + classes[i].fullName + "*>(instance)->serialize(os);" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"serialize::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// deserialize
	fileInput << "inline void deserialize(void* instance, std::size_t typeID, std::istream& is) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t";
		if (i != 0)
			fileInput << "else ";
		fileInput << "if (typeID == " + std::to_string(classes[i].typeID) + ")" << std::endl;
		fileInput << "\t" << "\t" << "return reinterpret_cast<" + classes[i].fullName + "*>(instance)->deserialize(is);" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"deserialize::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// onUpdate
	fileInput << "inline void onUpdate(void* instance, std::size_t typeID, Mirror::Property prop) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t" << "if (typeID == " << classes[i].typeID << ")" << std::endl;
		fileInput << "\t" << "\t" << "return prop.onUpdate(reinterpret_cast<" + classes[i].fullName + "*>(instance));" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"onUpdate::ERROR\");" << std::endl;
	fileInput << "}" << std::endl;
	// isReflected
	fileInput << "inline bool isReflected(std::string className) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t" << "if (className == \"" << classes[i].fullName << "\")" << std::endl;
		fileInput << "\t\t" << "return true;" << std::endl;
	}
	fileInput << "\t" << "return false;" << std::endl;
	fileInput << "}" << std::endl;
	// getTypeID
	fileInput << "inline std::size_t getTypeID(std::string className) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t" << "if (className == \"" << classes[i].fullName << "\")" << std::endl;
		fileInput << "\t\t" << "return " << classes[i].typeID << ";" << std::endl;
	}
	fileInput << "\t" << "return 0;" << std::endl;
	fileInput << "}" << std::endl;
	// getType (typeID)
	fileInput << "inline " << LIBRARY_NAME_SPACE << "::Class getType(std::size_t typeID) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t" << "if (typeID == " << classes[i].typeID << ")" << std::endl;
		fileInput << "\t\t" << "return " << classes[i].fullName << "::getClassType();" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"" << LIBRARY_NAME_SPACE << "::getType::ERROR There is no such reflected class!\");" << std::endl;
	fileInput << "}" << std::endl;
	// getType (className)
	fileInput << "inline " << LIBRARY_NAME_SPACE << "::Class getType(std::string className) {" << std::endl;
	for (std::size_t i = 0; i < classes.size(); i++) {
		fileInput << "\t" << "if (className == \"" << classes[i].fullName << "\")" << std::endl;
		fileInput << "\t\t" << "return " << classes[i].fullName << "::getClassType();" << std::endl;
	}
	fileInput << "\t" << "throw std::invalid_argument(\"" << LIBRARY_NAME_SPACE << "::getType::ERROR There is no such reflected class!\");" << std::endl;
	fileInput << "}" << std::endl;
	// isArrayType
	fileInput << "inline bool isArrayType(const " << LIBRARY_NAME_SPACE << "::VariableType& type) {" << std::endl;
	fileInput << "\t" << "if (type.isArray) return true;" << std::endl;
	for (std::size_t i = 0; i < arrayTypes.size(); i++) {
		fileInput << "\t" << "if (type.name == \"" << arrayTypes[i].name << "\")" << std::endl;
		fileInput << "\t\t" << "return true;" << std::endl;
	}
	fileInput << "\t" << "return false;" << std::endl;
	fileInput << "}" << std::endl;
	// getAllReflectedTypes
	fileInput << "inline std::vector<ReflectedType> getAllReflectedTypes() {" << std::endl;
	fileInput << printTabs(1) << "return std::vector<ReflectedType>{";
	for (std::size_t i = 0; i < classes.size(); i++) {
		if (i != 0)
			fileInput << ",";
		fileInput << "{" << classes[i].typeID << ", \"" << classes[i].fullName << "\"}";
	}
	fileInput << "}; " << std::endl;
	fileInput << "}" << std::endl;
	// TypeList
	fileInput << "typedef TypeList<";
	for (std::size_t i = 0; i < classes.size(); i++) {
		if (i != 0)
			fileInput << ",";
		fileInput << classes[i].fullName;
	}
	fileInput << "> ReflectedTypes;" << std::endl;
	// End of namespace
	fileInput << "}" << std::endl;

	printHeaderFile(target + "/ReflectionPolymorph.generated.h", fileInput);
}

std::string getFullTypename(const VariableType& type) {
	if (!type.isTemplate())
	return type.name;
	std::string value = type.name + "<";
	std::size_t i = 0;
	for (const VariableType& templateType : type.templateParams) {
		value += getFullTypename(templateType);
		if (i++ != type.templateParams.size() - 1) {
			value += ", ";
		}
	}
	return value + ">";
}

void Printer::printPropertyType(const VariableType& type, std::ostream& os, std::string varName) {
	std::string fullTypeName = getFullTypename(type);
	std::string sizeString = fullTypeName.compare("void") == 0 ? "0" : "sizeof(" + fullTypeName + ")";
	os << printTabs(1) << varName << ".name = \"" + type.name + "\";" << endml;
	os << printTabs(1) << varName << ".size = " << sizeString << ";" << endml;
	os << printTabs(1) << varName << ".isConst = " + toString(type.isConst) + ";" << endml;
	os << printTabs(1) << varName << ".isPointer = " + toString(type.isPointer) + ";" << endml;
	os << printTabs(1) << varName << ".isReference = " + toString(type.isReference) + ";" << endml;
	os << printTabs(1) << varName << ".isEnumeration = " + toString(type.isEnumeration) + ";" << endml;
	os << printTabs(1) << varName << ".isArray = " + toString(type.isArray) + ";" << endml;
	os << printTabs(1) << varName << ".arraySize = " + std::to_string(type.arraySize) + ";" << endml;
	std::size_t i = 0;
	for (VariableType tParam : type.templateParams) {
		std::string variableName = "type";
		os << printTabs(1) << "{" << endml;
		os << printTabs(1) << LIBRARY_NAME_SPACE + "::VariableType " << variableName  << ";" << endml;
		printPropertyType(tParam, os, variableName);
		os << printTabs(1) << varName << ".templateParams.push_back(" << variableName << ");" << endml;
		os << printTabs(1) << "}" << endml;
	}
}

void Printer::printPropertyDetails(const Property& prop, std::string variableName, std::ostream& os) {
	os << printTabs(1) << variableName << ".name = \"" << prop.name << "\";" << endml;
	printPropertyType(prop.type, os, "newProperty.type");
	os << printTabs(1) << variableName << ".isStatic = " << toString(prop.isStatic) + ";" << endml;
	os << printTabs(1) << variableName << ".accessSpecifier = " << printAccessSpecifier(prop.accessSpecifier) << ";" << endml;
	os << printTabs(1) << variableName << ".annotatedAttributes = " << printAnnotatedAttributes(prop.annotatedAttributes) << ";" << endml;
}

void Printer::printProperty(const Property& currentProperty, std::ostream& os, bool& newProperty) {
	os << "" << endml;
	if (!newProperty) {
		os << "\t" + LIBRARY_NAME_SPACE + "::Property newProperty;" << endml;
		newProperty = true;
	}
	else {
		os << "\tnewProperty = {};" << endml;
	}
	printPropertyDetails(currentProperty, "newProperty", os);
	/*os << "\tnewProperty.name = \"" + currentProperty.name + "\";" << endml;
	os << "{" << endml;
	os << "\t" << LIBRARY_NAME_SPACE + "::VariableType& newPropertyType = newProperty.type;" << endml;
	printPropertyType(currentProperty.type, os, "newPropertyType");
	os << "}" << endml;
	os << "\tnewProperty.isStatic = " + toString(currentProperty.isStatic) + ";" << endml;
	os << "\tnewProperty.accessSpecifier = " + printAccessSpecifier(currentProperty.accessSpecifier) + ";" << endml;
	os << "\tnewProperty.annotatedAttributes = " + printAnnotatedAttributes(currentProperty.annotatedAttributes) + ";" << endml;*/
	os << "\tnewClass.properties.push_back(newProperty);" << endml;
}

void Printer::printFunction(const Function& currentFunction, std::ostream& os, bool& newFunction, bool& newParameter) {
	os << "" << endml;
	if (!newFunction) {
		os << "\t" + LIBRARY_NAME_SPACE + "::Function newFunction;" << endml;
		newFunction = true;
	}
	else {
		os << "\tnewFunction = {};" << endml;
	}
	os << "\tnewFunction.name = \"" + currentFunction.name + "\";" << endml;
	printPropertyType(currentFunction.returnType, os, "newFunction.returnType");
	os << "\tnewFunction.isConst = " + toString(currentFunction.isConst) + ";" << endml;
	os << "\tnewFunction.isStatic = " + toString(currentFunction.isStatic) + ";" << endml;
	os << "\tnewFunction.accessSpecifier = " + printAccessSpecifier(currentFunction.accessSpecifier) + ";" << endml;
	os << "\tnewFunction.annotatedAttributes = " + printAnnotatedAttributes(currentFunction.annotatedAttributes) + ";" << endml;
	for (const Variable& currentParameter : currentFunction.parameters) {
		os << "" << endml;
		if (!newParameter) {
			os << "\t" + LIBRARY_NAME_SPACE + "::Variable newParameter;" << endml;
			newParameter = true;
		}
		else {
			os << "\tnewParameter = {};" << endml;
		}
		os << "\tnewParameter.name = \"" + currentParameter.name + "\";" << endml;
		printPropertyType(currentParameter.type, os, "newParameter.type");
		os << "\tnewFunction.parameters.push_back(newParameter);" << endml;
	}
	os << "\tnewClass.functions.push_back(newFunction);" << endml;
}


void Printer::printGetArraySize(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os) {
	// List of all properties from current
	const std::vector<Property>& properties = currentClass.properties;

	os << "std::size_t getArraySize(std::string propertyName) {" << endml;
	for (const Property& prop : properties) {
		if (prop.type.isConst) continue;
		os << "\tif (propertyName == \"" + prop.name + "\") {" << endml;
		if (isArrayType(prop.type)) {
			os << "\t\t" << "return " << getArrayTypeSize(prop) << ";" << endml;
		}
		else {
			os << "\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" is not an array!\");" << endml;
		}
		os << "\t}" << endml;
	}
	// Check in base class
	for (const ClassInfo& base : directBaseClasses) {
		os << "\tif (" + base.fullName + "::hasProperty(propertyName))" << endml;
		os << "\t\treturn " + base.fullName + "::getArraySize(propertyName);" << endml;
	}
	os << "\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" does not exist or the property is const!\" << \"\\n\";" << endml;
	os << "\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" does not exist or the property is const!\");" << endml;
	os << "}" << endml;
}

void Printer::printGetPointer(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os) {
	// List of all properties from current
	const std::vector<Property>& properties = currentClass.properties;

	os << "void* getPointer(std::string propertyName) {" << endml;
	for (const Property& prop : properties) {
		if (prop.type.isConst) continue;
		os << "\tif (propertyName == \"" + prop.name + "\") {" << endml;
		os << "\t\treturn &" << prop.name << ";" << endml;
		os << "\t}" << endml;
	}
	// Check in base class
	for (const ClassInfo& base : directBaseClasses) {
		os << "\tif (" + base.fullName + "::hasProperty(propertyName))" << endml;
		os << "\t\treturn " + base.fullName + "::getPointer(propertyName);" << endml;
	}
	os << "\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" does not exist or the property is const!\" << \"\\n\";" << endml;
	os << "\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" does not exist or the property is const!\");" << endml;
	os << "}" << endml;

	// Array
	os << "std::vector<void*> getArrayElementPointers(std::string propertyName) {" << endml;
	for (const Property& prop : properties) {
		if (prop.type.isConst) continue;
		os << "\tif (propertyName == \"" + prop.name + "\") {" << endml;
		if (isArrayType(prop.type)) {
			os << "\t\t" << "std::vector<void*> vec(" << getArrayTypeSize(prop) << ");" << endml;
			os << "\t\t" << "for (std::size_t i = 0; i < vec.size(); i++) {" << endml;
			os << "\t\t\t" << "vec[i] = &" << prop.name << "[i];" << endml;
			os << "\t\t" << "}" << endml;
			os << "\t\t" << "return vec;" << endml;
		}
		else {
			os << "\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" is not an array!\");" << endml;
		}
		os << "\t}" << endml;
	}
	// Check in base class
	for (const ClassInfo& base : directBaseClasses) {
		os << "\tif (" + base.fullName + "::hasProperty(propertyName))" << endml;
		os << "\t\treturn " + base.fullName + "::getArrayElementPointers(propertyName);" << endml;
	}
	os << "\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" does not exist or the property is const!\" << \"\\n\";" << endml;
	os << "\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" does not exist or the property is const!\");" << endml;
	os << "}" << endml;
}

/* Writes all getValue functions for the class to the end of the ostream. */
void Printer::printGetValue(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os) {
	// List of all properties from current
	const std::vector<Property>& properties = currentClass.properties;

	// ----------------------------------------- getValue_impl -----------------------------------------
	os << "template<typename T>" << endml;
	os << "T getValue_impl(std::string propertyName) {" << endml;
	os << "\ttry {" << endml;
	for (const Property& prop : properties) {
		os << "\t\tif (propertyName == \"" + prop.name + "\") {" << endml;
		if (!isArrayType(prop.type)) {
			os << "\t\t\treturn " + LIBRARY_NAME_SPACE + "::convertType<T>(" << prop.name << ");" << endml;
		}
		else {
			os << "\t\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" is an array!\");" << endml;
		}
		os << "\t\t}" << endml;
	}
	// Check in base class
	for (const ClassInfo& base : directBaseClasses) {
		os << "\t\tif (" + base.fullName + "::hasProperty(propertyName))" << endml;
		os << "\t\t\treturn " + base.fullName + "::getValue_impl<T>(propertyName);" << endml;
	}
	os << "\t} catch(std::exception&) {" << endml;
	os << "\t\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" cannot be converted to the specified type!\" << \"\\n\";" << endml;
	os << "\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" cannot be converted to the specified type!\");" << endml;
	os << "\t}" << endml;
	os << "\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\" << \"\\n\";" << endml;
	os << "\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\");" << endml;
	os << "}" << endml;
	// ----------------------------------------- getArrayValue_impl -----------------------------------------
	os << "template<typename T>" << endml;
	os << "std::vector<T> getArrayValue_impl(std::string propertyName) {" << endml;
	os << "\ttry {" << endml;
	for (const Property& prop : properties) {
		os << "\t\tif (propertyName == \"" + prop.name + "\") {" << endml;
		if (isArrayType(prop.type)) {
			os << "\t\t\t" << "std::vector<T> vec(" << getArrayTypeSize(prop) << ");" << endml;
			os << "\t\t\t" << "for (std::size_t i = 0; i < vec.size(); i++) {" << endml;
			os << "\t\t\t\t" << "vec[i] = " << LIBRARY_NAME_SPACE << "::convertType<T>(" << prop.name << "[i]);" << endml;
			os << "\t\t\t" << "}" << endml;
			os << "\t\t\t" << "return vec;" << endml;
		}
		else {
			os << "\t\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" is not an array!\");" << endml;
		}
		os << "\t\t}" << endml;
	}
	// Check in base class
	for (const ClassInfo& base : directBaseClasses) {
		os << "\t\tif (" + base.fullName + "::hasProperty(propertyName))" << endml;
		os << "\t\t\treturn " + base.fullName + "::getArrayValue_impl<T>(propertyName);" << endml;
	}
	os << "\t} catch(std::exception&) {" << endml;
	os << "\t\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" cannot be converted to the specified type!\" << \"\\n\";" << endml;
	os << "\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" cannot be converted to the specified type!\");" << endml;
	os << "\t}" << endml;
	os << "\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\" << \"\\n\";" << endml;
	os << "\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\");" << endml;
	os << "}" << endml;
	// ----------------------------------------- getArrayElementValue_impl -----------------------------------------
	os << "template<typename T>" << endml;
	os << "T getArrayElementValue_impl(std::string propertyName, std::size_t index) {" << endml;
	os << "\ttry {" << endml;
	for (const Property& prop : properties) {
		os << "\t\tif (propertyName == \"" + prop.name + "\") {" << endml;
		if (isArrayType(prop.type)) {
			os << "\t\t\tif (index >= " << getArrayTypeSize(prop) << ") throw std::out_of_range(\"getArrayElementValue_impl::ERROR index out of range.\");" << endml;
			os << "\t\t\t" << "return " << LIBRARY_NAME_SPACE << "::convertType<T>(" << prop.name << "[index]);" << endml;
		}
		else {
			os << "\t\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" is not an array!\");" << endml;
		}
		os << "\t\t}" << endml;
	}
	// Check in base class
	for (const ClassInfo& base : directBaseClasses) {
		os << "\t\tif (" + base.fullName + "::hasProperty(propertyName))" << endml;
		os << "\t\t\treturn " + base.fullName + "::getArrayElementValue_impl<T>(propertyName, index);" << endml;
	}
	os << "\t} catch(std::exception& e) {" << endml;
	//os << "\t\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" cannot be converted to the specified type!\" << \"\\n\";" << endml;
	//os << "\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" cannot be converted to the specified type!\");" << endml;
	os << "\t\tstd::cout << e.what() << std::endl;" << endml;
	os << "\t\tthrow e;" << endml;
	os << "\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" cannot be converted to the specified type!\");" << endml;
	os << "\t}" << endml;
	os << "\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\" << \"\\n\";" << endml;
	os << "\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\");" << endml;
	os << "}" << endml;
}

/* Writes all setValue functions for the class to the end of the ostream. */
void Printer::printSetValue(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os) {
	// List of all properties from current
	const std::vector<Property>& properties = currentClass.properties;

	// ----------------------------------------- setValue_impl -----------------------------------------
	os << "template<typename T>" << endml;
	os << "bool setValue_impl(std::string propertyName, T value) {" << endml;
	os << "\ttry {" << endml;
	for (const Property& prop : properties) {
		if (prop.type.isConst) continue;
		os << "\t\tif (propertyName == \"" + prop.name + "\") {" << endml;
		if (!isArrayType(prop.type)) {
			os << "\t\t\t" << LIBRARY_NAME_SPACE << "::setVariableValue(" << prop.name << ", value);" << endml;
			os << "\t\t\treturn true;" << endml;
		}
		else {
			os << "\t\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" is an array!\");" << endml;
		}
		os << "\t\t}" << endml;
	}
	// Check in base class
	for (const ClassInfo& base : directBaseClasses) {
		os << "\t\tif (" + base.fullName + "::setValue_impl<T>(propertyName, value)) return true;" << endml;
	}
	os << "\t} catch(std::exception&) {" << endml;
	os << "\t\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" was set to a value with an incompatible type!\\n\";" << endml;
	os << "\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" was set to a value with an incompatible type!\");" << endml;
	os << "\t}" << endml;
	os << "\treturn false;" << endml;
	//os << "\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\" << \"\\n\";" << endml;
	//os << "\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\");" << endml;
	os << "}" << endml;
	// ----------------------------------------- setArrayValue_impl -----------------------------------------
	os << "template<typename T, std::size_t N>" << endml;
	os << "bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {" << endml;
	os << "\ttry {" << endml;
	for (const Property& prop : properties) {
		if (prop.type.isConst) continue;
		os << "\t\tif (propertyName == \"" + prop.name + "\") {" << endml;
		if (isArrayType(prop.type)) {
			os << "\t\t\tfor (std::size_t i = 0; i < " << getArrayTypeSize(prop) << "; i++) {" << endml;
			os << "\t\t\t\t" << LIBRARY_NAME_SPACE << "::setVariableValue(" << prop.name << "[i], value[i]);" << endml;
			os << "\t\t\t}" << endml;
			os << "\t\t\treturn true;" << endml;
		}
		else {
			os << "\t\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" is not an array!\");" << endml;
		}
		os << "\t\t}" << endml;
	}
	// Check in base class
	for (const ClassInfo& base : directBaseClasses) {
		os << "\t\tif (" + base.fullName + "::setArrayValue_impl<T, N>(propertyName, value)) return true;" << endml;
	}
	os << "\t} catch(std::exception&) {" << endml;
	os << "\t\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" was set to a value with an incompatible type!\\n\";" << endml;
	os << "\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" was set to a value with an incompatible type!\");" << endml;
	os << "\t}" << endml;
	os << "\treturn false;" << endml;
	//os << "\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\" << \"\\n\";" << endml;
	//os << "\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\");" << endml;
	os << "}" << endml;
	// ----------------------------------------- setArrayElementValue_impl -----------------------------------------
	os << "template<typename T>" << endml;
	os << "bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {" << endml;
	os << "\ttry {" << endml;
	for (const Property& prop : properties) {
		if (prop.type.isConst) continue;
		os << "\t\tif (propertyName == \"" + prop.name + "\") {" << endml;

		if (isArrayType(prop.type)) {
			os << "\t\t\tif (index >= " << getArrayTypeSize(prop) << ") throw std::out_of_range(\"setArrayElementValue_impl::ERROR index out of range.\");" << endml;
			os << "\t\t\t" << LIBRARY_NAME_SPACE << "::setVariableValue(" << prop.name << "[index], value);" << endml;
		}
		else {
			os << "\t\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" is not an array!\");" << endml;
		}
		os << "\t\t\treturn true;" << endml;
		os << "\t\t}" << endml;
	}
	// Check in base class
	for (const ClassInfo& base : directBaseClasses) {
		os << "\t\tif (" + base.fullName + "::setArrayElementValue_impl<T>(propertyName, index, value)) return true;" << endml;
	}
	os << "\t} catch(std::exception& e) {" << endml;
	//os << "\t\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" was set to a value with an incompatible type!\\n\";" << endml;
	//os << "\t\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" was set to a value with an incompatible type!\");" << endml;
	os << "\t\tstd::cout << e.what() << std::endl;" << endml;
	os << "\t\tthrow e;" << endml;
	os << "\t}" << endml;
	os << "\treturn false;" << endml;
	//os << "\tstd::cout << \"Warning: The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\" << \"\\n\";" << endml;
	//os << "\tthrow std::invalid_argument(\"The property " + currentClass.fullName + "::\" + propertyName + \" does not exist!\");" << endml;
	os << "}" << endml;
}


void Printer::printOnUpdate(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os) {
	// List of all properties from current
	const std::vector<Property>& properties = currentClass.properties;

	// ----------------------------------------- onUpdate_impl -----------------------------------------
	os << "bool onUpdate(std::string propertyName) {" << endml;
	for (const Property& prop : properties) {
		if (prop.type.isConst) continue;
		os << "\tif (propertyName == \"" + prop.name + "\") {" << endml;
		for (const std::string& value : prop.getAnnotationValue("Update")) {
			os << "\t\t" << value << ";" << endml;
		}
		os << "\t\treturn true;" << endml;
		os << "\t}" << endml;
	}
	// Check in base class
	for (const ClassInfo& base : directBaseClasses) {
		os << "\tif (" + base.fullName + "::onUpdate(propertyName)) return true;" << endml;
	}
	os << "\treturn false;" << endml;
	os << "}" << endml;
}

/* Writes all invoke functions for the class to the end of the ostream. */
void Printer::printInvoke(const ClassInfo& currentClass, const std::vector<ClassInfo>& directBaseClasses, std::ostream& os) {
	// List of all functions from current + base
	std::vector<Function> functions = currentClass.functions;

	// Unique parameters
	std::vector<std::vector<Variable>> parameterVec;
	for (const Function& fun : functions) {
		parameterVec.push_back(fun.parameters);
	}
	parameterVec = getUniqueParameters(parameterVec);

	// invoke
	for (std::vector<Variable> parameters : parameterVec) {
		if (parameters.empty())
			os << "bool invoke(std::string functionName) {" << endml;
		else
			os << "bool invoke(std::string functionName, " + printParameters(parameters) + ") {" << endml;

		for (const Function& fun : currentClass.functions) {
			if (fun.parameters != parameters) continue;
			os << "\tif (functionName == \"" + fun.name + "\") {" << endml;
			os << "\t\t" + LIBRARY_NAME_SPACE + "::invoke(&" + currentClass.name + "::" + fun.name + ", this" + printParameterPack(parameters.size()) + ");" << endml;
			os << "\t\treturn true;" << endml;
			os << "\t}" << endml;
		}
		os << "\treturn false;" << endml;
		os << "}" << endml;
	}

	// invoke_impl
	os << "template<typename... Ts>" << endml;
	os << "bool invoke_impl(std::string functionName, Ts... args) { return false; }" << endml;

	std::vector<std::size_t> parameterAmounts = getUniqueParameterAmount(parameterVec);
	for (const std::size_t& amount : parameterAmounts) {
		os << "template<" + printTemplateArguments(amount, true) + ">" << endml;
		os << "bool invoke_impl(std::string functionName, " + printTemplateArguments(amount) + ") {" << endml;
		bool first = true;
		for (const Function& fun : functions) {
			if (fun.parameters.size() != amount) continue;
			if (!first) {
				os << "\t";
			}
			else {
				os << "\t";
				first = false;
			}
			os << "if (functionName == \"" + fun.name + "\") {" << endml;
			os << "\t\ttry {" << endml;
			os << "\t\t\tinvoke(functionName";
			if (amount > 0)
				os << ", ";
			for (std::size_t i = 0; i < amount; i++) {
				os << "(" + printVariableType(fun.parameters[i].type, true) + ")";
				os << LIBRARY_NAME_SPACE + "::convertType<" << printVariableType(fun.parameters[i].type, true) << ">(arg" + std::to_string(i) + ")";
				if (i != amount - 1)
					os << ", ";
			}
			os << ");" << endml;
			os << "\t\t\treturn true;" << endml;
			os << "\t\t} catch(std::exception&) {}" << endml;
			os << "\t}" << endml;
		}
		// Call invoke in all base classes
		for (const ClassInfo& baseClass : directBaseClasses) {
			os << "\tif (" << baseClass.fullName << "::invoke_impl(functionName, ";
			for (std::size_t i = 0; i < amount; i++) {
				os << "arg" << std::to_string(i);
				if (i != amount - 1)
					os << ",";
			}
			os << "))" << endml;
			os << "\t\treturn true;" << endml;
		}
		os << "\treturn false;" << endml;
		os << "}" << endml;
	}
}