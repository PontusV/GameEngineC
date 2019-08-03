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
	Mirror::Class newClass(17);\
	newClass.name = "Core::RectButton";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Behaviour" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "colors";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "Core::Color";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = true;\
	newPropertyType.arraySize = 3;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "clickFunction";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "ComponentFunctionHandle";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
	Mirror::VariableType newPropertyType_0;\
	newPropertyType_0.name = "void";\
	newPropertyType_0.isConst = false;\
	newPropertyType_0.isPointer = false;\
	newPropertyType_0.isReference = false;\
	newPropertyType_0.isEnumeration = false;\
	newPropertyType_0.isArray = false;\
	newPropertyType_0.arraySize = 0;\
	newPropertyType.templateParams.push_back(newPropertyType_0);\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
protected:\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "colors") {\
			throw std::invalid_argument("The property Core::RectButton::" + propertyName + " is an array!");\
		}\
		if (propertyName == "clickFunction") {\
			return Mirror::convertType<T>(clickFunction);\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::RectButton::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::RectButton::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::RectButton::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::RectButton::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "colors") {\
			std::vector<T> vec(3);\
			for (std::size_t i = 0; i < vec.size(); i++) {\
				vec[i] = Mirror::convertType<T>(colors[i]);\
			}\
			return vec;\
		}\
		if (propertyName == "clickFunction") {\
			throw std::invalid_argument("The property Core::RectButton::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::RectButton::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::RectButton::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::RectButton::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::RectButton::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
	try {\
		if (propertyName == "colors") {\
			if (index >= 3) throw std::out_of_range("getArrayElementValue_impl::ERROR index out of range.");\
			return Mirror::convertType<T>(colors[index]);\
		}\
		if (propertyName == "clickFunction") {\
			throw std::invalid_argument("The property Core::RectButton::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
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
			Mirror::setVariableValue(clickFunction, value);\
			return true;\
		}\
		if (Core::Behaviour::setValue_impl<T>(propertyName, value)) return true;\
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
				Mirror::setVariableValue(colors[i], value[i]);\
			}\
			return true;\
		}\
		if (propertyName == "clickFunction") {\
			throw std::invalid_argument("The property Core::RectButton::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::RectButton::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::RectButton::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::RectButton::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::RectButton::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "colors") {\
			if (index >= 3) throw std::out_of_range("setArrayElementValue_impl::ERROR index out of range.");\
			Mirror::setVariableValue(colors[index], value);\
			return true;\
		}\
		if (propertyName == "clickFunction") {\
			throw std::invalid_argument("The property Core::RectButton::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (Core::Behaviour::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
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
virtual void* getPointer(std::string propertyName) {\
	if (propertyName == "colors") {\
		return &colors;\
	}\
	if (propertyName == "clickFunction") {\
		return &clickFunction;\
	}\
	if (Core::Behaviour::hasProperty(propertyName))\
		return Core::Behaviour::getPointer(propertyName);\
	std::cout << "Warning: The property Core::RectButton::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::RectButton::" + propertyName + " does not exist or the property is const!");\
}\
virtual std::vector<void*> getArrayElementPointers(std::string propertyName) {\
	if (propertyName == "colors") {\
		std::vector<void*> vec(3);\
		for (std::size_t i = 0; i < vec.size(); i++) {\
			vec[i] = &colors[i];\
		}\
		return vec;\
	}\
	if (propertyName == "clickFunction") {\
		throw std::invalid_argument("The property Core::RectButton::" + propertyName + " is not an array!");\
	}\
	if (Core::Behaviour::hasProperty(propertyName))\
		return Core::Behaviour::getArrayElementPointers(propertyName);\
	std::cout << "Warning: The property Core::RectButton::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::RectButton::" + propertyName + " does not exist or the property is const!");\
}\
virtual void serialize(std::ostream& os) const {\
		Mirror::serialize(colors, os);\
		Mirror::serialize(clickFunction, os);\
	Core::Behaviour::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(colors, is);\
		Mirror::deserialize(clickFunction, is);\
	Core::Behaviour::deserialize(is);\
}
#endif