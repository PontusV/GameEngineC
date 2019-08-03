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
	Mirror::Class newClass(18);\
	newClass.name = "Core::DropDown";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Behaviour" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "boxWidth";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "int";\
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
	newProperty.name = "boxPaddingX";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "int";\
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
	newProperty.name = "boxPaddingY";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "int";\
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
	newProperty.name = "optionHeight";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "int";\
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
	newProperty.name = "optionFont";\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "optionTextColor";\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "border";\
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
	newProperty.name = "borderSize";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "int";\
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
	newProperty.name = "borderColor";\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "text";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "Core::Text";\
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
		if (propertyName == "boxWidth") {\
			return Mirror::convertType<T>(boxWidth);\
		}\
		if (propertyName == "boxPaddingX") {\
			return Mirror::convertType<T>(boxPaddingX);\
		}\
		if (propertyName == "boxPaddingY") {\
			return Mirror::convertType<T>(boxPaddingY);\
		}\
		if (propertyName == "optionHeight") {\
			return Mirror::convertType<T>(optionHeight);\
		}\
		if (propertyName == "optionFont") {\
			return Mirror::convertType<T>(optionFont);\
		}\
		if (propertyName == "optionTextColor") {\
			return Mirror::convertType<T>(optionTextColor);\
		}\
		if (propertyName == "border") {\
			return Mirror::convertType<T>(border);\
		}\
		if (propertyName == "borderSize") {\
			return Mirror::convertType<T>(borderSize);\
		}\
		if (propertyName == "borderColor") {\
			return Mirror::convertType<T>(borderColor);\
		}\
		if (propertyName == "text") {\
			return Mirror::convertType<T>(text);\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DropDown::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::DropDown::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DropDown::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "boxWidth") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "boxPaddingX") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "boxPaddingY") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionHeight") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionFont") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionTextColor") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "border") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "borderSize") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "borderColor") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "text") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DropDown::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::DropDown::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DropDown::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
	try {\
		if (propertyName == "boxWidth") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "boxPaddingX") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "boxPaddingY") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionHeight") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionFont") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionTextColor") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "border") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "borderSize") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "borderColor") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "text") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::DropDown::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DropDown::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "boxWidth") {\
			Mirror::setVariableValue(boxWidth, value);\
			return true;\
		}\
		if (propertyName == "boxPaddingX") {\
			Mirror::setVariableValue(boxPaddingX, value);\
			return true;\
		}\
		if (propertyName == "boxPaddingY") {\
			Mirror::setVariableValue(boxPaddingY, value);\
			return true;\
		}\
		if (propertyName == "optionHeight") {\
			Mirror::setVariableValue(optionHeight, value);\
			return true;\
		}\
		if (propertyName == "optionFont") {\
			Mirror::setVariableValue(optionFont, value);\
			return true;\
		}\
		if (propertyName == "optionTextColor") {\
			Mirror::setVariableValue(optionTextColor, value);\
			return true;\
		}\
		if (propertyName == "border") {\
			Mirror::setVariableValue(border, value);\
			return true;\
		}\
		if (propertyName == "borderSize") {\
			Mirror::setVariableValue(borderSize, value);\
			return true;\
		}\
		if (propertyName == "borderColor") {\
			Mirror::setVariableValue(borderColor, value);\
			return true;\
		}\
		if (propertyName == "text") {\
			Mirror::setVariableValue(text, value);\
			return true;\
		}\
		if (Core::Behaviour::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DropDown::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::DropDown::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DropDown::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "boxWidth") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "boxPaddingX") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "boxPaddingY") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionHeight") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionFont") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "optionTextColor") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "border") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "borderSize") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "borderColor") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "text") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DropDown::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::DropDown::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DropDown::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "boxWidth") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "boxPaddingX") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "boxPaddingY") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "optionHeight") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "optionFont") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "optionTextColor") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "border") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "borderSize") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "borderColor") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "text") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (Core::Behaviour::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
	}\
	std::cout << "Warning: The property Core::DropDown::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DropDown::" + propertyName + " does not exist!");\
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
virtual void* getPointer(std::string propertyName) {\
	if (propertyName == "boxWidth") {\
		return &boxWidth;\
	}\
	if (propertyName == "boxPaddingX") {\
		return &boxPaddingX;\
	}\
	if (propertyName == "boxPaddingY") {\
		return &boxPaddingY;\
	}\
	if (propertyName == "optionHeight") {\
		return &optionHeight;\
	}\
	if (propertyName == "optionFont") {\
		return &optionFont;\
	}\
	if (propertyName == "optionTextColor") {\
		return &optionTextColor;\
	}\
	if (propertyName == "border") {\
		return &border;\
	}\
	if (propertyName == "borderSize") {\
		return &borderSize;\
	}\
	if (propertyName == "borderColor") {\
		return &borderColor;\
	}\
	if (propertyName == "text") {\
		return &text;\
	}\
	if (Core::Behaviour::hasProperty(propertyName))\
		return Core::Behaviour::getPointer(propertyName);\
	std::cout << "Warning: The property Core::DropDown::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::DropDown::" + propertyName + " does not exist or the property is const!");\
}\
virtual std::vector<void*> getArrayElementPointers(std::string propertyName) {\
	if (propertyName == "boxWidth") {\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "boxPaddingX") {\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "boxPaddingY") {\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "optionHeight") {\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "optionFont") {\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "optionTextColor") {\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "border") {\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "borderSize") {\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "borderColor") {\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "text") {\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
	}\
	if (Core::Behaviour::hasProperty(propertyName))\
		return Core::Behaviour::getArrayElementPointers(propertyName);\
	std::cout << "Warning: The property Core::DropDown::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::DropDown::" + propertyName + " does not exist or the property is const!");\
}\
virtual void serialize(std::ostream& os) const {\
		Mirror::serialize(boxWidth, os);\
		Mirror::serialize(boxPaddingX, os);\
		Mirror::serialize(boxPaddingY, os);\
		Mirror::serialize(optionHeight, os);\
		Mirror::serialize(optionFont, os);\
		Mirror::serialize(optionTextColor, os);\
		Mirror::serialize(border, os);\
		Mirror::serialize(borderSize, os);\
		Mirror::serialize(borderColor, os);\
		Mirror::serialize(text, os);\
	Core::Behaviour::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(boxWidth, is);\
		Mirror::deserialize(boxPaddingX, is);\
		Mirror::deserialize(boxPaddingY, is);\
		Mirror::deserialize(optionHeight, is);\
		Mirror::deserialize(optionFont, is);\
		Mirror::deserialize(optionTextColor, is);\
		Mirror::deserialize(border, is);\
		Mirror::deserialize(borderSize, is);\
		Mirror::deserialize(borderColor, is);\
		Mirror::deserialize(text, is);\
	Core::Behaviour::deserialize(is);\
}
#endif