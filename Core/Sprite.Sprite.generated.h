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
	newClass.name = "Sprite";\
	newClass.size = sizeof(Sprite);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "color";\
	newProperty.type.name = "Core::Color";\
	newProperty.type.size = sizeof(Core::Color);\
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
	newProperty.name = "clipEnabled";\
	newProperty.type.name = "bool";\
	newProperty.type.size = sizeof(bool);\
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
	newProperty.name = "clipMaskVertices";\
	newProperty.type.name = "int";\
	newProperty.type.size = sizeof(int);\
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
		if (propertyName == "color") {\
			return Mirror::convertType<Core::Color, T>(instance->color);\
		}\
		if (propertyName == "layerIndex") {\
			return Mirror::convertType<unsigned char, T>(instance->layerIndex);\
		}\
		if (propertyName == "clipEnabled") {\
			return Mirror::convertType<bool, T>(instance->clipEnabled);\
		}\
		if (propertyName == "clipMaskVertices") {\
			return Mirror::convertType<int, T>(instance->clipMaskVertices);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Sprite::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Sprite::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property Sprite::" + propertyName + " does not exist in Sprite!" << "\n";\
	throw std::invalid_argument("The property Sprite::" + propertyName + " does not exist in Sprite!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "color") {\
			throw std::invalid_argument("The property Sprite::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "layerIndex") {\
			throw std::invalid_argument("The property Sprite::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "clipEnabled") {\
			throw std::invalid_argument("The property Sprite::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "clipMaskVertices") {\
			throw std::invalid_argument("The property Sprite::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Sprite::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Sprite::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property Sprite::" + propertyName + " does not exist in Sprite!" << "\n";\
	throw std::invalid_argument("The property Sprite::" + propertyName + " does not exist in Sprite!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "color") {\
			instance->color = Mirror::convertType<T, Core::Color>(value);\
			return;\
		}\
		if (propertyName == "layerIndex") {\
			instance->layerIndex = Mirror::convertType<T, unsigned char>(value);\
			return;\
		}\
		if (propertyName == "clipEnabled") {\
			instance->clipEnabled = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "clipMaskVertices") {\
			instance->clipMaskVertices = Mirror::convertType<T, int>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Sprite::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Sprite::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property Sprite::" + propertyName + " does not exist in Sprite!" << "\n";\
	throw std::invalid_argument("The property Sprite::" + propertyName + " does not exist in Sprite!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "color") {\
			throw std::invalid_argument("The property Sprite::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "layerIndex") {\
			throw std::invalid_argument("The property Sprite::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "clipEnabled") {\
			throw std::invalid_argument("The property Sprite::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "clipMaskVertices") {\
			throw std::invalid_argument("The property Sprite::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Sprite::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Sprite::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property Sprite::" + propertyName + " does not exist in Sprite!" << "\n";\
	throw std::invalid_argument("The property Sprite::" + propertyName + " does not exist in Sprite!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(color, os);\
		Mirror::serialize(layerIndex, os);\
		Mirror::serialize(clipEnabled, os);\
		Mirror::serialize(clipMaskVertices, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(color, is);\
		Mirror::deserialize(layerIndex, is);\
		Mirror::deserialize(clipEnabled, is);\
		Mirror::deserialize(clipMaskVertices, is);\
}
#endif