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
	newClass.name = "WindowAnchor";\
	newClass.size = sizeof(WindowAnchor);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "anchor";\
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
	newProperty.name = "offset";\
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
		if (propertyName == "anchor") {\
			return Mirror::convertType<int, T>(instance->anchor);\
		}\
		if (propertyName == "offset") {\
			return Mirror::convertType<int, T>(instance->offset);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property WindowAnchor::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property WindowAnchor::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property WindowAnchor::" + propertyName + " does not exist in WindowAnchor!" << "\n";\
	throw std::invalid_argument("The property WindowAnchor::" + propertyName + " does not exist in WindowAnchor!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "anchor") {\
			throw std::invalid_argument("The property WindowAnchor::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "offset") {\
			throw std::invalid_argument("The property WindowAnchor::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property WindowAnchor::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property WindowAnchor::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property WindowAnchor::" + propertyName + " does not exist in WindowAnchor!" << "\n";\
	throw std::invalid_argument("The property WindowAnchor::" + propertyName + " does not exist in WindowAnchor!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "anchor") {\
			instance->anchor = Mirror::convertType<T, int>(value);\
			return;\
		}\
		if (propertyName == "offset") {\
			instance->offset = Mirror::convertType<T, int>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property WindowAnchor::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property WindowAnchor::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property WindowAnchor::" + propertyName + " does not exist in WindowAnchor!" << "\n";\
	throw std::invalid_argument("The property WindowAnchor::" + propertyName + " does not exist in WindowAnchor!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "anchor") {\
			throw std::invalid_argument("The property WindowAnchor::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "offset") {\
			throw std::invalid_argument("The property WindowAnchor::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property WindowAnchor::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property WindowAnchor::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property WindowAnchor::" + propertyName + " does not exist in WindowAnchor!" << "\n";\
	throw std::invalid_argument("The property WindowAnchor::" + propertyName + " does not exist in WindowAnchor!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(anchor, os);\
		Mirror::serialize(offset, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(anchor, is);\
		Mirror::deserialize(offset, is);\
}
#endif