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
	newClass.name = "WindowScale";\
	newClass.size = sizeof(WindowScale);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "stretchWidth";\
	newProperty.type.name = "bool";\
	newProperty.type.size = sizeof(bool);\
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
	newProperty.name = "stretchHeight";\
	newProperty.type.name = "bool";\
	newProperty.type.size = sizeof(bool);\
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
	newProperty.name = "widthScale";\
	newProperty.type.name = "float";\
	newProperty.type.size = sizeof(float);\
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
	newProperty.name = "heightScale";\
	newProperty.type.name = "float";\
	newProperty.type.size = sizeof(float);\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
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
		if (propertyName == "stretchWidth") {\
			return Mirror::convertType<bool, T>(instance->stretchWidth);\
		}\
		if (propertyName == "stretchHeight") {\
			return Mirror::convertType<bool, T>(instance->stretchHeight);\
		}\
		if (propertyName == "widthScale") {\
			return Mirror::convertType<float, T>(instance->widthScale);\
		}\
		if (propertyName == "heightScale") {\
			return Mirror::convertType<float, T>(instance->heightScale);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property WindowScale::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property WindowScale::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property WindowScale::" + propertyName + " does not exist in WindowScale!" << "\n";\
	throw std::invalid_argument("The property WindowScale::" + propertyName + " does not exist in WindowScale!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "stretchWidth") {\
			throw std::invalid_argument("The property WindowScale::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "stretchHeight") {\
			throw std::invalid_argument("The property WindowScale::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "widthScale") {\
			throw std::invalid_argument("The property WindowScale::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "heightScale") {\
			throw std::invalid_argument("The property WindowScale::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property WindowScale::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property WindowScale::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property WindowScale::" + propertyName + " does not exist in WindowScale!" << "\n";\
	throw std::invalid_argument("The property WindowScale::" + propertyName + " does not exist in WindowScale!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "stretchWidth") {\
			instance->stretchWidth = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "stretchHeight") {\
			instance->stretchHeight = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "widthScale") {\
			instance->widthScale = Mirror::convertType<T, float>(value);\
			return;\
		}\
		if (propertyName == "heightScale") {\
			instance->heightScale = Mirror::convertType<T, float>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property WindowScale::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property WindowScale::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property WindowScale::" + propertyName + " does not exist in WindowScale!" << "\n";\
	throw std::invalid_argument("The property WindowScale::" + propertyName + " does not exist in WindowScale!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "stretchWidth") {\
			throw std::invalid_argument("The property WindowScale::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "stretchHeight") {\
			throw std::invalid_argument("The property WindowScale::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "widthScale") {\
			throw std::invalid_argument("The property WindowScale::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "heightScale") {\
			throw std::invalid_argument("The property WindowScale::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property WindowScale::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property WindowScale::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property WindowScale::" + propertyName + " does not exist in WindowScale!" << "\n";\
	throw std::invalid_argument("The property WindowScale::" + propertyName + " does not exist in WindowScale!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(stretchWidth, os);\
		Mirror::serialize(stretchHeight, os);\
		Mirror::serialize(widthScale, os);\
		Mirror::serialize(heightScale, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(stretchWidth, is);\
		Mirror::deserialize(stretchHeight, is);\
		Mirror::deserialize(widthScale, is);\
		Mirror::deserialize(heightScale, is);\
}
#endif