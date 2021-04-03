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
	newClass.name = "Foo2";\
	newClass.isAbstract = false;\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
\
	Mirror::Property newProperty;\
	newProperty.name = "constStringProperty";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "std::string";\
	newPropertyType.isConst = true;\
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
	newProperty.name = "integer";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "int";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = true;\
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
	newProperty.name = "staticBoolean";\
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
	newProperty.isStatic = true;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "constFloat";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "float";\
	newPropertyType.isConst = true;\
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
	newProperty.name = "simpleFloat";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "float";\
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
	Mirror::Function newFunction;\
	newFunction.name = "arrayFunctionTest";\
	newFunction.returnType.name = "bool";\
	newFunction.returnType.isConst = false;\
	newFunction.returnType.isPointer = false;\
	newFunction.returnType.isReference = false;\
	newFunction.returnType.isEnumeration = false;\
	newFunction.returnType.isArray = false;\
	newFunction.returnType.arraySize = 0;\
	newFunction.isConst = false;\
	newFunction.isStatic = true;\
	newFunction.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newFunction.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}, Mirror::Annotation{"world", {}}};\
\
	Mirror::Variable newParameter;\
	newParameter.name = "floatArray";\
	newParameter.type.name = "float";\
	newParameter.type.isConst = false;\
	newParameter.type.isPointer = false;\
	newParameter.type.isReference = false;\
	newParameter.type.isEnumeration = false;\
	newParameter.type.isArray = true;\
	newParameter.type.arraySize = 5;\
	newFunction.parameters.push_back(newParameter);\
	newClass.functions.push_back(newFunction);\
\
	newFunction = {};\
	newFunction.name = "isEven";\
	newFunction.returnType.name = "void";\
	newFunction.returnType.isConst = false;\
	newFunction.returnType.isPointer = false;\
	newFunction.returnType.isReference = false;\
	newFunction.returnType.isEnumeration = false;\
	newFunction.returnType.isArray = false;\
	newFunction.returnType.arraySize = 0;\
	newFunction.isConst = true;\
	newFunction.isStatic = false;\
	newFunction.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newFunction.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
\
	newParameter = {};\
	newParameter.name = "constStringReference";\
	newParameter.type.name = "std::string";\
	newParameter.type.isConst = true;\
	newParameter.type.isPointer = true;\
	newParameter.type.isReference = false;\
	newParameter.type.isEnumeration = false;\
	newParameter.type.isArray = false;\
	newParameter.type.arraySize = 0;\
	newFunction.parameters.push_back(newParameter);\
	newClass.functions.push_back(newFunction);\
\
	newFunction = {};\
	newFunction.name = "staticMethod";\
	newFunction.returnType.name = "bool";\
	newFunction.returnType.isConst = false;\
	newFunction.returnType.isPointer = false;\
	newFunction.returnType.isReference = false;\
	newFunction.returnType.isEnumeration = false;\
	newFunction.returnType.isArray = false;\
	newFunction.returnType.arraySize = 0;\
	newFunction.isConst = false;\
	newFunction.isStatic = true;\
	newFunction.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newFunction.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}, Mirror::Annotation{"world", {}}};\
\
	newParameter = {};\
	newParameter.name = "cloud";\
	newParameter.type.name = "float";\
	newParameter.type.isConst = false;\
	newParameter.type.isPointer = false;\
	newParameter.type.isReference = false;\
	newParameter.type.isEnumeration = false;\
	newParameter.type.isArray = false;\
	newParameter.type.arraySize = 0;\
	newFunction.parameters.push_back(newParameter);\
\
	newParameter = {};\
	newParameter.name = "integer";\
	newParameter.type.name = "int";\
	newParameter.type.isConst = false;\
	newParameter.type.isPointer = false;\
	newParameter.type.isReference = false;\
	newParameter.type.isEnumeration = false;\
	newParameter.type.isArray = false;\
	newParameter.type.arraySize = 0;\
	newFunction.parameters.push_back(newParameter);\
\
	newParameter = {};\
	newParameter.name = "notInteger";\
	newParameter.type.name = "long";\
	newParameter.type.isConst = false;\
	newParameter.type.isPointer = false;\
	newParameter.type.isReference = false;\
	newParameter.type.isEnumeration = false;\
	newParameter.type.isArray = false;\
	newParameter.type.arraySize = 0;\
	newFunction.parameters.push_back(newParameter);\
	newClass.functions.push_back(newFunction);\
\
	newFunction = {};\
	newFunction.name = "arrayFunctionTest2";\
	newFunction.returnType.name = "bool";\
	newFunction.returnType.isConst = false;\
	newFunction.returnType.isPointer = false;\
	newFunction.returnType.isReference = false;\
	newFunction.returnType.isEnumeration = false;\
	newFunction.returnType.isArray = false;\
	newFunction.returnType.arraySize = 0;\
	newFunction.isConst = false;\
	newFunction.isStatic = true;\
	newFunction.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newFunction.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}, Mirror::Annotation{"world", {}}};\
\
	newParameter = {};\
	newParameter.name = "floatArray";\
	newParameter.type.name = "float";\
	newParameter.type.isConst = false;\
	newParameter.type.isPointer = false;\
	newParameter.type.isReference = false;\
	newParameter.type.isEnumeration = false;\
	newParameter.type.isArray = true;\
	newParameter.type.arraySize = 10;\
	newFunction.parameters.push_back(newParameter);\
	newClass.functions.push_back(newFunction);\
	return newClass;\
}\
protected:\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "constStringProperty") {\
			return Mirror::convertType<T>(constStringProperty);\
		}\
		if (propertyName == "integer") {\
			return Mirror::convertType<T>(integer);\
		}\
		if (propertyName == "staticBoolean") {\
			return Mirror::convertType<T>(staticBoolean);\
		}\
		if (propertyName == "constFloat") {\
			return Mirror::convertType<T>(constFloat);\
		}\
		if (propertyName == "simpleFloat") {\
			return Mirror::convertType<T>(simpleFloat);\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Foo2::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Foo2::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Foo2::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Foo2::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "constStringProperty") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "integer") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "staticBoolean") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "constFloat") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "simpleFloat") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Foo2::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Foo2::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Foo2::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Foo2::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
	try {\
		if (propertyName == "constStringProperty") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "integer") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "staticBoolean") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "constFloat") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "simpleFloat") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Foo2::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Foo2::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Foo2::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "integer") {\
			Mirror::setVariableValue(integer, value);\
			return true;\
		}\
		if (propertyName == "staticBoolean") {\
			Mirror::setVariableValue(staticBoolean, value);\
			return true;\
		}\
		if (propertyName == "simpleFloat") {\
			Mirror::setVariableValue(simpleFloat, value);\
			return true;\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Foo2::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Foo2::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	return false;\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "integer") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "staticBoolean") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "simpleFloat") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Foo2::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Foo2::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	return false;\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "integer") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "staticBoolean") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "simpleFloat") {\
			throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
			return true;\
		}\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
	}\
	return false;\
}\
bool onUpdate(std::string propertyName) {\
	if (propertyName == "integer") {\
		return true;\
	}\
	if (propertyName == "staticBoolean") {\
		return true;\
	}\
	if (propertyName == "simpleFloat") {\
		return true;\
	}\
	return false;\
}\
bool invoke(std::string functionName, float* param0) {\
	if (functionName == "arrayFunctionTest") {\
		Mirror::invoke(&Foo2::arrayFunctionTest, this, param0);\
		return true;\
	}\
	if (functionName == "arrayFunctionTest2") {\
		Mirror::invoke(&Foo2::arrayFunctionTest2, this, param0);\
		return true;\
	}\
	return false;\
}\
bool invoke(std::string functionName, const std::string* param0) {\
	if (functionName == "isEven") {\
		Mirror::invoke(&Foo2::isEven, this, param0);\
		return true;\
	}\
	return false;\
}\
bool invoke(std::string functionName, float param0, int param1, long param2) {\
	if (functionName == "staticMethod") {\
		Mirror::invoke(&Foo2::staticMethod, this, param0, param1, param2);\
		return true;\
	}\
	return false;\
}\
template<typename... Ts>\
bool invoke_impl(std::string functionName, Ts... args) { return false; }\
template<typename Arg0>\
bool invoke_impl(std::string functionName, Arg0 arg0) {\
	if (functionName == "arrayFunctionTest") {\
		try {\
			invoke(functionName, (float*)Mirror::convertType<float*>(arg0));\
			return true;\
		} catch(std::exception&) {}\
	}\
	if (functionName == "isEven") {\
		try {\
			invoke(functionName, (const std::string*)Mirror::convertType<const std::string*>(arg0));\
			return true;\
		} catch(std::exception&) {}\
	}\
	if (functionName == "arrayFunctionTest2") {\
		try {\
			invoke(functionName, (float*)Mirror::convertType<float*>(arg0));\
			return true;\
		} catch(std::exception&) {}\
	}\
	return false;\
}\
template<typename Arg0, typename Arg1, typename Arg2>\
bool invoke_impl(std::string functionName, Arg0 arg0, Arg1 arg1, Arg2 arg2) {\
	if (functionName == "staticMethod") {\
		try {\
			invoke(functionName, (float)Mirror::convertType<float>(arg0), (int)Mirror::convertType<int>(arg1), (long)Mirror::convertType<long>(arg2));\
			return true;\
		} catch(std::exception&) {}\
	}\
	return false;\
}\
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
	return std::vector<std::size_t>{};\
}\
static Mirror::Class getClassType() {\
	static Mirror::Class type = getTypeImpl();\
	return type;\
}\
std::size_t getArraySize(std::string propertyName) {\
	if (propertyName == "integer") {\
		throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "staticBoolean") {\
		throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "simpleFloat") {\
		throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
	}\
	std::cout << "Warning: The property Foo2::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Foo2::" + propertyName + " does not exist or the property is const!");\
}\
void* getPointer(std::string propertyName) {\
	if (propertyName == "integer") {\
		return &integer;\
	}\
	if (propertyName == "staticBoolean") {\
		return &staticBoolean;\
	}\
	if (propertyName == "simpleFloat") {\
		return &simpleFloat;\
	}\
	std::cout << "Warning: The property Foo2::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Foo2::" + propertyName + " does not exist or the property is const!");\
}\
std::vector<void*> getArrayElementPointers(std::string propertyName) {\
	if (propertyName == "integer") {\
		throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "staticBoolean") {\
		throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "simpleFloat") {\
		throw std::invalid_argument("The property Foo2::" + propertyName + " is not an array!");\
	}\
	std::cout << "Warning: The property Foo2::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Foo2::" + propertyName + " does not exist or the property is const!");\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(staticBoolean, os);\
		Mirror::serialize(simpleFloat, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(staticBoolean, is);\
		Mirror::deserialize(simpleFloat, is);\
}
#endif