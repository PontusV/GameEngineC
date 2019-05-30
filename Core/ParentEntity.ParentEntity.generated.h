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
	newClass.name = "ParentEntity";\
	newClass.size = sizeof(ParentEntity);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "parent";\
	newProperty.type.name = "Core::Handle";\
	newProperty.type.size = sizeof(Core::Handle);\
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
		if (propertyName == "parent") {\
			return Mirror::convertType<Core::Handle, T>(instance->parent);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ParentEntity::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property ParentEntity::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property ParentEntity::" + propertyName + " does not exist in ParentEntity!" << "\n";\
	throw std::invalid_argument("The property ParentEntity::" + propertyName + " does not exist in ParentEntity!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "parent") {\
			throw std::invalid_argument("The property ParentEntity::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ParentEntity::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property ParentEntity::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property ParentEntity::" + propertyName + " does not exist in ParentEntity!" << "\n";\
	throw std::invalid_argument("The property ParentEntity::" + propertyName + " does not exist in ParentEntity!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "parent") {\
			instance->parent = Mirror::convertType<T, Core::Handle>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ParentEntity::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property ParentEntity::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property ParentEntity::" + propertyName + " does not exist in ParentEntity!" << "\n";\
	throw std::invalid_argument("The property ParentEntity::" + propertyName + " does not exist in ParentEntity!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "parent") {\
			throw std::invalid_argument("The property ParentEntity::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ParentEntity::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property ParentEntity::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property ParentEntity::" + propertyName + " does not exist in ParentEntity!" << "\n";\
	throw std::invalid_argument("The property ParentEntity::" + propertyName + " does not exist in ParentEntity!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(parent, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(parent, is);\
}
#endif