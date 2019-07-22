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
	Mirror::Class newClass(9);\
	newClass.name = "Core::CheckBox";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Behaviour" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "onToggle";\
	newProperty.type.name = "ComponentFunctionHandle<void, bool>";\
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
	newProperty.name = "toggle";\
	newProperty.type.name = "bool";\
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
ComponentFunctionHandle<void, bool> getValue1710621462000(std::string propertyName) {\
	if (propertyName == "onToggle") {\
		return this->onToggle;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
bool getValue3365180733000(std::string propertyName) {\
	if (propertyName == "toggle") {\
		return this->toggle;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "onToggle") {\
			return Mirror::convertType<ComponentFunctionHandle<void, bool>, T>(getValue1710621462000(propertyName));\
		}\
		if (propertyName == "toggle") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::CheckBox::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::CheckBox::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "onToggle") {\
			throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "toggle") {\
			throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::CheckBox::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::CheckBox::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "onToggle") {\
			this->onToggle = Mirror::convertType<T, ComponentFunctionHandle<void, bool>>(value);\
			return true;\
		}\
		if (propertyName == "toggle") {\
			this->toggle = Mirror::convertType<T, bool>(value);\
			return true;\
		}\
		if (Core::Behaviour::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::CheckBox::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::CheckBox::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "onToggle") {\
			throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "toggle") {\
			throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::CheckBox::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::CheckBox::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " does not exist!");\
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
		Mirror::serialize(onToggle, os);\
		Mirror::serialize(toggle, os);\
	Core::Behaviour::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(onToggle, is);\
		Mirror::deserialize(toggle, is);\
	Core::Behaviour::deserialize(is);\
}
#endif