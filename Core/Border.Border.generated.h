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
	newClass.name = "Border";\
	newClass.size = sizeof(Border);\
	newClass.annotatedAttributes = {"Reflect"};\
\
	Mirror::Property newProperty;\
	newProperty.name = "top";\
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
	newProperty.name = "right";\
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
	newProperty.name = "bottom";\
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
	newProperty.name = "left";\
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
	newProperty.name = "inner";\
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
	newProperty.name = "borderThickness";\
	newProperty.type.name = "std::size_t";\
	newProperty.type.size = sizeof(std::size_t);\
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
		if (propertyName == "top") {\
			return Mirror::convertType<bool, T>(instance->top);\
		}\
		if (propertyName == "right") {\
			return Mirror::convertType<bool, T>(instance->right);\
		}\
		if (propertyName == "bottom") {\
			return Mirror::convertType<bool, T>(instance->bottom);\
		}\
		if (propertyName == "left") {\
			return Mirror::convertType<bool, T>(instance->left);\
		}\
		if (propertyName == "inner") {\
			return Mirror::convertType<bool, T>(instance->inner);\
		}\
		if (propertyName == "borderThickness") {\
			return Mirror::convertType<std::size_t, T>(instance->borderThickness);\
		}\
		if (propertyName == "color") {\
			return Mirror::convertType<Core::Color, T>(instance->color);\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Border::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Border::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property Border::" + propertyName + " does not exist in Border!" << "\n";\
	throw std::invalid_argument("The property Border::" + propertyName + " does not exist in Border!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
		if (propertyName == "top") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "right") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "bottom") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "left") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "inner") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "borderThickness") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "color") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Border::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Border::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property Border::" + propertyName + " does not exist in Border!" << "\n";\
	throw std::invalid_argument("The property Border::" + propertyName + " does not exist in Border!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
		if (propertyName == "top") {\
			instance->top = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "right") {\
			instance->right = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "bottom") {\
			instance->bottom = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "left") {\
			instance->left = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "inner") {\
			instance->inner = Mirror::convertType<T, bool>(value);\
			return;\
		}\
		if (propertyName == "borderThickness") {\
			instance->borderThickness = Mirror::convertType<T, std::size_t>(value);\
			return;\
		}\
		if (propertyName == "color") {\
			instance->color = Mirror::convertType<T, Core::Color>(value);\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Border::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Border::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property Border::" + propertyName + " does not exist in Border!" << "\n";\
	throw std::invalid_argument("The property Border::" + propertyName + " does not exist in Border!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
		if (propertyName == "top") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "right") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "bottom") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "left") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "inner") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "borderThickness") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is an array!");\
			return;\
		}\
		if (propertyName == "color") {\
			throw std::invalid_argument("The property Border::" + propertyName + " is an array!");\
			return;\
		}\
	} catch(std::exception&) {\
		std::cout << "The property Border::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Border::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property Border::" + propertyName + " does not exist in Border!" << "\n";\
	throw std::invalid_argument("The property Border::" + propertyName + " does not exist in Border!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(top, os);\
		Mirror::serialize(right, os);\
		Mirror::serialize(bottom, os);\
		Mirror::serialize(left, os);\
		Mirror::serialize(inner, os);\
		Mirror::serialize(borderThickness, os);\
		Mirror::serialize(color, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(top, is);\
		Mirror::deserialize(right, is);\
		Mirror::deserialize(bottom, is);\
		Mirror::deserialize(left, is);\
		Mirror::deserialize(inner, is);\
		Mirror::deserialize(borderThickness, is);\
		Mirror::deserialize(color, is);\
}
#endif