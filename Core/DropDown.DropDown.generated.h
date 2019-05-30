#ifndef REFLECTION_PARSER
#pragma once
#include <ReflectionParser.h>
#include <cstddef>
#include <stdexcept>
#include <type_traits>

#ifdef GENERATED_BODY
#undef GENERATED_BODY
#endif
#define GENERATED_BODY(...) \
private: \
Mirror::Class getTypeImpl() { \
	Mirror::Class newClass(0);\
	newClass.name = "DropDown";\
	newClass.size = sizeof(DropDown);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "boxWidth";\
	newProperty.type.name = "int";\
	newProperty.type.size = sizeof(int);\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "boxPaddingX";\
	newProperty.type.name = "int";\
	newProperty.type.size = sizeof(int);\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "boxPaddingY";\
	newProperty.type.name = "int";\
	newProperty.type.size = sizeof(int);\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "optionHeight";\
	newProperty.type.name = "int";\
	newProperty.type.size = sizeof(int);\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "optionFont";\
	newProperty.type.name = "Core::Font";\
	newProperty.type.size = sizeof(Core::Font);\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "optionTextColor";\
	newProperty.type.name = "Core::Color";\
	newProperty.type.size = sizeof(Core::Color);\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "text";\
	newProperty.type.name = "Core::Text";\
	newProperty.type.size = sizeof(Core::Text);\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "options";\
	newProperty.type.name = "int";\
	newProperty.type.size = sizeof(int);\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
public:\
Mirror::Class getType() {\
	static Mirror::Class type = getTypeImpl();\
	return type;\
}\
template<typename T, typename ClassType>\
static T getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "boxWidth") {\
			return Mirror::convertType<int, T>(instance->boxWidth);\
		}\
		if (propertyName == "boxPaddingX") {\
			return Mirror::convertType<int, T>(instance->boxPaddingX);\
		}\
		if (propertyName == "boxPaddingY") {\
			return Mirror::convertType<int, T>(instance->boxPaddingY);\
		}\
		if (propertyName == "optionHeight") {\
			return Mirror::convertType<int, T>(instance->optionHeight);\
		}\
		if (propertyName == "optionFont") {\
			return Mirror::convertType<Core::Font, T>(instance->optionFont);\
		}\
		if (propertyName == "optionTextColor") {\
			return Mirror::convertType<Core::Color, T>(instance->optionTextColor);\
		}\
		if (propertyName == "text") {\
			return Mirror::convertType<Core::Text, T>(instance->text);\
		}\
		if (propertyName == "options") {\
			return Mirror::convertType<int, T>(instance->options);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property DropDown::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property DropDown::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property DropDown::" + propertyName + " does not exist in DropDown!" << "\n";\
	throw std::invalid_argument("The property DropDown::" + propertyName + " does not exist in DropDown!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "boxWidth") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "boxPaddingX") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "boxPaddingY") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionHeight") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionFont") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionTextColor") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "text") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "options") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property DropDown::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property DropDown::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property DropDown::" + propertyName + " does not exist in DropDown!" << "\n";\
	throw std::invalid_argument("The property DropDown::" + propertyName + " does not exist in DropDown!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "boxWidth") {\
			instance->boxWidth = Mirror::convertType<T, int>(value);\
			return;\
		}\
		if (propertyName == "boxPaddingX") {\
			instance->boxPaddingX = Mirror::convertType<T, int>(value);\
			return;\
		}\
		if (propertyName == "boxPaddingY") {\
			instance->boxPaddingY = Mirror::convertType<T, int>(value);\
			return;\
		}\
		if (propertyName == "optionHeight") {\
			instance->optionHeight = Mirror::convertType<T, int>(value);\
			return;\
		}\
		if (propertyName == "optionFont") {\
			instance->optionFont = Mirror::convertType<T, Core::Font>(value);\
			return;\
		}\
		if (propertyName == "optionTextColor") {\
			instance->optionTextColor = Mirror::convertType<T, Core::Color>(value);\
			return;\
		}\
		if (propertyName == "text") {\
			instance->text = Mirror::convertType<T, Core::Text>(value);\
			return;\
		}\
		if (propertyName == "options") {\
			instance->options = Mirror::convertType<T, int>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property DropDown::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property DropDown::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property DropDown::" + propertyName + " does not exist in DropDown!" << "\n";\
	throw std::invalid_argument("The property DropDown::" + propertyName + " does not exist in DropDown!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "boxWidth") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "boxPaddingX") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "boxPaddingY") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "optionHeight") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "optionFont") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "optionTextColor") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "text") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "options") {\
			throw std::invalid_argument("The property DropDown::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property DropDown::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property DropDown::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property DropDown::" + propertyName + " does not exist in DropDown!" << "\n";\
	throw std::invalid_argument("The property DropDown::" + propertyName + " does not exist in DropDown!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(boxWidth, os);\
		Mirror::serialize(boxPaddingX, os);\
		Mirror::serialize(boxPaddingY, os);\
		Mirror::serialize(optionHeight, os);\
		Mirror::serialize(optionFont, os);\
		Mirror::serialize(optionTextColor, os);\
		Mirror::serialize(text, os);\
		Mirror::serialize(options, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(boxWidth, is);\
		Mirror::deserialize(boxPaddingX, is);\
		Mirror::deserialize(boxPaddingY, is);\
		Mirror::deserialize(optionHeight, is);\
		Mirror::deserialize(optionFont, is);\
		Mirror::deserialize(optionTextColor, is);\
		Mirror::deserialize(text, is);\
		Mirror::deserialize(options, is);\
}
#endif