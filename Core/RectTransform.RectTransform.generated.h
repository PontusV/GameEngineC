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
	Mirror::Class newClass(3);\
	newClass.name = "Core::RectTransform";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Transform" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "size";\
	newProperty.type.name = "glm::vec2";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "pivot";\
	newProperty.type.name = "glm::vec2";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "position";\
	newProperty.type.name = "glm::vec2";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PROTECTED;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "z";\
	newProperty.type.name = "float";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PROTECTED;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "rotation";\
	newProperty.type.name = "float";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PROTECTED;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "scale";\
	newProperty.type.name = "float";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PROTECTED;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
protected:\
glm::vec2 getValue226795177000(std::string propertyName) {\
	if (propertyName == "size") {\
		return this->size;\
	}\
	if (propertyName == "pivot") {\
		return this->pivot;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "size") {\
			return Mirror::convertType<glm::vec2, T>(getValue226795177000(propertyName));\
		}\
		if (propertyName == "pivot") {\
			return Mirror::convertType<glm::vec2, T>(getValue226795177000(propertyName));\
		}\
		if (Core::Transform::hasProperty(propertyName))\
			return Core::Transform::getValue_impl<T>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::RectTransform::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::RectTransform::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "size") {\
			throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "pivot") {\
			throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " is not an array!");\
		}\
		if (Core::Transform::hasProperty(propertyName))\
			return Core::Transform::getArrayValue_impl<T, N>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::RectTransform::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::RectTransform::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "size") {\
			this->size = Mirror::convertType<T, glm::vec2>(value);\
			return true;\
		}\
		if (propertyName == "pivot") {\
			this->pivot = Mirror::convertType<T, glm::vec2>(value);\
			return true;\
		}\
		if (Core::Transform::setValue_impl<T>(propertyName, value)) return true;\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::RectTransform::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::RectTransform::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "size") {\
			throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "pivot") {\
			throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " is not an array!");\
		}\
		if (Core::Transform::setArrayValue_impl<T, N>(propertyName, value)) return true;\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::RectTransform::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::RectTransform::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::RectTransform::" + propertyName + " does not exist!");\
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
		Mirror::serialize(size, os);\
		Mirror::serialize(pivot, os);\
	Core::Transform::serialize(os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(size, is);\
		Mirror::deserialize(pivot, is);\
	Core::Transform::deserialize(is);\
	Core::Component::deserialize(is);\
}
#endif