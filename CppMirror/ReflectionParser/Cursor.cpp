#include "Cursor.h"
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace Mirror;

Cursor::Cursor(const CXCursor& handle) : handle(handle) {
}

Cursor::~Cursor() {
}



CXCursorKind Cursor::getKind() const {
	return handle.kind;
}

Cursor Cursor::getLexicalParent() const {
	return clang_getCursorLexicalParent(handle);
}

Cursor Cursor::getTemplateSpecialization() const {
	return clang_getSpecializedCursorTemplate(handle);
}

std::string Cursor::getSpelling() const {
	return clang_getCString(clang_getCursorSpelling(handle));
}

std::string Cursor::getMangledName() const {
	return clang_getCString(clang_Cursor_getMangling(handle));
}

std::string Cursor::getUSR() const {
	return clang_getCString(clang_getCursorUSR(handle));
}

std::string Cursor::getDisplayName() const {
	return clang_getCString(clang_getCursorDisplayName(handle));
}

std::string Cursor::getSourceFile() const {
	CXSourceRange range = clang_Cursor_getSpellingNameRange(handle, 0, 0);
	CXSourceLocation start = clang_getRangeStart(range);

	CXFile file;
	unsigned line, column, offset;

	clang_getFileLocation(start, &file, &line, &column, &offset);

	return clang_getCString(clang_getFileName(file));
}

bool Cursor::isDefinition() const {
	return clang_isCursorDefinition(handle);
}
bool Cursor::isConstMethod() const {
	return clang_CXXMethod_isConst(handle);
}

bool Cursor::isStaticMethod() const {
	return clang_CXXMethod_isStatic(handle);
}

bool Cursor::isVirtualMethod() const {
	return clang_CXXMethod_isVirtual(handle);
}

bool Cursor::isPureVirtualMethod() const {
	return clang_CXXMethod_isPureVirtual(handle);
}

bool Cursor::isVariadicMethod() const {
	return clang_Cursor_isVariadic(handle);
}

CursorType Cursor::getType() const {
	return CursorType(clang_getCursorType(handle));
}

CursorType Cursor::getReturnType() const {
	return CursorType(clang_getCursorResultType(handle));
}

CursorType Cursor::getTypedefType() const {
	return CursorType(clang_getTypedefDeclUnderlyingType(handle));
}

CX_CXXAccessSpecifier Cursor::getAccessSpecifier() const {
	return clang_getCXXAccessSpecifier(handle);
}

CX_StorageClass Cursor::getStorageClass() const {
	return clang_Cursor_getStorageClass(handle);
}

std::vector<Cursor> Cursor::getChildren() const {
	std::vector<Cursor> children;

	auto visitor = [](CXCursor cursor, CXCursor parent, CXClientData data) {
		auto container = static_cast<std::vector<Cursor>*>(data);
		container->emplace_back(cursor);

		if (cursor.kind == CXCursor_LastPreprocessing)
			return CXChildVisit_Break;

		return CXChildVisit_Continue;
	};

	clang_visitChildren(handle, visitor, &children);

	return children;
}

bool getNextAnnotationLine(std::stringstream& ss, std::string& str) {
	char prevCh = 0;
	char ch;
	int count = 0;
	bool assign = false;
	bool open = false;
	str.clear();
	while (ss.get(ch)) {
		if (ch == ',' && count == 0)
			break;
		if (ch == '=') {
			if (assign) {
				std::cout << "Too many '=' in annotation!";
				throw std::invalid_argument("Too many '=' in annotation!");
			}
			assign = true;
		}
		else if (ch == '(') {
			count++;
			if (prevCh == '=') {
				open = true;
				continue;
			}
		}
		else if (ch == ')') {
			count--;
			if (count == 0 && open)
				continue;
		}
		str.push_back(ch);
		prevCh = ch;
	}
	if (str.empty())
		return false;
	if (count > 0) {
		std::cout << "Too many '(' in annotation!";
		throw std::invalid_argument("Too many '(' in annotation!");
	}
	if (count < 0) {
		std::cout << "Too many ')' in annotation!";
		throw std::invalid_argument("Too many ')' in annotation!");
	}
	return true;
}

Annotation getAnnotation(std::string annotationLine) {
	Annotation annotation;
	std::size_t valueIndex = annotationLine.find('=');
	if (valueIndex == std::string::npos) {
		annotation.name = annotationLine;
		return annotation;
	}
	annotation.name = annotationLine.substr(0, valueIndex);
	if (++valueIndex == annotationLine.size()) {
		std::cout << "Assignment operator found without any value to assign!";
		throw std::invalid_argument("Assignment operator found without any value to assign!");
	}

	std::string valueLine = annotationLine.substr(valueIndex, annotationLine.size() - valueIndex);
	std::stringstream ss(valueLine);
	std::string str;
	while (std::getline(ss, str, ',')) {
		annotation.values.push_back(str);
	}
	return annotation;
}

std::vector<Annotation> Cursor::getAnnotateAttributes() const {
	std::vector<Annotation> annotationVector;

	for (Cursor& child : getChildren()) {
		if (child.getKind() == CXCursor_AnnotateAttr) {
			std::string attributes = child.getSpelling();
			attributes.erase(std::remove_if(attributes.begin(), attributes.end(), isspace), attributes.end()); // Erases all whitespace

			std::stringstream ss(attributes);
			std::string str;
			while (getNextAnnotationLine(ss, str)) {
				annotationVector.push_back(getAnnotation(str));
			}
		}
	}

	return annotationVector;
}
std::vector<VariableType> Cursor::getTemplateParameterTypes() const {
	std::vector<VariableType> templateTypes;
	/*
	std::size_t argumentCount = clang_Cursor_getNumTemplateArguments(handle);
	clang_Cursor_
	*/
	/*for (std::size_t i = 0; i < argumentCount; i++) {
		CursorType type = clang_Cursor_getTemplateArgumentType(handle, i);
		std::cout << type.getSpelling() << std::endl;
	}*/
	/*CursorType type = clang_Cursor_getTemplateArgumentType(handle, 0);
	std::cout << "asdf: " << type.getSpelling() << std::endl;
	std::cout << argumentCount << std::endl;
	*/
	return templateTypes;
}

int Cursor::getArgumentCount() const {
	return getType().getArgumentCount();
}

Cursor Cursor::getArgument(unsigned index) const {
	return clang_Cursor_getArgument(handle, index);
}

std::vector<Cursor> Cursor::getArguments() const {
	std::vector<Cursor> arguments;

	int argumentCount = getArgumentCount();
	for (int i = 0; i < argumentCount; i++) {
		arguments.push_back(getArgument(i));
	}

	return arguments;
}