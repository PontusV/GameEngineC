#include "PropertyParser.h"
#include <sstream>
#include <algorithm>
using namespace Mirror;


VariableType PropertyTypeParser::parse(std::string spelling) const {
	VariableType type;
	type.name = getName(spelling);
	type.isPointer = isPointer(spelling);
	type.isReference = isReference(spelling);
	type.arraySize = getArraySize(spelling);
	type.isArray = isArray(spelling);
	type.isConst = isConst(spelling);
	type.isEnumeration = isEnum(spelling);
	for (std::string templateParam : getTemplateParameters(spelling)) {
		type.templateParams.push_back(parse(templateParam));
	}
	/*std::cout << "Spelling:\t" << spelling << std::endl;
	std::cout << "Name:    \t" << type.name << std::endl;
	std::cout << "isPointer:     " << type.isPointer << std::endl;
	std::cout << "isReference:   " << type.isReference << std::endl;
	std::cout << "isConst:       " << type.isConst << std::endl;
	std::cout << "isEnum:        " << type.isEnumeration << std::endl;
	std::cout << "isArray:       " << type.isArray << std::endl;
	std::cout << "arraySize:     " << type.arraySize << std::endl;
	std::cout << std::endl;*/
	return type;
}

std::string PropertyTypeParser::getName(std::string spelling) const {
	std::size_t startIndex = getNameStartIndex(spelling);
	std::string name = spelling.substr(startIndex, getNameEndIndex(spelling) - startIndex);
	// Adds unsigned to name if the type is unsigned
	if (spelling.find("unsigned") != std::string::npos)
		name = "unsigned " + name;
	return name;
}

std::size_t PropertyTypeParser::getNameStartIndex(std::string spelling) const {
	std::size_t startIndex = 0;
	while (spelling.at(startIndex) == ' ') { // Removes any whitespace before the actually spelling of the type
		startIndex++;
	}
	spelling = spelling.substr(startIndex, getNameEndIndex(spelling));
	std::size_t lastSpaceIndex = spelling.find_last_of(' ');
	if (lastSpaceIndex != std::string::npos)
		return lastSpaceIndex + 1;
	return startIndex;
}

std::size_t PropertyTypeParser::getNameEndIndex(std::string spelling) const {
	std::size_t templateIndex = spelling.find_first_of('<');
	std::size_t arrayIndex = spelling.find_first_of('[');
	std::size_t pointerIndex = spelling.find_first_of('*');
	std::size_t refIndex = spelling.find_first_of('&');
	// Removes Template symbols
	if (templateIndex != std::string::npos)
		spelling = spelling.substr(0, templateIndex);
	// Removes Array symbols
	if (arrayIndex != std::string::npos)
		spelling = spelling.substr(0, arrayIndex);
	// Removes Pointer symbols
	if (pointerIndex != std::string::npos)
		spelling = spelling.substr(0, pointerIndex);
	// Removes Reference symbol
	if (refIndex != std::string::npos)
		spelling = spelling.substr(0, refIndex);
	// Removes whitespace after name
	while (spelling.back() == ' ')
		spelling.pop_back();
	return spelling.length();
}

bool PropertyTypeParser::isPointer(std::string spelling) const {
	std::string typeName = stripTemplate(spelling);
	std::size_t pointerIndex = typeName.find_last_of('*');
	if (pointerIndex != std::string::npos) {
		return typeName.find_first_of('<') == std::string::npos || pointerIndex > typeName.find_last_of('>');
	}
	return false;
}

bool PropertyTypeParser::isReference(std::string spelling) const {
	std::string typeName = stripTemplate(spelling);
	std::size_t refIndex = typeName.find_last_of('&');
	if (refIndex != std::string::npos) {
		return typeName.find_first_of('<') == std::string::npos || refIndex > typeName.find_last_of('>');
	}
	return false;
}

bool PropertyTypeParser::isConst(std::string spelling) const {
	std::string typeName = stripTemplate(spelling);
	std::size_t index = typeName.find(std::string("const "));
	if (index == std::string::npos) // Not found
		return false;
	if (index == 0 || index < getNameStartIndex(spelling))
		return true;

	return false;
}

bool PropertyTypeParser::isEnum(std::string spelling) const {
	std::string name = getName(spelling);
	for (const std::string enumName : enumerations) {
		if (enumName == name)
			return true;
	}
	return false;
}

bool PropertyTypeParser::isArray(std::string spelling) const {
	return stripTemplate(spelling).find('[') != std::string::npos;
}

std::size_t PropertyTypeParser::getArraySize(std::string spelling) const {
	if (!isArray(spelling)) return 0;
	std::size_t arrayStart = spelling.find('[');
	std::size_t arrayEnd = spelling.find(']');
	std::string sizeString = spelling.substr(arrayStart+1, arrayEnd - arrayStart - 1);
	return std::stoi(sizeString);
}

std::size_t PropertyTypeParser::getPointerCount(std::string spelling) const {
	std::size_t count = 0;
	std::string typeName = spelling;
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

std::vector<std::string> PropertyTypeParser::getTemplateParameters(std::string spelling) const {
	std::vector<std::string> parameters;
	std::size_t startIndex = spelling.find_first_of('<');
	std::size_t endIndex = spelling.find_last_of('>');
	if (startIndex != std::string::npos) {
		std::string parameterString = spelling.substr(startIndex + 1, endIndex - startIndex - 1);
		// Test
		std::string parameterBuffer;
		std::size_t templateCount = 0;
		for (const char& c : parameterString) {
			parameterBuffer.push_back(c);
			if (c == '<') {
				templateCount++;
			}
			else if (c == '>'){
				templateCount--;
			}
			else if (templateCount == 0 && c == ',') {
				parameterBuffer.pop_back();
				parameters.push_back(parameterBuffer);
			}
		}
		parameters.push_back(parameterBuffer);
	}
	return parameters;
}

void PropertyTypeParser::registerEnum(std::string fullName) {
	enumerations.push_back(fullName);
}

std::string PropertyTypeParser::stripTemplate(std::string spelling) const {
	std::size_t templateIndex = spelling.find_first_of('<');
	if (templateIndex != std::string::npos) {
		spelling.erase(templateIndex, spelling.find_last_of('>'));
	}
	return spelling;
}