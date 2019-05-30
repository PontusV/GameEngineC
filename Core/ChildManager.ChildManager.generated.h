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
	newClass.name = "ChildManager";\
	newClass.size = sizeof(ChildManager);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "children";\
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
		if (propertyName == "children") {\
			return Mirror::convertType<int, T>(instance->children);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ChildManager::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property ChildManager::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property ChildManager::" + propertyName + " does not exist in ChildManager!" << "\n";\
	throw std::invalid_argument("The property ChildManager::" + propertyName + " does not exist in ChildManager!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "children") {\
			throw std::invalid_argument("The property ChildManager::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ChildManager::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property ChildManager::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property ChildManager::" + propertyName + " does not exist in ChildManager!" << "\n";\
	throw std::invalid_argument("The property ChildManager::" + propertyName + " does not exist in ChildManager!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "children") {\
			instance->children = Mirror::convertType<T, int>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ChildManager::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property ChildManager::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property ChildManager::" + propertyName + " does not exist in ChildManager!" << "\n";\
	throw std::invalid_argument("The property ChildManager::" + propertyName + " does not exist in ChildManager!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "children") {\
			throw std::invalid_argument("The property ChildManager::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property ChildManager::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property ChildManager::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property ChildManager::" + propertyName + " does not exist in ChildManager!" << "\n";\
	throw std::invalid_argument("The property ChildManager::" + propertyName + " does not exist in ChildManager!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(children, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(children, is);\
}
#endif