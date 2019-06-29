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
	Mirror::Class newClass(30);\
	newClass.name = "Core::InputField";\
	newClass.annotatedAttributes = {"Reflect"};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Selectable" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "initText";\
	newProperty.type.name = "std::wstring";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "onSubmit";\
	newProperty.type.name = "ComponentFunctionHandle<std::wstring>";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "characterLimit";\
	newProperty.type.name = "std::size_t";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "markColor";\
	newProperty.type.name = "Core::Color";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "highlightColor";\
	newProperty.type.name = "Core::Color";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
protected:\
std::wstring getValue3223175238000(std::string propertyName) {\
	if (propertyName == "initText") {\
		return this->initText;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
ComponentFunctionHandle<std::wstring> getValue3958114413000(std::string propertyName) {\
	if (propertyName == "onSubmit") {\
		return this->onSubmit;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
std::size_t getValue3491869076000(std::string propertyName) {\
	if (propertyName == "characterLimit") {\
		return this->characterLimit;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
Core::Color getValue1701103765000(std::string propertyName) {\
	if (propertyName == "markColor") {\
		return this->markColor;\
	}\
	if (propertyName == "highlightColor") {\
		return this->highlightColor;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "initText") {\
			return Mirror::convertType<std::wstring, T>(getValue3223175238000(propertyName));\
		}\
		if (propertyName == "onSubmit") {\
			return Mirror::convertType<ComponentFunctionHandle<std::wstring>, T>(getValue3958114413000(propertyName));\
		}\
		if (propertyName == "characterLimit") {\
			return Mirror::convertType<std::size_t, T>(getValue3491869076000(propertyName));\
		}\
		if (propertyName == "markColor") {\
			return Mirror::convertType<Core::Color, T>(getValue1701103765000(propertyName));\
		}\
		if (propertyName == "highlightColor") {\
			return Mirror::convertType<Core::Color, T>(getValue1701103765000(propertyName));\
		}\
		if (Core::Selectable::hasProperty(propertyName))\
			return Core::Selectable::getValue_impl<T>(propertyName);\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getValue_impl<T>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::InputField::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::InputField::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::InputField::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::InputField::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
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
			return Core::Selectable::getArrayValue_impl<T, N>(propertyName);\
		if (Core::Behaviour::hasProperty(propertyName))\
			return Core::Behaviour::getArrayValue_impl<T, N>(propertyName);\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::InputField::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::InputField::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::InputField::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::InputField::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "initText") {\
			this->initText = Mirror::convertType<T, std::wstring>(value);\
			return true;\
		}\
		if (propertyName == "onSubmit") {\
			this->onSubmit = Mirror::convertType<T, ComponentFunctionHandle<std::wstring>>(value);\
			return true;\
		}\
		if (propertyName == "characterLimit") {\
			this->characterLimit = Mirror::convertType<T, std::size_t>(value);\
			return true;\
		}\
		if (propertyName == "markColor") {\
			this->markColor = Mirror::convertType<T, Core::Color>(value);\
			return true;\
		}\
		if (propertyName == "highlightColor") {\
			this->highlightColor = Mirror::convertType<T, Core::Color>(value);\
			return true;\
		}\
		if (Core::Selectable::setValue_impl<T>(propertyName, value)) return true;\
		if (Core::Behaviour::setValue_impl<T>(propertyName, value)) return true;\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
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
		if (Core::Behaviour::setArrayValue_impl<T, N>(propertyName, value)) return true;\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::InputField::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::InputField::" + propertyName + " was set to a value with an incompatible type!");\
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
	Core::Behaviour::serialize(os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(initText, is);\
		Mirror::deserialize(onSubmit, is);\
		Mirror::deserialize(characterLimit, is);\
		Mirror::deserialize(markColor, is);\
		Mirror::deserialize(highlightColor, is);\
	Core::Selectable::deserialize(is);\
	Core::Behaviour::deserialize(is);\
	Core::Component::deserialize(is);\
}
#endif