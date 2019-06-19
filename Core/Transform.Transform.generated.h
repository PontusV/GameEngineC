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
	Mirror::Class newClass(9);\
	newClass.name = "Core::Transform";\
	newClass.annotatedAttributes = {"Reflect"};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Component" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "position";\
	newProperty.type.name = "glm::vec2";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PROTECTED;\
	newProperty.annotatedAttributes = {"Reflect"};\
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
	newProperty.annotatedAttributes = {"Reflect"};\
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
	newProperty.annotatedAttributes = {"Reflect"};\
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
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
protected:\
glm::vec2 getValue226795177000(std::string propertyName) {\
	if (propertyName == "position") {\
		return this->position;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
float getValue2797886853000(std::string propertyName) {\
	if (propertyName == "z") {\
		return this->z;\
	}\
	if (propertyName == "rotation") {\
		return this->rotation;\
	}\
	if (propertyName == "scale") {\
		return this->scale;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "position") {\
			return Mirror::convertType<glm::vec2, T>(getValue226795177000(propertyName));\
		}\
		if (propertyName == "z") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
		if (propertyName == "rotation") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
		if (propertyName == "scale") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Transform::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Transform::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Transform::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Transform::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "position") {\
			throw std::invalid_argument("The property Core::Transform::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "z") {\
			throw std::invalid_argument("The property Core::Transform::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "rotation") {\
			throw std::invalid_argument("The property Core::Transform::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "scale") {\
			throw std::invalid_argument("The property Core::Transform::" + propertyName + " is not an array!");\
		}\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Transform::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Transform::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Transform::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Transform::" + propertyName + " does not exist!");\
}\
bool setValue(std::string propertyName, glm::vec2 value) {\
	if (propertyName == "position") {\
		this->position = value;\
		return true;\
	}\
	return false;\
}\
bool setValue(std::string propertyName, float value) {\
	if (propertyName == "z") {\
		this->z = value;\
		return true;\
	}\
	if (propertyName == "rotation") {\
		this->rotation = value;\
		return true;\
	}\
	if (propertyName == "scale") {\
		this->scale = value;\
		return true;\
	}\
	return false;\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "position") {\
			if (setValue(propertyName, Mirror::convertType<T, glm::vec2>(value))) return true;\
		}\
		if (propertyName == "z") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
		if (propertyName == "rotation") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
		if (propertyName == "scale") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Transform::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Transform::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Transform::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Transform::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "position") {\
			throw std::invalid_argument("The property Core::Transform::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "z") {\
			throw std::invalid_argument("The property Core::Transform::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "rotation") {\
			throw std::invalid_argument("The property Core::Transform::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "scale") {\
			throw std::invalid_argument("The property Core::Transform::" + propertyName + " is not an array!");\
		}\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Transform::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Transform::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Transform::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Transform::" + propertyName + " does not exist!");\
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
		Mirror::serialize(position, os);\
		Mirror::serialize(z, os);\
		Mirror::serialize(rotation, os);\
		Mirror::serialize(scale, os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(position, is);\
		Mirror::deserialize(z, is);\
		Mirror::deserialize(rotation, is);\
		Mirror::deserialize(scale, is);\
	Core::Component::deserialize(is);\
}
#endif