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
	Mirror::Class newClass(24);\
	newClass.name = "Core::HorizontalLayoutGroup";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::LayoutGroup" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "shrinkableChildWidth";\
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
	newProperty.name = "shrinkableChildHeight";\
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
	newProperty.name = "childForceExpandWidth";\
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
	newProperty.name = "childForceExpandHeight";\
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
	newProperty.name = "paddingTop";\
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
\
	newProperty = {};\
	newProperty.name = "paddingRight";\
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
\
	newProperty = {};\
	newProperty.name = "paddingBottom";\
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
\
	newProperty = {};\
	newProperty.name = "paddingLeft";\
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
\
	newProperty = {};\
	newProperty.name = "spacing";\
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
\
	newProperty = {};\
	newProperty.name = "childAlignment";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "Core::Alignment";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = true;\
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
		if (propertyName == "shrinkableChildWidth") {\
			return Mirror::convertType<T>(shrinkableChildWidth);\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			return Mirror::convertType<T>(shrinkableChildHeight);\
		}\
		if (propertyName == "childForceExpandWidth") {\
			return Mirror::convertType<T>(childForceExpandWidth);\
		}\
		if (propertyName == "childForceExpandHeight") {\
			return Mirror::convertType<T>(childForceExpandHeight);\
		}\
		if (Core::LayoutGroup::hasProperty(propertyName))\
			return Core::LayoutGroup::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "shrinkableChildWidth") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandWidth") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandHeight") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (Core::LayoutGroup::hasProperty(propertyName))\
			return Core::LayoutGroup::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
	try {\
		if (propertyName == "shrinkableChildWidth") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandWidth") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandHeight") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (Core::LayoutGroup::hasProperty(propertyName))\
			return Core::LayoutGroup::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "shrinkableChildWidth") {\
			Mirror::setVariableValue(shrinkableChildWidth, value);\
			return true;\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			Mirror::setVariableValue(shrinkableChildHeight, value);\
			return true;\
		}\
		if (propertyName == "childForceExpandWidth") {\
			Mirror::setVariableValue(childForceExpandWidth, value);\
			return true;\
		}\
		if (propertyName == "childForceExpandHeight") {\
			Mirror::setVariableValue(childForceExpandHeight, value);\
			return true;\
		}\
		if (Core::LayoutGroup::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "shrinkableChildWidth") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandWidth") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandHeight") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (Core::LayoutGroup::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "shrinkableChildWidth") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "childForceExpandWidth") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "childForceExpandHeight") {\
			throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (Core::LayoutGroup::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
	}\
	std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist!");\
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
	return 24;\
}\
static std::vector<std::size_t> getBaseTypeIDs() {\
	return std::vector<std::size_t>{LayoutGroup::getTypeID(),LayoutController::getTypeID(),UIBehaviour::getTypeID(),Behaviour::getTypeID(),Component::getTypeID(),ReflectedObject::getTypeID()};\
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
	if (propertyName == "shrinkableChildWidth") {\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "shrinkableChildHeight") {\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "childForceExpandWidth") {\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "childForceExpandHeight") {\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
	}\
	if (Core::LayoutGroup::hasProperty(propertyName))\
		return Core::LayoutGroup::getArraySize(propertyName);\
	std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist or the property is const!");\
}\
virtual void* getPointer(std::string propertyName) {\
	if (propertyName == "shrinkableChildWidth") {\
		return &shrinkableChildWidth;\
	}\
	if (propertyName == "shrinkableChildHeight") {\
		return &shrinkableChildHeight;\
	}\
	if (propertyName == "childForceExpandWidth") {\
		return &childForceExpandWidth;\
	}\
	if (propertyName == "childForceExpandHeight") {\
		return &childForceExpandHeight;\
	}\
	if (Core::LayoutGroup::hasProperty(propertyName))\
		return Core::LayoutGroup::getPointer(propertyName);\
	std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist or the property is const!");\
}\
virtual std::vector<void*> getArrayElementPointers(std::string propertyName) {\
	if (propertyName == "shrinkableChildWidth") {\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "shrinkableChildHeight") {\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "childForceExpandWidth") {\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "childForceExpandHeight") {\
		throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " is not an array!");\
	}\
	if (Core::LayoutGroup::hasProperty(propertyName))\
		return Core::LayoutGroup::getArrayElementPointers(propertyName);\
	std::cout << "Warning: The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::HorizontalLayoutGroup::" + propertyName + " does not exist or the property is const!");\
}\
virtual void serialize(std::ostream& os) const {\
		Mirror::serialize(shrinkableChildWidth, os);\
		Mirror::serialize(shrinkableChildHeight, os);\
		Mirror::serialize(childForceExpandWidth, os);\
		Mirror::serialize(childForceExpandHeight, os);\
	Core::LayoutGroup::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(shrinkableChildWidth, is);\
		Mirror::deserialize(shrinkableChildHeight, is);\
		Mirror::deserialize(childForceExpandWidth, is);\
		Mirror::deserialize(childForceExpandHeight, is);\
	Core::LayoutGroup::deserialize(is);\
}
#endif