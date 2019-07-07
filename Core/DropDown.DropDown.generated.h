#ifndef REFLECTION_PARSER
#pragma once
#include <ReflectionParser/ReflectionParser.h>
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
	Mirror::Class newClass(19);\
	newClass.name = "Core::DropDown";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Behaviour" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "boxWidth";\
	newProperty.type.name = "int";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "boxPaddingX";\
	newProperty.type.name = "int";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "boxPaddingY";\
	newProperty.type.name = "int";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "optionHeight";\
	newProperty.type.name = "int";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "optionFont";\
	newProperty.type.name = "Core::Font";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "optionTextColor";\
	newProperty.type.name = "Core::Color";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "text";\
	newProperty.type.name = "Core::Text";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
protected:\
int getValue2515107422000(std::string propertyName) {\
	if (propertyName == "boxWidth") {\
		return this->boxWidth;\
	}\
	if (propertyName == "boxPaddingX") {\
		return this->boxPaddingX;\
	}\
	if (propertyName == "boxPaddingY") {\
		return this->boxPaddingY;\
	}\
	if (propertyName == "optionHeight") {\
		return this->optionHeight;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
Core::Font getValue1469732627000(std::string propertyName) {\
	if (propertyName == "optionFont") {\
		return this->optionFont;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
Core::Color getValue1701103765000(std::string propertyName) {\
	if (propertyName == "optionTextColor") {\
		return this->optionTextColor;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
Core::Text getValue2684722473000(std::string propertyName) {\
	if (propertyName == "text") {\
		return this->text;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "boxWidth") {\
			return Mirror::convertType<int, T>(getValue2515107422000(propertyName));\
		}\
		if (propertyName == "boxPaddingX") {\
			return Mirror::convertType<int, T>(getValue2515107422000(propertyName));\
		}\
		if (propertyName == "boxPaddingY") {\
			return Mirror::convertType<int, T>(getValue2515107422000(propertyName));\
		}\
		if (propertyName == "optionHeight") {\
			return Mirror::convertType<int, T>(getValue2515107422000(propertyName));\
		}\
		if (propertyName == "optionFont") {\
			return Mirror::convertType<Core::Font, T>(getValue1469732627000(propertyName));\
		}\
		if (propertyName == "optionTextColor") {\
			return Mirror::convertType<Core::Color, T>(getValue1701103765000(propertyName));\
		}\
		if (propertyName == "text") {\
			return Mirror::convertType<Core::Text, T>(getValue2684722473000(propertyName));\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getValue_impl<T>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DropDown::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::DropDown::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DropDown::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
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
		if (propertyName == "text") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayValue_impl<T, N>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DropDown::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::DropDown::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DropDown::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "boxWidth") {\
			this->boxWidth = Mirror::convertType<T, int>(value);\
			return true;\
		}\
		if (propertyName == "boxPaddingX") {\
			this->boxPaddingX = Mirror::convertType<T, int>(value);\
			return true;\
		}\
		if (propertyName == "boxPaddingY") {\
			this->boxPaddingY = Mirror::convertType<T, int>(value);\
			return true;\
		}\
		if (propertyName == "optionHeight") {\
			this->optionHeight = Mirror::convertType<T, int>(value);\
			return true;\
		}\
		if (propertyName == "optionFont") {\
			this->optionFont = Mirror::convertType<T, Core::Font>(value);\
			return true;\
		}\
		if (propertyName == "optionTextColor") {\
			this->optionTextColor = Mirror::convertType<T, Core::Color>(value);\
			return true;\
		}\
		if (propertyName == "text") {\
			this->text = Mirror::convertType<T, Core::Text>(value);\
			return true;\
		}\
		if (Core::Behaviour::setValue_impl<T>(propertyName, value)) return true;\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
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
		if (propertyName == "text") {\
			throw std::invalid_argument("The property Core::DropDown::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::setArrayValue_impl<T, N>(propertyName, value)) return true;\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DropDown::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::DropDown::" + propertyName + " was set to a value with an incompatible type!");\
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
virtual void serialize(std::ostream& os) const {\
		Mirror::serialize(boxWidth, os);\
		Mirror::serialize(boxPaddingX, os);\
		Mirror::serialize(boxPaddingY, os);\
		Mirror::serialize(optionHeight, os);\
		Mirror::serialize(optionFont, os);\
		Mirror::serialize(optionTextColor, os);\
		Mirror::serialize(text, os);\
	Core::Behaviour::serialize(os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(boxWidth, is);\
		Mirror::deserialize(boxPaddingX, is);\
		Mirror::deserialize(boxPaddingY, is);\
		Mirror::deserialize(optionHeight, is);\
		Mirror::deserialize(optionFont, is);\
		Mirror::deserialize(optionTextColor, is);\
		Mirror::deserialize(text, is);\
	Core::Behaviour::deserialize(is);\
	Core::Component::deserialize(is);\
}
#endif