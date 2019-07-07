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
	newClass.name = "Core::WindowAnchor";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::UIBehaviour" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "anchor";\
	newProperty.type.name = "glm::vec2";\
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
	newProperty.name = "offset";\
	newProperty.type.name = "glm::vec2";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
protected:\
glm::vec2 getValue226795177000(std::string propertyName) {\
	if (propertyName == "anchor") {\
		return this->anchor;\
	}\
	if (propertyName == "offset") {\
		return this->offset;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "anchor") {\
			return Mirror::convertType<glm::vec2, T>(getValue226795177000(propertyName));\
		}\
		if (propertyName == "offset") {\
			return Mirror::convertType<glm::vec2, T>(getValue226795177000(propertyName));\
		}\
		if (Core::UIBehaviour::hasProperty(propertyName))\
			return Core::UIBehaviour::getValue_impl<T>(propertyName);\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getValue_impl<T>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::WindowAnchor::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::WindowAnchor::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "anchor") {\
			throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "offset") {\
			throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " is not an array!");\
		}\
		if (Core::UIBehaviour::hasProperty(propertyName))\
			return Core::UIBehaviour::getArrayValue_impl<T, N>(propertyName);\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayValue_impl<T, N>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::WindowAnchor::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::WindowAnchor::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "anchor") {\
			this->anchor = Mirror::convertType<T, glm::vec2>(value);\
			return true;\
		}\
		if (propertyName == "offset") {\
			this->offset = Mirror::convertType<T, glm::vec2>(value);\
			return true;\
		}\
		if (Core::UIBehaviour::setValue_impl<T>(propertyName, value)) return true;\
		if (Core::Behaviour::setValue_impl<T>(propertyName, value)) return true;\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::WindowAnchor::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::WindowAnchor::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "anchor") {\
			throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "offset") {\
			throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " is not an array!");\
		}\
		if (Core::UIBehaviour::setArrayValue_impl<T, N>(propertyName, value)) return true;\
		if (Core::Behaviour::setArrayValue_impl<T, N>(propertyName, value)) return true;\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::WindowAnchor::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::WindowAnchor::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::WindowAnchor::" + propertyName + " does not exist!");\
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
		Mirror::serialize(anchor, os);\
		Mirror::serialize(offset, os);\
	Core::UIBehaviour::serialize(os);\
	Core::Behaviour::serialize(os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(anchor, is);\
		Mirror::deserialize(offset, is);\
	Core::UIBehaviour::deserialize(is);\
	Core::Behaviour::deserialize(is);\
	Core::Component::deserialize(is);\
}
#endif