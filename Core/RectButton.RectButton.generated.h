#ifndef REFLECTION_PARSER
#pragma once
#include <ReflectionParser.h>
#include <cstddef>
#include <stdexcept>
#include <type_traits>

#ifdef GENERATED_BODY
#undef GENERATED_BODY
#endif
#define GENERATED_BODY(...) \
private: \
Mirror::Class getTypeImpl() { \
	Mirror::Class newClass(0);\
	newClass.name = "RectButton";\
	newClass.size = sizeof(RectButton);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "colors";\
	newProperty.type.name = "Core::Color";\
	newProperty.type.size = sizeof(Core::Color)*3;\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = true;\
	newProperty.type.arraySize = 3;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "clickFunction";\
	newProperty.type.name = "Core::ComponentFunctionHandleWrapper";\
	newProperty.type.size = sizeof(Core::ComponentFunctionHandleWrapper);\
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
	newProperty.name = "layerIndex";\
	newProperty.type.name = "char";\
	newProperty.type.size = sizeof(char);\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "state";\
	newProperty.type.name = "Core::RectButton::ButtonState";\
	newProperty.type.size = sizeof(Core::RectButton::ButtonState);\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {"Reflect"};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
public:\
Mirror::Class getType() {\
	static Mirror::Class type = getTypeImpl();\
	return type;\
}\
template<typename T, typename ClassType>\
static T getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "colors") {\
			throw std::invalid_argument("The property RectButton::" + propertyName + " is an array!");\
		}\
		if (propertyName == "clickFunction") {\
			return Mirror::convertType<Core::ComponentFunctionHandleWrapper, T>(instance->clickFunction);\
		}\
		if (propertyName == "layerIndex") {\
			return Mirror::convertType<unsigned char, T>(instance->layerIndex);\
		}\
		if (propertyName == "state") {\
			return Mirror::convertType<Core::RectButton::ButtonState, T>(instance->state);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property RectButton::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property RectButton::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property RectButton::" + propertyName + " does not exist in RectButton!" << "\n";\
	throw std::invalid_argument("The property RectButton::" + propertyName + " does not exist in RectButton!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "colors") {\
			return Mirror::convertType<Core::Color , 3, T, N>(instance->colors);\
		}\
		if (propertyName == "clickFunction") {\
			throw std::invalid_argument("The property RectButton::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "layerIndex") {\
			throw std::invalid_argument("The property RectButton::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "state") {\
			throw std::invalid_argument("The property RectButton::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property RectButton::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property RectButton::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property RectButton::" + propertyName + " does not exist in RectButton!" << "\n";\
	throw std::invalid_argument("The property RectButton::" + propertyName + " does not exist in RectButton!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "colors") {\
			throw std::invalid_argument("The property RectButton::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "clickFunction") {\
			instance->clickFunction = Mirror::convertType<T, Core::ComponentFunctionHandleWrapper>(value);\
			return;\
		}\
		if (propertyName == "layerIndex") {\
			instance->layerIndex = Mirror::convertType<T, unsigned char>(value);\
			return;\
		}\
		if (propertyName == "state") {\
			instance->state = Mirror::convertType<T, Core::RectButton::ButtonState>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property RectButton::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property RectButton::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property RectButton::" + propertyName + " does not exist in RectButton!" << "\n";\
	throw std::invalid_argument("The property RectButton::" + propertyName + " does not exist in RectButton!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "colors") {\
			instance->colors = Mirror::convertType<T, N, Core::Color , 3>(value);\
			return;\
		}\
		if (propertyName == "clickFunction") {\
			throw std::invalid_argument("The property RectButton::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "layerIndex") {\
			throw std::invalid_argument("The property RectButton::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "state") {\
			throw std::invalid_argument("The property RectButton::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property RectButton::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property RectButton::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property RectButton::" + propertyName + " does not exist in RectButton!" << "\n";\
	throw std::invalid_argument("The property RectButton::" + propertyName + " does not exist in RectButton!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(colors, os);\
		Mirror::serialize(clickFunction, os);\
		Mirror::serialize(layerIndex, os);\
		Mirror::serialize(state, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(colors, is);\
		Mirror::deserialize(clickFunction, is);\
		Mirror::deserialize(layerIndex, is);\
		Mirror::deserialize(state, is);\
}
#endif