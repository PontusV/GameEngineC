#pragma once
#include <ReflectionParser/ReflectionTypes.h>
#include <string>
#include <cstddef>
/* Can parse a string of the decaration of a property/variable/field. Can find out if the property is a pointer, reference, const, array and the size of the array.
 * To find out if the property is Static you need to use the Parser.h
 */
class PropertyTypeParser {
public:
	Mirror::VariableType parse(std::string spelling) const;

	/* Registers the name as an enumeration. This is used by the parser to determine if the given spelling of a property is of an enum type. */
	void registerEnum(std::string fullName);

private:
	/* Returns the name of the property together with the namespaces it belongs to. */
	std::string getName(std::string spelling) const;
	std::size_t getNameStartIndex(std::string spelling) const;
	std::size_t getNameEndIndex(std::string spelling) const;
	bool isPointer(std::string spelling) const;
	bool isReference(std::string spelling) const;
	bool isConst(std::string spelling) const;
	/* Checks if the Property type name matches a registered Enum type name. */
	bool isEnum(std::string spelling) const;
	bool isArray(std::string spelling) const;
	std::size_t getArraySize(std::string spelling) const;
	std::size_t getPointerCount(std::string spelling) const;
	std::vector<std::string> getTemplateParameters(std::string spelling) const;

	std::string stripTemplate(std::string spelling) const;

private:

	std::vector<std::string> enumerations;
};