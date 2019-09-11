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
	Mirror::Class newClass(19);\
	newClass.name = "Core::Text";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Sprite" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "text";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "std::wstring";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}, Mirror::Annotation{"Update", {"refresh()"}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "font";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "Core::Font";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "color";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "Core::Color";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
protected:\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "text") {\
			return Mirror::convertType<T>(text);\
		}\
		if (propertyName == "font") {\
			return Mirror::convertType<T>(font);\
		}\
		if (Core::Sprite::hasProperty(propertyName))\
			return Core::Sprite::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Text::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Text::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Text::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "text") {\
			throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "font") {\
			throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
		}\
		if (Core::Sprite::hasProperty(propertyName))\
			return Core::Sprite::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Text::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Text::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Text::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
	try {\
		if (propertyName == "text") {\
			throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "font") {\
			throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
		}\
		if (Core::Sprite::hasProperty(propertyName))\
			return Core::Sprite::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Text::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Text::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "text") {\
			Mirror::setVariableValue(text, value);\
			refresh();\
			return true;\
		}\
		if (propertyName == "font") {\
			Mirror::setVariableValue(font, value);\
			return true;\
		}\
		if (Core::Sprite::setValue_impl<T>(propertyName, value)) return true;\
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
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Text::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Text::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Text::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "text") {\
			throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
			refresh();\
			return true;\
		}\
		if (propertyName == "font") {\
			throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (Core::Sprite::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
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
static constexpr std::size_t getTypeID() {\
	return 19;\
}\
static std::vector<std::size_t> getBaseTypeIDs() {\
	return std::vector<std::size_t>{Sprite::getTypeID(),Component::getTypeID(),ReflectedObject::getTypeID()};\
}\
static Mirror::Class getClassType() {\
	static Mirror::Class type = getTypeImpl();\
	return type;\
}\
public:\
virtual Mirror::Class getType() {\
	return getClassType();\
}\
virtual std::size_t getArraySize(std::string propertyName) {\
	if (propertyName == "text") {\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "font") {\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
	}\
	if (Core::Sprite::hasProperty(propertyName))\
		return Core::Sprite::getArraySize(propertyName);\
	std::cout << "Warning: The property Core::Text::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Text::" + propertyName + " does not exist or the property is const!");\
}\
virtual void* getPointer(std::string propertyName) {\
	if (propertyName == "text") {\
		return &text;\
	}\
	if (propertyName == "font") {\
		return &font;\
	}\
	if (Core::Sprite::hasProperty(propertyName))\
		return Core::Sprite::getPointer(propertyName);\
	std::cout << "Warning: The property Core::Text::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Text::" + propertyName + " does not exist or the property is const!");\
}\
virtual std::vector<void*> getArrayElementPointers(std::string propertyName) {\
	if (propertyName == "text") {\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "font") {\
		throw std::invalid_argument("The property Core::Text::" + propertyName + " is not an array!");\
	}\
	if (Core::Sprite::hasProperty(propertyName))\
		return Core::Sprite::getArrayElementPointers(propertyName);\
	std::cout << "Warning: The property Core::Text::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Text::" + propertyName + " does not exist or the property is const!");\
}\
virtual void serialize(std::ostream& os) const {\
		Mirror::serialize(text, os);\
		Mirror::serialize(font, os);\
	Core::Sprite::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(text, is);\
		Mirror::deserialize(font, is);\
	Core::Sprite::deserialize(is);\
}
#endif