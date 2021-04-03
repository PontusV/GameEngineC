#pragma once
#include "Cursor.h"
#include "ClassInfo.h"
#include "EnumInfo.h"
#include "PropertyParser.h"
#include <cstddef>
#include <string>
#include <vector>
#include <ReflectionParser/ReflectionTypes.h>

class Parser {
public:
	/* Parser the target translation unit and generates reflection data, which is stored in @var classes. These classes may be retrieved with getClasses() */
	void parseTranslationUnit(const char* target, const std::vector<const char*>& cmdArguments, std::size_t& typeIDCounter);
	const std::vector<ClassInfo>& getClasses() const;
	const std::vector<EnumInfo>& getEnumerations() const;

private:
	void parse(Cursor cursor, std::size_t& typeIDCounter, std::string nameSpace = "");
	void parseEnum(Cursor cursor, std::string nameSpace);

	PropertyTypeParser propertyParser;
	std::vector<ClassInfo> classes;
	std::vector<EnumInfo> enumerations;
};