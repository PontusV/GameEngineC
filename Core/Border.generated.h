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
	Mirror::Class newClass(8);\
	newClass.name = "Core::Border";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Sprite" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "top";\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "right";\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "bottom";\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "left";\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "inner";\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "borderThickness";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "std::size_t";\
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
		if (propertyName == "top") {\
			return Mirror::convertType<T>(top);\
		}\
		if (propertyName == "right") {\
			return Mirror::convertType<T>(right);\
		}\
		if (propertyName == "bottom") {\
			return Mirror::convertType<T>(bottom);\
		}\
		if (propertyName == "left") {\
			return Mirror::convertType<T>(left);\
		}\
		if (propertyName == "inner") {\
			return Mirror::convertType<T>(inner);\
		}\
		if (propertyName == "borderThickness") {\
			return Mirror::convertType<T>(borderThickness);\
		}\
		if (propertyName == "color") {\
			return Mirror::convertType<T>(color);\
		}\
		if (Core::Sprite::hasProperty(propertyName))\
			return Core::Sprite::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Border::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Border::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Border::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Border::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
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
			return Core::Sprite::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Border::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Border::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Border::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Border::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
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
			return Core::Sprite::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Core::Border::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Border::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Border::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "top") {\
			Mirror::setVariableValue(top, value);\
			return true;\
		}\
		if (propertyName == "right") {\
			Mirror::setVariableValue(right, value);\
			return true;\
		}\
		if (propertyName == "bottom") {\
			Mirror::setVariableValue(bottom, value);\
			return true;\
		}\
		if (propertyName == "left") {\
			Mirror::setVariableValue(left, value);\
			return true;\
		}\
		if (propertyName == "inner") {\
			Mirror::setVariableValue(inner, value);\
			return true;\
		}\
		if (propertyName == "borderThickness") {\
			Mirror::setVariableValue(borderThickness, value);\
			return true;\
		}\
		if (propertyName == "color") {\
			Mirror::setVariableValue(color, value);\
			return true;\
		}\
		if (Core::Sprite::setValue_impl<T>(propertyName, value)) return true;\
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
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Border::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Border::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Border::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Border::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "top") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "right") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "bottom") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "left") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "inner") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "borderThickness") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "color") {\
			throw std::invalid_argument("The property Core::Border::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (Core::Sprite::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
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
static constexpr std::size_t getTypeID() {\
	return 8;\
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
		return Core::Sprite::getArraySize(propertyName);\
	std::cout << "Warning: The property Core::Border::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Border::" + propertyName + " does not exist or the property is const!");\
}\
virtual void* getPointer(std::string propertyName) {\
	if (propertyName == "top") {\
		return &top;\
	}\
	if (propertyName == "right") {\
		return &right;\
	}\
	if (propertyName == "bottom") {\
		return &bottom;\
	}\
	if (propertyName == "left") {\
		return &left;\
	}\
	if (propertyName == "inner") {\
		return &inner;\
	}\
	if (propertyName == "borderThickness") {\
		return &borderThickness;\
	}\
	if (propertyName == "color") {\
		return &color;\
	}\
	if (Core::Sprite::hasProperty(propertyName))\
		return Core::Sprite::getPointer(propertyName);\
	std::cout << "Warning: The property Core::Border::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Border::" + propertyName + " does not exist or the property is const!");\
}\
virtual std::vector<void*> getArrayElementPointers(std::string propertyName) {\
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
		return Core::Sprite::getArrayElementPointers(propertyName);\
	std::cout << "Warning: The property Core::Border::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Border::" + propertyName + " does not exist or the property is const!");\
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
}
#endif