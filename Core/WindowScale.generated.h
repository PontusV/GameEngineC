#ifndef REFLECTION_PARSER
#pragma once
#include <ReflectionParser/ReflectionParser.h>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <tuple>
#include <vector>

#ifdef GENERATED_BODY
#undef GENERATED_BODY
#endif
#define GENERATED_BODY(...)\
private:\
static Mirror::Class createType() {\
	Mirror::Class newClass(20);\
	newClass.name = "Core::WindowScale";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::UIBehaviour" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "stretchWidth";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "bool";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "stretchHeight";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "bool";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "widthScale";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "float";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "heightScale";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "float";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
protected:\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "stretchWidth") {\
			return Mirror::convertType<T>(stretchWidth);\
		}\
		if (propertyName == "stretchHeight") {\
			return Mirror::convertType<T>(stretchHeight);\
		}\
		if (propertyName == "widthScale") {\
			return Mirror::convertType<T>(widthScale);\
		}\
		if (propertyName == "heightScale") {\
			return Mirror::convertType<T>(heightScale);\
		}\
		if (Core::UIBehaviour::hasProperty(propertyName))\
			return Core::UIBehaviour::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::WindowScale::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::WindowScale::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
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
		if (Core::UIBehaviour::hasProperty(propertyName))\
			return Core::UIBehaviour::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::WindowScale::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::WindowScale::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
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
		if (Core::UIBehaviour::hasProperty(propertyName))\
			return Core::UIBehaviour::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::WindowScale::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "stretchWidth") {\
			Mirror::setVariableValue(stretchWidth, value);\
			return true;\
		}\
		if (propertyName == "stretchHeight") {\
			Mirror::setVariableValue(stretchHeight, value);\
			return true;\
		}\
		if (propertyName == "widthScale") {\
			Mirror::setVariableValue(widthScale, value);\
			return true;\
		}\
		if (propertyName == "heightScale") {\
			Mirror::setVariableValue(heightScale, value);\
			return true;\
		}\
		if (Core::UIBehaviour::setValue_impl<T>(propertyName, value)) return true;\
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
		if (Core::UIBehaviour::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::WindowScale::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::WindowScale::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "stretchWidth") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "stretchHeight") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "widthScale") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "heightScale") {\
			throw std::invalid_argument("The property Core::WindowScale::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (Core::UIBehaviour::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
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
static Mirror::Class getClassType() {\
	static Mirror::Class type = getTypeImpl();\
	return type;\
}\
public:\
virtual Mirror::Class getType() {\
	return getClassType();\
}\
virtual void serialize(std::ostream& os) const {\
		Mirror::serialize(stretchWidth, os);\
		Mirror::serialize(stretchHeight, os);\
		Mirror::serialize(widthScale, os);\
		Mirror::serialize(heightScale, os);\
	Core::UIBehaviour::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(stretchWidth, is);\
		Mirror::deserialize(stretchHeight, is);\
		Mirror::deserialize(widthScale, is);\
		Mirror::deserialize(heightScale, is);\
	Core::UIBehaviour::deserialize(is);\
}
#endif