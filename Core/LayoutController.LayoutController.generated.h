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
	newClass.name = "LayoutController";\
	newClass.size = sizeof(LayoutController);\
	newClass.annotatedAttributes = {"Reflect"};\
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
	} catch(std::exception&) {\
		std::cout << "The property LayoutController::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property LayoutController::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property LayoutController::" + propertyName + " does not exist in LayoutController!" << "\n";\
	throw std::invalid_argument("The property LayoutController::" + propertyName + " does not exist in LayoutController!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static std::array<T, N> getValue(ClassType* instance, std::string propertyName) {\
	try {\
	} catch(std::exception&) {\
		std::cout << "The property LayoutController::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property LayoutController::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "The property LayoutController::" + propertyName + " does not exist in LayoutController!" << "\n";\
	throw std::invalid_argument("The property LayoutController::" + propertyName + " does not exist in LayoutController!");\
}\
template<typename T, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value) {\
	try {\
	} catch(std::exception&) {\
		std::cout << "The property LayoutController::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property LayoutController::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property LayoutController::" + propertyName + " does not exist in LayoutController!" << "\n";\
	throw std::invalid_argument("The property LayoutController::" + propertyName + " does not exist in LayoutController!");\
}\
template<typename T, std::size_t N, typename ClassType>\
static void setValue(ClassType* instance, std::string propertyName, T value[N]) {\
	try {\
	} catch(std::exception&) {\
		std::cout << "The property LayoutController::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property LayoutController::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "The property LayoutController::" + propertyName + " does not exist in LayoutController!" << "\n";\
	throw std::invalid_argument("The property LayoutController::" + propertyName + " does not exist in LayoutController!");\
}\
template<typename ClassType, typename... Args>\
static void invoke(ClassType* instance, std::string functionName, Args... args) {\
}\
void serialize(std::ostream& os) const {\
}\
void deserialize(std::istream& is) {\
}
#endif