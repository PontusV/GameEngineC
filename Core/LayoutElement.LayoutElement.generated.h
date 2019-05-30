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
	newClass.name = "LayoutElement";\
	newClass.size = sizeof(LayoutElement);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "minSize";\
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
\
	newProperty = {};\
	newProperty.name = "prefSize";\
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
\
	newProperty = {};\
	newProperty.name = "flexibleSize";\
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
\
	newProperty = {};\
	newProperty.name = "minSizeEnabled";\
	newProperty.type.name = "bool";\
	newProperty.type.size = sizeof(bool);\
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
	newProperty.name = "prefSizeEnabled";\
	newProperty.type.name = "bool";\
	newProperty.type.size = sizeof(bool);\
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
	newProperty.name = "flexibleSizeEnabled";\
	newProperty.type.name = "bool";\
	newProperty.type.size = sizeof(bool);\
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
		if (propertyName == "minSize") {\
			return Mirror::convertType<int, T>(instance->minSize);\
		}\
		if (propertyName == "prefSize") {\
			return Mirror::convertType<int, T>(instance->prefSize);\
		}\
		if (propertyName == "flexibleSize") {\
			return Mirror::convertType<int, T>(instance->flexibleSize);\
		}\
		if (propertyName == "minSizeEnabled") {\
			return Mirror::convertType<bool, T>(instance->minSizeEnabled);\
		}\
		if (propertyName == "prefSizeEnabled") {\
			return Mirror::convertType<bool, T>(instance->prefSizeEnabled);\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			return Mirror::convertType<bool, T>(instance->flexibleSizeEnabled);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property LayoutElement::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property LayoutElement::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property LayoutElement::" + propertyName + " does not exist in LayoutElement!" << "\n";\
	throw std::invalid_argument("The property LayoutElement::" + propertyName + " does not exist in LayoutElement!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "minSize") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSize") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSize") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "minSizeEnabled") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSizeEnabled") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property LayoutElement::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property LayoutElement::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property LayoutElement::" + propertyName + " does not exist in LayoutElement!" << "\n";\
	throw std::invalid_argument("The property LayoutElement::" + propertyName + " does not exist in LayoutElement!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "minSize") {\
			instance->minSize = Mirror::convertType<T, int>(value);\
			return;\
		}\
		if (propertyName == "prefSize") {\
			instance->prefSize = Mirror::convertType<T, int>(value);\
			return;\
		}\
		if (propertyName == "flexibleSize") {\
			instance->flexibleSize = Mirror::convertType<T, int>(value);\
			return;\
		}\
		if (propertyName == "minSizeEnabled") {\
			instance->minSizeEnabled = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "prefSizeEnabled") {\
			instance->prefSizeEnabled = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			instance->flexibleSizeEnabled = Mirror::convertType<T, bool>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property LayoutElement::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property LayoutElement::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property LayoutElement::" + propertyName + " does not exist in LayoutElement!" << "\n";\
	throw std::invalid_argument("The property LayoutElement::" + propertyName + " does not exist in LayoutElement!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "minSize") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "prefSize") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "flexibleSize") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "minSizeEnabled") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "prefSizeEnabled") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			throw std::invalid_argument("The property LayoutElement::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property LayoutElement::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property LayoutElement::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property LayoutElement::" + propertyName + " does not exist in LayoutElement!" << "\n";\
	throw std::invalid_argument("The property LayoutElement::" + propertyName + " does not exist in LayoutElement!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(minSize, os);\
		Mirror::serialize(prefSize, os);\
		Mirror::serialize(flexibleSize, os);\
		Mirror::serialize(minSizeEnabled, os);\
		Mirror::serialize(prefSizeEnabled, os);\
		Mirror::serialize(flexibleSizeEnabled, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(minSize, is);\
		Mirror::deserialize(prefSize, is);\
		Mirror::deserialize(flexibleSize, is);\
		Mirror::deserialize(minSizeEnabled, is);\
		Mirror::deserialize(prefSizeEnabled, is);\
		Mirror::deserialize(flexibleSizeEnabled, is);\
}
#endif