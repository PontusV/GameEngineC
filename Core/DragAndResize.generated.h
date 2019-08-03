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
	Mirror::Class newClass(27);\
	newClass.name = "Core::DragAndResize";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Behaviour" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "top";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "bool";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "right";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "bool";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "bottom";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "bool";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "left";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "bool";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "edgeSize";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "float";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
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
		if (propertyName == "top") {\
			return Mirror::convertType<T>(top);\
		}\
		if (propertyName == "right") {\
			return Mirror::convertType<T>(right);\
		}\
		if (propertyName == "bottom") {\
			return Mirror::convertType<T>(bottom);\
		}\
		if (propertyName == "left") {\
			return Mirror::convertType<T>(left);\
		}\
		if (propertyName == "edgeSize") {\
			return Mirror::convertType<T>(edgeSize);\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
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
			return Core::Behaviour::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
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
			return Core::Behaviour::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "top") {\
			Mirror::setVariableValue(top, value);\
			return true;\
		}\
		if (propertyName == "right") {\
			Mirror::setVariableValue(right, value);\
			return true;\
		}\
		if (propertyName == "bottom") {\
			Mirror::setVariableValue(bottom, value);\
			return true;\
		}\
		if (propertyName == "left") {\
			Mirror::setVariableValue(left, value);\
			return true;\
		}\
		if (propertyName == "edgeSize") {\
			Mirror::setVariableValue(edgeSize, value);\
			return true;\
		}\
		if (Core::Behaviour::setValue_impl<T>(propertyName, value)) return true;\
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
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "top") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "right") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "bottom") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "left") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "edgeSize") {\
			throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (Core::Behaviour::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
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
static Mirror::Class getClassType() {\
	static Mirror::Class type = getTypeImpl();\
	return type;\
}\
public:\
virtual Mirror::Class getType() {\
	return getClassType();\
}\
virtual void* getPointer(std::string propertyName) {\
	if (propertyName == "top") {\
		return &top;\
	}\
	if (propertyName == "right") {\
		return &right;\
	}\
	if (propertyName == "bottom") {\
		return &bottom;\
	}\
	if (propertyName == "left") {\
		return &left;\
	}\
	if (propertyName == "edgeSize") {\
		return &edgeSize;\
	}\
	if (Core::Behaviour::hasProperty(propertyName))\
		return Core::Behaviour::getPointer(propertyName);\
	std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " does not exist or the property is const!");\
}\
virtual std::vector<void*> getArrayElementPointers(std::string propertyName) {\
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
		return Core::Behaviour::getArrayElementPointers(propertyName);\
	std::cout << "Warning: The property Core::DragAndResize::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::DragAndResize::" + propertyName + " does not exist or the property is const!");\
}\
virtual void serialize(std::ostream& os) const {\
		Mirror::serialize(top, os);\
		Mirror::serialize(right, os);\
		Mirror::serialize(bottom, os);\
		Mirror::serialize(left, os);\
		Mirror::serialize(edgeSize, os);\
	Core::Behaviour::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(top, is);\
		Mirror::deserialize(right, is);\
		Mirror::deserialize(bottom, is);\
		Mirror::deserialize(left, is);\
		Mirror::deserialize(edgeSize, is);\
	Core::Behaviour::deserialize(is);\
}
#endif