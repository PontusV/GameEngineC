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
	newClass.name = "HorizontalLayoutGroup";\
	newClass.size = sizeof(HorizontalLayoutGroup);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "shrinkableChildWidth";\
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
	newProperty.name = "shrinkableChildHeight";\
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
	newProperty.name = "childForceExpandWidth";\
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
	newProperty.name = "childForceExpandHeight";\
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
		if (propertyName == "shrinkableChildWidth") {\
			return Mirror::convertType<bool, T>(instance->shrinkableChildWidth);\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			return Mirror::convertType<bool, T>(instance->shrinkableChildHeight);\
		}\
		if (propertyName == "childForceExpandWidth") {\
			return Mirror::convertType<bool, T>(instance->childForceExpandWidth);\
		}\
		if (propertyName == "childForceExpandHeight") {\
			return Mirror::convertType<bool, T>(instance->childForceExpandHeight);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property HorizontalLayoutGroup::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property HorizontalLayoutGroup::" + propertyName + " does not exist in HorizontalLayoutGroup!" << "\n";\
	throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " does not exist in HorizontalLayoutGroup!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "shrinkableChildWidth") {\
			throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandWidth") {\
			throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandHeight") {\
			throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property HorizontalLayoutGroup::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property HorizontalLayoutGroup::" + propertyName + " does not exist in HorizontalLayoutGroup!" << "\n";\
	throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " does not exist in HorizontalLayoutGroup!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "shrinkableChildWidth") {\
			instance->shrinkableChildWidth = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			instance->shrinkableChildHeight = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "childForceExpandWidth") {\
			instance->childForceExpandWidth = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "childForceExpandHeight") {\
			instance->childForceExpandHeight = Mirror::convertType<T, bool>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property HorizontalLayoutGroup::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property HorizontalLayoutGroup::" + propertyName + " does not exist in HorizontalLayoutGroup!" << "\n";\
	throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " does not exist in HorizontalLayoutGroup!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "shrinkableChildWidth") {\
			throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "childForceExpandWidth") {\
			throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "childForceExpandHeight") {\
			throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property HorizontalLayoutGroup::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property HorizontalLayoutGroup::" + propertyName + " does not exist in HorizontalLayoutGroup!" << "\n";\
	throw std::invalid_argument("The property HorizontalLayoutGroup::" + propertyName + " does not exist in HorizontalLayoutGroup!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(shrinkableChildWidth, os);\
		Mirror::serialize(shrinkableChildHeight, os);\
		Mirror::serialize(childForceExpandWidth, os);\
		Mirror::serialize(childForceExpandHeight, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(shrinkableChildWidth, is);\
		Mirror::deserialize(shrinkableChildHeight, is);\
		Mirror::deserialize(childForceExpandWidth, is);\
		Mirror::deserialize(childForceExpandHeight, is);\
}
#endif