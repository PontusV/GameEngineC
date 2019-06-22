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
	Mirror::Class newClass(26);\
	newClass.name = "Core::DragAndResize";\
	newClass.annotatedAttributes = {"Reflect"};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Behaviour" });\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "edgeSize";\
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
	throw std::invalid_argument("Could not find the property!");\
}\
float getValue2797886853000(std::string propertyName) {\
	if (propertyName == "edgeSize") {\
		return this->edgeSize;\
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
		if (propertyName == "edgeSize") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getValue_impl<T>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "top") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "right") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "bottom") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "left") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "edgeSize") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayValue_impl<T, N>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " does not exist!");\
}\
bool setValue(std::string propertyName, bool value) {\
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
	return false;\
}\
bool setValue(std::string propertyName, float value) {\
	if (propertyName == "edgeSize") {\
		this->edgeSize = value;\
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
		if (propertyName == "edgeSize") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
		if (Core::Behaviour::setValue_impl<T>(propertyName, value)) return true;\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "top") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "right") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "bottom") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "left") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "edgeSize") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::setArrayValue_impl<T, N>(propertyName, value)) return true;\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " does not exist!");\
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
		Mirror::serialize(edgeSize, os);\
	Core::Behaviour::serialize(os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(top, is);\
		Mirror::deserialize(right, is);\
		Mirror::deserialize(bottom, is);\
		Mirror::deserialize(left, is);\
		Mirror::deserialize(edgeSize, is);\
	Core::Behaviour::deserialize(is);\
	Core::Component::deserialize(is);\
}
#endif