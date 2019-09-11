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
	Mirror::Class newClass(11);\
	newClass.name = "Core::Button";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Behaviour" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "images";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "Core::Image";\
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
	return newClass;\
}\
protected:\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "images") {\
			throw std::invalid_argument("The property Core::Button::" + propertyName + " is an array!");\
		}\
		if (propertyName == "colors") {\
			throw std::invalid_argument("The property Core::Button::" + propertyName + " is an array!");\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Button::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Button::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Button::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Button::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "images") {\
			std::vector<T> vec(3);\
			for (std::size_t i = 0; i < vec.size(); i++) {\
				vec[i] = Mirror::convertType<T>(images[i]);\
			}\
			return vec;\
		}\
		if (propertyName == "colors") {\
			std::vector<T> vec(3);\
			for (std::size_t i = 0; i < vec.size(); i++) {\
				vec[i] = Mirror::convertType<T>(colors[i]);\
			}\
			return vec;\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Button::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Button::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Button::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Button::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
	try {\
		if (propertyName == "images") {\
			if (index >= 3) throw std::out_of_range("getArrayElementValue_impl::ERROR index out of range.");\
			return Mirror::convertType<T>(images[index]);\
		}\
		if (propertyName == "colors") {\
			if (index >= 3) throw std::out_of_range("getArrayElementValue_impl::ERROR index out of range.");\
			return Mirror::convertType<T>(colors[index]);\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Core::Button::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Button::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Button::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "images") {\
			throw std::invalid_argument("The property Core::Button::" + propertyName + " is an array!");\
		}\
		if (propertyName == "colors") {\
			throw std::invalid_argument("The property Core::Button::" + propertyName + " is an array!");\
		}\
		if (Core::Behaviour::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Button::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Button::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Button::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Button::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "images") {\
			for (std::size_t i = 0; i < 3; i++) {\
				Mirror::setVariableValue(images[i], value[i]);\
			}\
			return true;\
		}\
		if (propertyName == "colors") {\
			for (std::size_t i = 0; i < 3; i++) {\
				Mirror::setVariableValue(colors[i], value[i]);\
			}\
			return true;\
		}\
		if (Core::Behaviour::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Button::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Button::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Button::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Button::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "images") {\
			if (index >= 3) throw std::out_of_range("setArrayElementValue_impl::ERROR index out of range.");\
			Mirror::setVariableValue(images[index], value);\
			return true;\
		}\
		if (propertyName == "colors") {\
			if (index >= 3) throw std::out_of_range("setArrayElementValue_impl::ERROR index out of range.");\
			Mirror::setVariableValue(colors[index], value);\
			return true;\
		}\
		if (Core::Behaviour::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
	}\
	std::cout << "Warning: The property Core::Button::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Button::" + propertyName + " does not exist!");\
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
	return 11;\
}\
static std::vector<std::size_t> getBaseTypeIDs() {\
	return std::vector<std::size_t>{Behaviour::getTypeID(),Component::getTypeID(),ReflectedObject::getTypeID()};\
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
	if (propertyName == "images") {\
		return 3;\
	}\
	if (propertyName == "colors") {\
		return 3;\
	}\
	if (Core::Behaviour::hasProperty(propertyName))\
		return Core::Behaviour::getArraySize(propertyName);\
	std::cout << "Warning: The property Core::Button::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Button::" + propertyName + " does not exist or the property is const!");\
}\
virtual void* getPointer(std::string propertyName) {\
	if (propertyName == "images") {\
		return &images;\
	}\
	if (propertyName == "colors") {\
		return &colors;\
	}\
	if (Core::Behaviour::hasProperty(propertyName))\
		return Core::Behaviour::getPointer(propertyName);\
	std::cout << "Warning: The property Core::Button::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Button::" + propertyName + " does not exist or the property is const!");\
}\
virtual std::vector<void*> getArrayElementPointers(std::string propertyName) {\
	if (propertyName == "images") {\
		std::vector<void*> vec(3);\
		for (std::size_t i = 0; i < vec.size(); i++) {\
			vec[i] = &images[i];\
		}\
		return vec;\
	}\
	if (propertyName == "colors") {\
		std::vector<void*> vec(3);\
		for (std::size_t i = 0; i < vec.size(); i++) {\
			vec[i] = &colors[i];\
		}\
		return vec;\
	}\
	if (Core::Behaviour::hasProperty(propertyName))\
		return Core::Behaviour::getArrayElementPointers(propertyName);\
	std::cout << "Warning: The property Core::Button::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Button::" + propertyName + " does not exist or the property is const!");\
}\
virtual void serialize(std::ostream& os) const {\
		Mirror::serialize(images, os);\
		Mirror::serialize(colors, os);\
	Core::Behaviour::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(images, is);\
		Mirror::deserialize(colors, is);\
	Core::Behaviour::deserialize(is);\
}
#endif