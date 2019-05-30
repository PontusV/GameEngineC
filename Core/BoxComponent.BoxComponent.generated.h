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
	newClass.name = "BoxComponent";\
	newClass.size = sizeof(BoxComponent);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "size";\
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
	newProperty.name = "bounds";\
	newProperty.type.name = "Core::Bounds";\
	newProperty.type.size = sizeof(Core::Bounds);\
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
		if (propertyName == "size") {\
			return Mirror::convertType<int, T>(instance->size);\
		}\
		if (propertyName == "bounds") {\
			return Mirror::convertType<Core::Bounds, T>(instance->bounds);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property BoxComponent::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property BoxComponent::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property BoxComponent::" + propertyName + " does not exist in BoxComponent!" << "\n";\
	throw std::invalid_argument("The property BoxComponent::" + propertyName + " does not exist in BoxComponent!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "size") {\
			throw std::invalid_argument("The property BoxComponent::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "bounds") {\
			throw std::invalid_argument("The property BoxComponent::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property BoxComponent::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property BoxComponent::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property BoxComponent::" + propertyName + " does not exist in BoxComponent!" << "\n";\
	throw std::invalid_argument("The property BoxComponent::" + propertyName + " does not exist in BoxComponent!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "size") {\
			instance->size = Mirror::convertType<T, int>(value);\
			return;\
		}\
		if (propertyName == "bounds") {\
			instance->bounds = Mirror::convertType<T, Core::Bounds>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property BoxComponent::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property BoxComponent::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property BoxComponent::" + propertyName + " does not exist in BoxComponent!" << "\n";\
	throw std::invalid_argument("The property BoxComponent::" + propertyName + " does not exist in BoxComponent!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "size") {\
			throw std::invalid_argument("The property BoxComponent::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "bounds") {\
			throw std::invalid_argument("The property BoxComponent::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property BoxComponent::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property BoxComponent::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property BoxComponent::" + propertyName + " does not exist in BoxComponent!" << "\n";\
	throw std::invalid_argument("The property BoxComponent::" + propertyName + " does not exist in BoxComponent!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(size, os);\
		Mirror::serialize(bounds, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(size, is);\
		Mirror::deserialize(bounds, is);\
}
#endif