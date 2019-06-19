#ifndef REFLECTION_PARSER
#pragma once
#include <ReflectionParser.h>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <tuple>

#ifdef GENERATED_BODY
#undef GENERATED_BODY
#endif
#define GENERATED_BODY(...)\
private:\
static Mirror::Class createType() {\
	Mirror::Class newClass(16);\
	newClass.name = "Core::WindowScale";\
	newClass.annotatedAttributes = {"Reflect"};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::UIBehaviour" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "stretchWidth";\
	newProperty.type.name = "bool";\
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
protected:\
bool getValue3365180733000(std::string propertyName) {\
	if (propertyName == "stretchWidth") {\
		return this->stretchWidth;\
	}\
	if (propertyName == "stretchHeight") {\
		return this->stretchHeight;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
float getValue2797886853000(std::string propertyName) {\
	if (propertyName == "widthScale") {\
		return this->widthScale;\
	}\
	if (propertyName == "heightScale") {\
		return this->heightScale;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "stretchWidth") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "stretchHeight") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "widthScale") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
		if (propertyName == "heightScale") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::WindowScale::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::WindowScale::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "stretchWidth") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "stretchHeight") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "widthScale") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "heightScale") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::WindowScale::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::WindowScale::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " does not exist!");\
}\
bool setValue(std::string propertyName, bool value) {\
	if (propertyName == "stretchWidth") {\
		this->stretchWidth = value;\
		return true;\
	}\
	if (propertyName == "stretchHeight") {\
		this->stretchHeight = value;\
		return true;\
	}\
	return false;\
}\
bool setValue(std::string propertyName, float value) {\
	if (propertyName == "widthScale") {\
		this->widthScale = value;\
		return true;\
	}\
	if (propertyName == "heightScale") {\
		this->heightScale = value;\
		return true;\
	}\
	return false;\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "stretchWidth") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "stretchHeight") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "widthScale") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
		if (propertyName == "heightScale") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::WindowScale::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::WindowScale::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "stretchWidth") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "stretchHeight") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "widthScale") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "heightScale") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::WindowScale::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::WindowScale::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " does not exist!");\
}\
template<typename... Ts>\
bool invoke_impl(std::string functionName, Ts... args) { return false; }\
friend Mirror::Function;\
friend Mirror::Property;\
static Mirror::Class getTypeImpl() {\
	static Mirror::Class type = createType();\
	return type;\
}\
static bool hasProperty(std::string propertyName) {\
	Mirror::Class type = getTypeImpl();\
	for (const Mirror::Property& prop : type.properties)\
		if (prop.name == propertyName) return true;\
	return false;\
}\
public:\
virtual Mirror::Class getType() {\
	static Mirror::Class type = getTypeImpl();\
	return type;\
}\
virtual void serialize(std::ostream& os) const {\
		Mirror::serialize(stretchWidth, os);\
		Mirror::serialize(stretchHeight, os);\
		Mirror::serialize(widthScale, os);\
		Mirror::serialize(heightScale, os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(stretchWidth, is);\
		Mirror::deserialize(stretchHeight, is);\
		Mirror::deserialize(widthScale, is);\
		Mirror::deserialize(heightScale, is);\
}
#endif