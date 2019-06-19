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
	Mirror::Class newClass(3);\
	newClass.name = "Core::Border";\
	newClass.annotatedAttributes = {"Reflect"};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Sprite" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "top";\
	newProperty.type.name = "bool";\
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
	newProperty.name = "right";\
	newProperty.type.name = "bool";\
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
	newProperty.name = "bottom";\
	newProperty.type.name = "bool";\
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
	newProperty.name = "left";\
	newProperty.type.name = "bool";\
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
	newProperty.name = "inner";\
	newProperty.type.name = "bool";\
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
	newProperty.name = "borderThickness";\
	newProperty.type.name = "std::size_t";\
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
	newProperty.type.name = "char";\
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
virtual bool getValue3365180733000(std::string propertyName) {\
	if (propertyName == "top") {\
		return this->top;\
	}\
	if (propertyName == "right") {\
		return this->right;\
	}\
	if (propertyName == "bottom") {\
		return this->bottom;\
	}\
	if (propertyName == "left") {\
		return this->left;\
	}\
	if (propertyName == "inner") {\
		return this->inner;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
virtual std::size_t getValue3491869076000(std::string propertyName) {\
	if (propertyName == "borderThickness") {\
		return this->borderThickness;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
virtual Core::Color getValue1701103765000(std::string propertyName) {\
	if (propertyName == "color") {\
		return this->color;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "top") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "right") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "bottom") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "left") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "inner") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "borderThickness") {\
			return Mirror::convertType<std::size_t, T>(getValue3491869076000(propertyName));\
		}\
		if (propertyName == "color") {\
			return Mirror::convertType<Core::Color, T>(getValue1701103765000(propertyName));\
		}\
		if (Core::Sprite::hasProperty(propertyName))\
			return Core::Sprite::getValue_impl<T>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Border::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Border::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Border::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Border::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "top") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "right") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "bottom") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "left") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "inner") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "borderThickness") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "color") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (Core::Sprite::hasProperty(propertyName))\
			return Core::Sprite::getArrayValue_impl<T, N>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Border::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Border::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Border::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Border::" + propertyName + " does not exist!");\
}\
virtual bool setValue(std::string propertyName, bool value) {\
	if (propertyName == "top") {\
		this->top = value;\
		return true;\
	}\
	if (propertyName == "right") {\
		this->right = value;\
		return true;\
	}\
	if (propertyName == "bottom") {\
		this->bottom = value;\
		return true;\
	}\
	if (propertyName == "left") {\
		this->left = value;\
		return true;\
	}\
	if (propertyName == "inner") {\
		this->inner = value;\
		return true;\
	}\
	return false;\
}\
virtual bool setValue(std::string propertyName, std::size_t value) {\
	if (propertyName == "borderThickness") {\
		this->borderThickness = value;\
		return true;\
	}\
	return false;\
}\
virtual bool setValue(std::string propertyName, Core::Color value) {\
	if (propertyName == "color") {\
		this->color = value;\
		return true;\
	}\
	return false;\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "top") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "right") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "bottom") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "left") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "inner") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "borderThickness") {\
			if (setValue(propertyName, Mirror::convertType<T, std::size_t>(value))) return true;\
		}\
		if (propertyName == "color") {\
			if (setValue(propertyName, Mirror::convertType<T, Core::Color>(value))) return true;\
		}\
		if (Core::Sprite::setValue_impl<T>(propertyName, value)) return true;\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Border::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Border::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Border::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Border::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "top") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "right") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "bottom") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "left") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "inner") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "borderThickness") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "color") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
		}\
		if (Core::Sprite::setArrayValue_impl<T, N>(propertyName, value)) return true;\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Border::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Border::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Border::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Border::" + propertyName + " does not exist!");\
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
		Mirror::serialize(top, os);\
		Mirror::serialize(right, os);\
		Mirror::serialize(bottom, os);\
		Mirror::serialize(left, os);\
		Mirror::serialize(inner, os);\
		Mirror::serialize(borderThickness, os);\
		Mirror::serialize(color, os);\
	Core::Sprite::serialize(os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(top, is);\
		Mirror::deserialize(right, is);\
		Mirror::deserialize(bottom, is);\
		Mirror::deserialize(left, is);\
		Mirror::deserialize(inner, is);\
		Mirror::deserialize(borderThickness, is);\
		Mirror::deserialize(color, is);\
	Core::Sprite::deserialize(is);\
	Core::Component::deserialize(is);\
}
#endif