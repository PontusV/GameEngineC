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
	Mirror::Class newClass(30);\
	newClass.name = "Core::InputField";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Selectable" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "initText";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "std::wstring";\
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
	newProperty.name = "onSubmit";\
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
	newPropertyType_1.name = "std::wstring";\
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
	newProperty.name = "characterLimit";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "std::size_t";\
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
	newProperty.name = "markColor";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "Core::Color";\
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
	newProperty.name = "highlightColor";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "Core::Color";\
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
		if (propertyName == "initText") {\
			return Mirror::convertType<T>(initText);\
		}\
		if (propertyName == "onSubmit") {\
			return Mirror::convertType<T>(onSubmit);\
		}\
		if (propertyName == "characterLimit") {\
			return Mirror::convertType<T>(characterLimit);\
		}\
		if (propertyName == "markColor") {\
			return Mirror::convertType<T>(markColor);\
		}\
		if (propertyName == "highlightColor") {\
			return Mirror::convertType<T>(highlightColor);\
		}\
		if (Core::Selectable::hasProperty(propertyName))\
			return Core::Selectable::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::InputField::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::InputField::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::InputField::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::InputField::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "initText") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "onSubmit") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "characterLimit") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "markColor") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "highlightColor") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (Core::Selectable::hasProperty(propertyName))\
			return Core::Selectable::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::InputField::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::InputField::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::InputField::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::InputField::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
	try {\
		if (propertyName == "initText") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "onSubmit") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "characterLimit") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "markColor") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "highlightColor") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (Core::Selectable::hasProperty(propertyName))\
			return Core::Selectable::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Core::InputField::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::InputField::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::InputField::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "initText") {\
			Mirror::setVariableValue(initText, value);\
			return true;\
		}\
		if (propertyName == "onSubmit") {\
			Mirror::setVariableValue(onSubmit, value);\
			return true;\
		}\
		if (propertyName == "characterLimit") {\
			Mirror::setVariableValue(characterLimit, value);\
			return true;\
		}\
		if (propertyName == "markColor") {\
			Mirror::setVariableValue(markColor, value);\
			return true;\
		}\
		if (propertyName == "highlightColor") {\
			Mirror::setVariableValue(highlightColor, value);\
			return true;\
		}\
		if (Core::Selectable::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::InputField::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::InputField::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::InputField::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::InputField::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "initText") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "onSubmit") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "characterLimit") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "markColor") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "highlightColor") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
		}\
		if (Core::Selectable::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::InputField::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::InputField::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::InputField::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::InputField::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "initText") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "onSubmit") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "characterLimit") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "markColor") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "highlightColor") {\
			throw std::invalid_argument("The property Core::InputField::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (Core::Selectable::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
	}\
	std::cout << "Warning: The property Core::InputField::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::InputField::" + propertyName + " does not exist!");\
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
		Mirror::serialize(initText, os);\
		Mirror::serialize(onSubmit, os);\
		Mirror::serialize(characterLimit, os);\
		Mirror::serialize(markColor, os);\
		Mirror::serialize(highlightColor, os);\
	Core::Selectable::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(initText, is);\
		Mirror::deserialize(onSubmit, is);\
		Mirror::deserialize(characterLimit, is);\
		Mirror::deserialize(markColor, is);\
		Mirror::deserialize(highlightColor, is);\
	Core::Selectable::deserialize(is);\
}
#endif