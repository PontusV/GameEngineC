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
	Mirror::Class newClass(23);\
	newClass.name = "Core::LayoutElement";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Component" });\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::ILayoutElement" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "minSize";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "glm::vec2";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "prefSize";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "glm::vec2";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "flexibleSize";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "glm::vec2";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "minSizeEnabled";\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "prefSizeEnabled";\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "flexibleSizeEnabled";\
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
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
protected:\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "minSize") {\
			return Mirror::convertType<T>(minSize);\
		}\
		if (propertyName == "prefSize") {\
			return Mirror::convertType<T>(prefSize);\
		}\
		if (propertyName == "flexibleSize") {\
			return Mirror::convertType<T>(flexibleSize);\
		}\
		if (propertyName == "minSizeEnabled") {\
			return Mirror::convertType<T>(minSizeEnabled);\
		}\
		if (propertyName == "prefSizeEnabled") {\
			return Mirror::convertType<T>(prefSizeEnabled);\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			return Mirror::convertType<T>(flexibleSizeEnabled);\
		}\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "minSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "minSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
	try {\
		if (propertyName == "minSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "minSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "minSize") {\
			Mirror::setVariableValue(minSize, value);\
			return true;\
		}\
		if (propertyName == "prefSize") {\
			Mirror::setVariableValue(prefSize, value);\
			return true;\
		}\
		if (propertyName == "flexibleSize") {\
			Mirror::setVariableValue(flexibleSize, value);\
			return true;\
		}\
		if (propertyName == "minSizeEnabled") {\
			Mirror::setVariableValue(minSizeEnabled, value);\
			return true;\
		}\
		if (propertyName == "prefSizeEnabled") {\
			Mirror::setVariableValue(prefSizeEnabled, value);\
			return true;\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			Mirror::setVariableValue(flexibleSizeEnabled, value);\
			return true;\
		}\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "minSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "minSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "minSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "prefSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "flexibleSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "minSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "prefSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (Core::Component::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
	}\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist!");\
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
	if (propertyName == "minSize") {\
		return &minSize;\
	}\
	if (propertyName == "prefSize") {\
		return &prefSize;\
	}\
	if (propertyName == "flexibleSize") {\
		return &flexibleSize;\
	}\
	if (propertyName == "minSizeEnabled") {\
		return &minSizeEnabled;\
	}\
	if (propertyName == "prefSizeEnabled") {\
		return &prefSizeEnabled;\
	}\
	if (propertyName == "flexibleSizeEnabled") {\
		return &flexibleSizeEnabled;\
	}\
	if (Core::Component::hasProperty(propertyName))\
		return Core::Component::getPointer(propertyName);\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist or the property is const!");\
}\
virtual std::vector<void*> getArrayElementPointers(std::string propertyName) {\
	if (propertyName == "minSize") {\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "prefSize") {\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "flexibleSize") {\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "minSizeEnabled") {\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "prefSizeEnabled") {\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "flexibleSizeEnabled") {\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
	}\
	if (Core::Component::hasProperty(propertyName))\
		return Core::Component::getArrayElementPointers(propertyName);\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist or the property is const!");\
}\
virtual void serialize(std::ostream& os) const {\
		Mirror::serialize(minSize, os);\
		Mirror::serialize(prefSize, os);\
		Mirror::serialize(flexibleSize, os);\
		Mirror::serialize(minSizeEnabled, os);\
		Mirror::serialize(prefSizeEnabled, os);\
		Mirror::serialize(flexibleSizeEnabled, os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(minSize, is);\
		Mirror::deserialize(prefSize, is);\
		Mirror::deserialize(flexibleSize, is);\
		Mirror::deserialize(minSizeEnabled, is);\
		Mirror::deserialize(prefSizeEnabled, is);\
		Mirror::deserialize(flexibleSizeEnabled, is);\
	Core::Component::deserialize(is);\
}
#endif