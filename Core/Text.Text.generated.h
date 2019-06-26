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
	newClass.name = "Core::Text";\
	newClass.annotatedAttributes = {"Reflect"};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Sprite" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "text";\
	newProperty.type.name = "std::wstring";\
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
	newProperty.name = "font";\
	newProperty.type.name = "Core::Font";\
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
	newProperty.name = "color";\
	newProperty.type.name = "Core::Color";\
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
	newProperty.name = "layerIndex";\
	newProperty.type.name = "unsigned char";\
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
protected:\
std::wstring getValue3223175238000(std::string propertyName) {\
	if (propertyName == "text") {\
		return this->text;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
Core::Font getValue1469732627000(std::string propertyName) {\
	if (propertyName == "font") {\
		return this->font;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "text") {\
			return Mirror::convertType<std::wstring, T>(getValue3223175238000(propertyName));\
		}\
		if (propertyName == "font") {\
			return Mirror::convertType<Core::Font, T>(getValue1469732627000(propertyName));\
		}\
		if (Core::Sprite::hasProperty(propertyName))\
			return Core::Sprite::getValue_impl<T>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Text::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Text::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Text::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "text") {\
			throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "font") {\
			throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
		}\
		if (Core::Sprite::hasProperty(propertyName))\
			return Core::Sprite::getArrayValue_impl<T, N>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Text::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Text::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Text::" + propertyName + " does not exist!");\
}\
bool setValue(std::string propertyName, std::wstring value) {\
	if (propertyName == "text") {\
		this->text = value;\
		return true;\
	}\
	return false;\
}\
bool setValue(std::string propertyName, Core::Font value) {\
	if (propertyName == "font") {\
		this->font = value;\
		return true;\
	}\
	return false;\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "text") {\
			if (setValue(propertyName, Mirror::convertType<T, std::wstring>(value))) return true;\
		}\
		if (propertyName == "font") {\
			if (setValue(propertyName, Mirror::convertType<T, Core::Font>(value))) return true;\
		}\
		if (Core::Sprite::setValue_impl<T>(propertyName, value)) return true;\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Text::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Text::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Text::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "text") {\
			throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "font") {\
			throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
		}\
		if (Core::Sprite::setArrayValue_impl<T, N>(propertyName, value)) return true;\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Text::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Text::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Text::" + propertyName + " does not exist!");\
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
		Mirror::serialize(text, os);\
		Mirror::serialize(font, os);\
	Core::Sprite::serialize(os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(text, is);\
		Mirror::deserialize(font, is);\
	Core::Sprite::deserialize(is);\
	Core::Component::deserialize(is);\
}
#endif