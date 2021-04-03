#include "CursorType.h"
#include "Cursor.h"


CursorType::CursorType(const CXType& handle) : handle(handle) {
}

CursorType::~CursorType() {
}



bool CursorType::isReference() const {
	std::string typeName = getSpelling();
	std::size_t refIndex = typeName.find_last_of('&');
	if (refIndex != std::string::npos) {
		return typeName.find_first_of('<') == std::string::npos || refIndex > typeName.find_last_of('>');
	}
	return false;
}

bool CursorType::isPointer() const {
	std::string typeName = getSpelling();
	std::size_t pointerIndex = typeName.find_last_of('*');
	if (pointerIndex != std::string::npos) {
		return typeName.find_first_of('<') == std::string::npos || pointerIndex > typeName.find_last_of('>');
	}
	return false;
}

std::size_t CursorType::getPointerCount() const {
	std::size_t count = 0;
	std::string typeName = getSpelling();
	std::size_t index = typeName.find_last_of('>');
	if (index != std::string::npos) {
		typeName = typeName.substr(index);
		for (char& c : typeName) {
			if (c == '*') {
				count++;
			}
		}
	}
	return count;
}

Cursor CursorType::getDeclaration() const {
	return clang_getTypeDeclaration(handle);
}

CXTypeKind CursorType::getKind() const {
	return handle.kind;
}

std::string CursorType::getSpelling() const {
	return clang_getCString(clang_getTypeSpelling(handle));
}

std::string CursorType::getFullName() const {
	std::string spelling = getSpelling();
	std::size_t arrayStart = spelling.find_first_of('[');
	if (arrayStart != std::string::npos) {
		return spelling.substr(0, arrayStart);
	}
	return spelling;
}

bool CursorType::isArray() const {
	return handle.kind == CXType_ConstantArray;
}

bool CursorType::isEnum() const {
	return handle.kind == CXType_Enum;
}

long long CursorType::getArraySize() const {
	long long arraySize = clang_getArraySize(handle);
	if (arraySize < 0)
		arraySize = 0;
	return arraySize;
}

int CursorType::getArgumentCount() const {
	return clang_getNumArgTypes(handle);
}

CursorType CursorType::getArgumentType(unsigned index) const {
	return clang_getArgType(handle, index);
}

CursorType CursorType::getCanonicalType() const {
	return clang_getCanonicalType(handle);
}

bool CursorType::isConst() const {
	//clang_isConstQualifiedType(handle) is bugged for references
	if (clang_isConstQualifiedType(handle))
		return true;

	std::string typeName = getSpelling();
	std::size_t index = typeName.find(std::string("const "));
	if (index == std::string::npos) // Not found
		return false;
	if (index == 0 || index < getNameStartIndex())
		return true;

	return false;
}

std::string CursorType::getName() const {
	std::string spelling = getSpelling();
	std::size_t startIndex = getNameStartIndex();
	std::string name = spelling.substr(startIndex, getNameEndIndex() - startIndex);
	// Add unsigned
	if (spelling.find("unsigned") != std::string::npos)
		name = "unsigned " + name;
	return name;
}

std::size_t CursorType::getNameStartIndex() const {
	std::string spelling = getSpelling();

	std::size_t templateIndex = spelling.find('<');
	if (templateIndex != std::string::npos) {
		spelling = spelling.substr(0, templateIndex);
	}
	else {
		spelling = spelling.substr(0, getNameEndIndex());
	}
	std::size_t startIndex = spelling.find_last_of(' ');
	std::size_t arrayStart = spelling.find('[');
	if (arrayStart != std::string::npos) { // Check if type is array
		std::string sub = spelling.substr(0, arrayStart-1);
		startIndex = sub.find_last_of(' ');
	}
	if (startIndex != std::string::npos)
		return startIndex+1;
	return 0;
}

std::size_t CursorType::getNameEndIndex() const {
	std::string spelling = getSpelling();
	// Remove pointer and reference symbols
	// Relies on the symbols being separated from the name with a whitespace and that the symbols come after the name, if not an out of range exception will be cast.
	while (spelling.find('&') != std::string::npos || spelling.find('*') != std::string::npos || spelling.find('[') != std::string::npos)
		spelling = spelling.substr(0, spelling.find_last_of(' '));
	return spelling.length();
}