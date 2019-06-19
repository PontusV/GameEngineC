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
	Mirror::Class newClass(1);\
	newClass.name = "Core::BoxComponent";\
	newClass.annotatedAttributes = {"Reflect"};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Component" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "size";\
	newProperty.type.name = "glm::ivec2";\
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
	newProperty.name = "bounds";\
	newProperty.type.name = "Core::Bounds";\
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
virtual glm::ivec2 getValue2153734176000(std::string propertyName) {\
	if (propertyName == "size") {\
		return this->size;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
virtual Core::Bounds getValue3542546387000(std::string propertyName) {\
	if (propertyName == "bounds") {\
		return this->bounds;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "size") {\
			return Mirror::convertType<glm::ivec2, T>(getValue2153734176000(propertyName));\
		}\
		if (propertyName == "bounds") {\
			return Mirror::convertType<Core::Bounds, T>(getValue3542546387000(propertyName));\
		}\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::BoxComponent::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::BoxComponent::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "size") {\
			throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "bounds") {\
			throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " is not an array!");\
		}\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::BoxComponent::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::BoxComponent::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " does not exist!");\
}\
virtual bool setValue(std::string propertyName, glm::ivec2 value) {\
	if (propertyName == "size") {\
		this->size = value;\
		return true;\
	}\
	return false;\
}\
virtual bool setValue(std::string propertyName, Core::Bounds value) {\
	if (propertyName == "bounds") {\
		this->bounds = value;\
		return true;\
	}\
	return false;\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "size") {\
			if (setValue(propertyName, Mirror::convertType<T, glm::ivec2>(value))) return true;\
		}\
		if (propertyName == "bounds") {\
			if (setValue(propertyName, Mirror::convertType<T, Core::Bounds>(value))) return true;\
		}\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::BoxComponent::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::BoxComponent::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "size") {\
			throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "bounds") {\
			throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " is not an array!");\
		}\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::BoxComponent::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::BoxComponent::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::BoxComponent::" + propertyName + " does not exist!");\
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
		Mirror::serialize(size, os);\
		Mirror::serialize(bounds, os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(size, is);\
		Mirror::deserialize(bounds, is);\
	Core::Component::deserialize(is);\
}
#endif