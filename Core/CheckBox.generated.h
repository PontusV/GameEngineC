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
	Mirror::Class newClass(12);\
	newClass.name = "Core::CheckBox";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Behaviour" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "onToggle";\
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
	Mirror::VariableType newPropertyType_1;\
	newPropertyType_1.name = "bool";\
	newPropertyType_1.isConst = false;\
	newPropertyType_1.isPointer = false;\
	newPropertyType_1.isReference = false;\
	newPropertyType_1.isEnumeration = false;\
	newPropertyType_1.isArray = false;\
	newPropertyType_1.arraySize = 0;\
	newPropertyType.templateParams.push_back(newPropertyType_1);\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "toggle";\
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
		if (propertyName == "onToggle") {\
			return Mirror::convertType<T>(onToggle);\
		}\
		if (propertyName == "toggle") {\
			return Mirror::convertType<T>(toggle);\
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
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "onToggle") {\
			throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "toggle") {\
			throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::CheckBox::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::CheckBox::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
	try {\
		if (propertyName == "onToggle") {\
			throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "toggle") {\
			throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
		}\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::CheckBox::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "onToggle") {\
			Mirror::setVariableValue(onToggle, value);\
			return true;\
		}\
		if (propertyName == "toggle") {\
			Mirror::setVariableValue(toggle, value);\
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
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "onToggle") {\
			throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "toggle") {\
			throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (Core::Behaviour::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
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
static constexpr std::size_t getTypeID() {\
	return 12;\
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
	if (propertyName == "onToggle") {\
		throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "toggle") {\
		throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
	}\
	if (Core::Behaviour::hasProperty(propertyName))\
		return Core::Behaviour::getArraySize(propertyName);\
	std::cout << "Warning: The property Core::CheckBox::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " does not exist or the property is const!");\
}\
virtual void* getPointer(std::string propertyName) {\
	if (propertyName == "onToggle") {\
		return &onToggle;\
	}\
	if (propertyName == "toggle") {\
		return &toggle;\
	}\
	if (Core::Behaviour::hasProperty(propertyName))\
		return Core::Behaviour::getPointer(propertyName);\
	std::cout << "Warning: The property Core::CheckBox::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " does not exist or the property is const!");\
}\
virtual std::vector<void*> getArrayElementPointers(std::string propertyName) {\
	if (propertyName == "onToggle") {\
		throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "toggle") {\
		throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " is not an array!");\
	}\
	if (Core::Behaviour::hasProperty(propertyName))\
		return Core::Behaviour::getArrayElementPointers(propertyName);\
	std::cout << "Warning: The property Core::CheckBox::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::CheckBox::" + propertyName + " does not exist or the property is const!");\
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