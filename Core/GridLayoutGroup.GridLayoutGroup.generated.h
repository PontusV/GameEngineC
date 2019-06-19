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
	Mirror::Class newClass(23);\
	newClass.name = "Core::GridLayoutGroup";\
	newClass.annotatedAttributes = {"Reflect"};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::LayoutController" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "cellSize";\
	newProperty.type.name = "float";\
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
virtual float getValue2797886853000(std::string propertyName) {\
	if (propertyName == "cellSize") {\
		return this->cellSize;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "cellSize") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
		if (Core::LayoutController::hasProperty(propertyName))\
			return Core::LayoutController::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::GridLayoutGroup::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::GridLayoutGroup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::GridLayoutGroup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::GridLayoutGroup::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "cellSize") {\
			throw std::invalid_argument("The property Core::GridLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (Core::LayoutController::hasProperty(propertyName))\
			return Core::LayoutController::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::GridLayoutGroup::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::GridLayoutGroup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::GridLayoutGroup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::GridLayoutGroup::" + propertyName + " does not exist!");\
}\
virtual bool setValue(std::string propertyName, float value) {\
	if (propertyName == "cellSize") {\
		this->cellSize = value;\
		return true;\
	}\
	return false;\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "cellSize") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
		if (Core::LayoutController::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::GridLayoutGroup::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::GridLayoutGroup::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::GridLayoutGroup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::GridLayoutGroup::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "cellSize") {\
			throw std::invalid_argument("The property Core::GridLayoutGroup::" + propertyName + " is not an array!");\
		}\
		if (Core::LayoutController::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::GridLayoutGroup::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::GridLayoutGroup::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::GridLayoutGroup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::GridLayoutGroup::" + propertyName + " does not exist!");\
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
virtual Mirror::Class getType() {\
	static Mirror::Class type = getTypeImpl();\
	return type;\
}\
virtual void serialize(std::ostream& os) const {\
		Mirror::serialize(cellSize, os);\
	Core::LayoutController::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(cellSize, is);\
	Core::LayoutController::deserialize(is);\
}
#endif