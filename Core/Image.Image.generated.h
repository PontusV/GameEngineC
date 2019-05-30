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
	newClass.name = "Image";\
	newClass.size = sizeof(Image);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "fileName";\
	newProperty.type.name = "std::string";\
	newProperty.type.size = sizeof(std::string);\
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
	newProperty.name = "texture";\
	newProperty.type.name = "Core::Texture2D";\
	newProperty.type.size = sizeof(Core::Texture2D);\
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
		if (propertyName == "fileName") {\
			return Mirror::convertType<std::string, T>(instance->fileName);\
		}\
		if (propertyName == "texture") {\
			return Mirror::convertType<Core::Texture2D, T>(instance->texture);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Image::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Image::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property Image::" + propertyName + " does not exist in Image!" << "\n";\
	throw std::invalid_argument("The property Image::" + propertyName + " does not exist in Image!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "fileName") {\
			throw std::invalid_argument("The property Image::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "texture") {\
			throw std::invalid_argument("The property Image::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Image::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Image::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property Image::" + propertyName + " does not exist in Image!" << "\n";\
	throw std::invalid_argument("The property Image::" + propertyName + " does not exist in Image!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "fileName") {\
			instance->fileName = Mirror::convertType<T, std::string>(value);\
			return;\
		}\
		if (propertyName == "texture") {\
			instance->texture = Mirror::convertType<T, Core::Texture2D>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Image::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Image::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property Image::" + propertyName + " does not exist in Image!" << "\n";\
	throw std::invalid_argument("The property Image::" + propertyName + " does not exist in Image!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "fileName") {\
			throw std::invalid_argument("The property Image::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "texture") {\
			throw std::invalid_argument("The property Image::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Image::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Image::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property Image::" + propertyName + " does not exist in Image!" << "\n";\
	throw std::invalid_argument("The property Image::" + propertyName + " does not exist in Image!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(fileName, os);\
		Mirror::serialize(texture, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(fileName, is);\
		Mirror::deserialize(texture, is);\
}
#endif