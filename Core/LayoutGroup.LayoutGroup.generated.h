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
	newClass.name = "LayoutGroup";\
	newClass.size = sizeof(LayoutGroup);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "paddingTop";\
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
	newProperty.name = "paddingRight";\
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
	newProperty.name = "paddingBottom";\
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
	newProperty.name = "paddingLeft";\
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
	newProperty.name = "spacing";\
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
	newProperty.name = "childAlignment";\
	newProperty.type.name = "Core::Alignment";\
	newProperty.type.size = sizeof(Core::Alignment);\
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
		if (propertyName == "paddingTop") {\
			return Mirror::convertType<float, T>(instance->paddingTop);\
		}\
		if (propertyName == "paddingRight") {\
			return Mirror::convertType<float, T>(instance->paddingRight);\
		}\
		if (propertyName == "paddingBottom") {\
			return Mirror::convertType<float, T>(instance->paddingBottom);\
		}\
		if (propertyName == "paddingLeft") {\
			return Mirror::convertType<float, T>(instance->paddingLeft);\
		}\
		if (propertyName == "spacing") {\
			return Mirror::convertType<float, T>(instance->spacing);\
		}\
		if (propertyName == "childAlignment") {\
			return Mirror::convertType<Core::Alignment, T>(instance->childAlignment);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property LayoutGroup::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property LayoutGroup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property LayoutGroup::" + propertyName + " does not exist in LayoutGroup!" << "\n";\
	throw std::invalid_argument("The property LayoutGroup::" + propertyName + " does not exist in LayoutGroup!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "paddingTop") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "paddingRight") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "paddingBottom") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "paddingLeft") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "spacing") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childAlignment") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property LayoutGroup::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property LayoutGroup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property LayoutGroup::" + propertyName + " does not exist in LayoutGroup!" << "\n";\
	throw std::invalid_argument("The property LayoutGroup::" + propertyName + " does not exist in LayoutGroup!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "paddingTop") {\
			instance->paddingTop = Mirror::convertType<T, float>(value);\
			return;\
		}\
		if (propertyName == "paddingRight") {\
			instance->paddingRight = Mirror::convertType<T, float>(value);\
			return;\
		}\
		if (propertyName == "paddingBottom") {\
			instance->paddingBottom = Mirror::convertType<T, float>(value);\
			return;\
		}\
		if (propertyName == "paddingLeft") {\
			instance->paddingLeft = Mirror::convertType<T, float>(value);\
			return;\
		}\
		if (propertyName == "spacing") {\
			instance->spacing = Mirror::convertType<T, float>(value);\
			return;\
		}\
		if (propertyName == "childAlignment") {\
			instance->childAlignment = Mirror::convertType<T, Core::Alignment>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property LayoutGroup::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property LayoutGroup::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property LayoutGroup::" + propertyName + " does not exist in LayoutGroup!" << "\n";\
	throw std::invalid_argument("The property LayoutGroup::" + propertyName + " does not exist in LayoutGroup!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "paddingTop") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "paddingRight") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "paddingBottom") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "paddingLeft") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "spacing") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "childAlignment") {\
			throw std::invalid_argument("The property LayoutGroup::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property LayoutGroup::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property LayoutGroup::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property LayoutGroup::" + propertyName + " does not exist in LayoutGroup!" << "\n";\
	throw std::invalid_argument("The property LayoutGroup::" + propertyName + " does not exist in LayoutGroup!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(paddingTop, os);\
		Mirror::serialize(paddingRight, os);\
		Mirror::serialize(paddingBottom, os);\
		Mirror::serialize(paddingLeft, os);\
		Mirror::serialize(spacing, os);\
		Mirror::serialize(childAlignment, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(paddingTop, is);\
		Mirror::deserialize(paddingRight, is);\
		Mirror::deserialize(paddingBottom, is);\
		Mirror::deserialize(paddingLeft, is);\
		Mirror::deserialize(spacing, is);\
		Mirror::deserialize(childAlignment, is);\
}
#endif