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
	Mirror::Class newClass(7);\
	newClass.name = "Core::ParentEntity";\
	newClass.annotatedAttributes = {"Reflect"};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Component" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "parent";\
	newProperty.type.name = "Core::Handle";\
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
virtual Core::Handle getValue1911159836000(std::string propertyName) {\
	if (propertyName == "parent") {\
		return this->parent;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "parent") {\
			return Mirror::convertType<Core::Handle, T>(getValue1911159836000(propertyName));\
		}\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::ParentEntity::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::ParentEntity::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::ParentEntity::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::ParentEntity::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "parent") {\
			throw std::invalid_argument("The property Core::ParentEntity::" + propertyName + " is not an array!");\
		}\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::ParentEntity::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::ParentEntity::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::ParentEntity::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::ParentEntity::" + propertyName + " does not exist!");\
}\
virtual bool setValue(std::string propertyName, Core::Handle value) {\
	if (propertyName == "parent") {\
		this->parent = value;\
		return true;\
	}\
	return false;\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "parent") {\
			if (setValue(propertyName, Mirror::convertType<T, Core::Handle>(value))) return true;\
		}\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::ParentEntity::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::ParentEntity::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::ParentEntity::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::ParentEntity::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "parent") {\
			throw std::invalid_argument("The property Core::ParentEntity::" + propertyName + " is not an array!");\
		}\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::ParentEntity::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::ParentEntity::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::ParentEntity::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::ParentEntity::" + propertyName + " does not exist!");\
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
		Mirror::serialize(parent, os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(parent, is);\
	Core::Component::deserialize(is);\
}
#endif