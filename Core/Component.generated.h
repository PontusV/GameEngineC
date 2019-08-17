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
	Mirror::Class newClass(1);\
	newClass.name = "Core::Component";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::ReflectedObject" });\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Serializable" });\
	return newClass;\
}\
protected:\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (Core::ReflectedObject::hasProperty(propertyName))\
			return Core::ReflectedObject::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Component::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Component::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Component::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Component::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (Core::ReflectedObject::hasProperty(propertyName))\
			return Core::ReflectedObject::getArrayValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Component::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::Component::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Component::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Component::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
	try {\
		if (Core::ReflectedObject::hasProperty(propertyName))\
			return Core::ReflectedObject::getArrayElementValue_impl<T>(propertyName, index);\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Core::Component::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::Component::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Component::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (Core::ReflectedObject::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Component::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Component::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Component::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Component::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (Core::ReflectedObject::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::Component::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::Component::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::Component::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Component::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (Core::ReflectedObject::setArrayElementValue_impl<T>(propertyName, index, value)) return true;\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
	}\
	std::cout << "Warning: The property Core::Component::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::Component::" + propertyName + " does not exist!");\
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
	return 1;\
}\
static std::vector<std::size_t> getBaseTypeIDs() {\
	return std::vector<std::size_t>{ReflectedObject::getTypeID()};\
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
	if (Core::ReflectedObject::hasProperty(propertyName))\
		return Core::ReflectedObject::getArraySize(propertyName);\
	std::cout << "Warning: The property Core::Component::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Component::" + propertyName + " does not exist or the property is const!");\
}\
virtual void* getPointer(std::string propertyName) {\
	if (Core::ReflectedObject::hasProperty(propertyName))\
		return Core::ReflectedObject::getPointer(propertyName);\
	std::cout << "Warning: The property Core::Component::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Component::" + propertyName + " does not exist or the property is const!");\
}\
virtual std::vector<void*> getArrayElementPointers(std::string propertyName) {\
	if (Core::ReflectedObject::hasProperty(propertyName))\
		return Core::ReflectedObject::getArrayElementPointers(propertyName);\
	std::cout << "Warning: The property Core::Component::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Core::Component::" + propertyName + " does not exist or the property is const!");\
}\
virtual void serialize(std::ostream& os) const {\
	Core::ReflectedObject::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
	Core::ReflectedObject::deserialize(is);\
}
#endif