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
	Mirror::Class newClass(18);\
	newClass.name = "Core::RectButton";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Behaviour" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "colors";\
	newProperty.type.name = "Core::Color";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = true;\
	newProperty.type.arraySize = 3;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "clickFunction";\
	newProperty.type.name = "ComponentFunctionHandle<>";\
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
Core::Color* getValue1701103765001(std::string propertyName) {\
	if (propertyName == "colors") {\
		return this->colors;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
ComponentFunctionHandle<> getValue4233867720000(std::string propertyName) {\
	if (propertyName == "clickFunction") {\
		return this->clickFunction;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "colors") {\
			throw std::invalid_argument("The property Core::RectButton::" + propertyName + " is an array!");\
		}\
		if (propertyName == "clickFunction") {\
			return Mirror::convertType<ComponentFunctionHandle<>, T>(getValue4233867720000(propertyName));\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getValue_impl<T>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::RectButton::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::RectButton::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::RectButton::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::RectButton::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "colors") {\
			return Mirror::convertArrayType<Core::Color, 3, T, N>(getValue1701103765001(propertyName));\
		}\
		if (propertyName == "clickFunction") {\
			throw std::invalid_argument("The property Core::RectButton::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayValue_impl<T, N>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::RectButton::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::RectButton::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::RectButton::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::RectButton::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "colors") {\
			throw std::invalid_argument("The property Core::RectButton::" + propertyName + " is an array!");\
		}\
		if (propertyName == "clickFunction") {\
			this->clickFunction = Mirror::convertType<T, ComponentFunctionHandle<>>(value);\
			return true;\
		}\
		if (Core::Behaviour::setValue_impl<T>(propertyName, value)) return true;\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::RectButton::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::RectButton::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::RectButton::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::RectButton::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "colors") {\
			for (std::size_t i = 0; i < 3; i++) {\
				this->colors[i] = Mirror::convertType<T, Core::Color>(value[i]);\
			}\
			return true;\
		}\
		if (propertyName == "clickFunction") {\
			throw std::invalid_argument("The property Core::RectButton::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::setArrayValue_impl<T, N>(propertyName, value)) return true;\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::RectButton::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::RectButton::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::RectButton::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::RectButton::" + propertyName + " does not exist!");\
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
		Mirror::serialize(colors, os);\
		Mirror::serialize(clickFunction, os);\
	Core::Behaviour::serialize(os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(colors, is);\
		Mirror::deserialize(clickFunction, is);\
	Core::Behaviour::deserialize(is);\
	Core::Component::deserialize(is);\
}
#endif