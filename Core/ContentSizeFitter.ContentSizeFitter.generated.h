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
	newClass.name = "ContentSizeFitter";\
	newClass.size = sizeof(ContentSizeFitter);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "horizontalFit";\
	newProperty.type.name = "Core::ContentSizeFitter::Mode";\
	newProperty.type.size = sizeof(Core::ContentSizeFitter::Mode);\
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
	newProperty.name = "verticalFit";\
	newProperty.type.name = "Core::ContentSizeFitter::Mode";\
	newProperty.type.size = sizeof(Core::ContentSizeFitter::Mode);\
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
		if (propertyName == "horizontalFit") {\
			return Mirror::convertType<Core::ContentSizeFitter::Mode, T>(instance->horizontalFit);\
		}\
		if (propertyName == "verticalFit") {\
			return Mirror::convertType<Core::ContentSizeFitter::Mode, T>(instance->verticalFit);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ContentSizeFitter::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property ContentSizeFitter::" + propertyName + " does not exist in ContentSizeFitter!" << "\n";\
	throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " does not exist in ContentSizeFitter!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "horizontalFit") {\
			throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "verticalFit") {\
			throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ContentSizeFitter::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property ContentSizeFitter::" + propertyName + " does not exist in ContentSizeFitter!" << "\n";\
	throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " does not exist in ContentSizeFitter!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "horizontalFit") {\
			instance->horizontalFit = Mirror::convertType<T, Core::ContentSizeFitter::Mode>(value);\
			return;\
		}\
		if (propertyName == "verticalFit") {\
			instance->verticalFit = Mirror::convertType<T, Core::ContentSizeFitter::Mode>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ContentSizeFitter::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property ContentSizeFitter::" + propertyName + " does not exist in ContentSizeFitter!" << "\n";\
	throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " does not exist in ContentSizeFitter!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "horizontalFit") {\
			throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "verticalFit") {\
			throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ContentSizeFitter::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property ContentSizeFitter::" + propertyName + " does not exist in ContentSizeFitter!" << "\n";\
	throw std::invalid_argument("The property ContentSizeFitter::" + propertyName + " does not exist in ContentSizeFitter!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(horizontalFit, os);\
		Mirror::serialize(verticalFit, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(horizontalFit, is);\
		Mirror::deserialize(verticalFit, is);\
}
#endif