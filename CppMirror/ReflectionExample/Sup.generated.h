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
	Mirror::Class newClass(2);\
	newClass.name = "Test::Sup";\
	newClass.isAbstract = false;\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
\
	Mirror::Property newProperty;\
	newProperty.name = "string";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "std::string";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}, Mirror::Annotation{"Dumb", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "stringVector";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "int";\
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
	newProperty.name = "intVector";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "int";\
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
	newProperty.name = "templateParseTester";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "int";\
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
	newProperty.name = "arrayTest";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "int";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = true;\
	newPropertyType.arraySize = 5;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "arrayPointerTest";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "int";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = true;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = false;\
	newPropertyType.isArray = true;\
	newPropertyType.arraySize = 5;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "longVar";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "long";\
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
	newProperty.name = "innerEnumProperty";\
{\
	Mirror::VariableType& newPropertyType = newProperty.type;\
	newPropertyType.name = "Test::Sup::InnerTestEnum";\
	newPropertyType.isConst = false;\
	newPropertyType.isPointer = false;\
	newPropertyType.isReference = false;\
	newPropertyType.isEnumeration = true;\
	newPropertyType.isArray = false;\
	newPropertyType.arraySize = 0;\
}\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PUBLIC;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "reloadTest";\
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
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}, Mirror::Annotation{"Category", {"Ayy"}}, Mirror::Annotation{"Update", {"reload()"}}};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
protected:\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "string") {\
			return Mirror::convertType<T>(string);\
		}\
		if (propertyName == "stringVector") {\
			return Mirror::convertType<T>(stringVector);\
		}\
		if (propertyName == "intVector") {\
			return Mirror::convertType<T>(intVector);\
		}\
		if (propertyName == "templateParseTester") {\
			return Mirror::convertType<T>(templateParseTester);\
		}\
		if (propertyName == "arrayTest") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is an array!");\
		}\
		if (propertyName == "arrayPointerTest") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is an array!");\
		}\
		if (propertyName == "longVar") {\
			return Mirror::convertType<T>(longVar);\
		}\
		if (propertyName == "innerEnumProperty") {\
			return Mirror::convertType<T>(innerEnumProperty);\
		}\
		if (propertyName == "reloadTest") {\
			return Mirror::convertType<T>(reloadTest);\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Test::Sup::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Test::Sup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Test::Sup::" + propertyName + " does not exist!");\
}\
template<typename T>\
std::vector<T> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "string") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "stringVector") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "intVector") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "templateParseTester") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "arrayTest") {\
			std::vector<T> vec(5);\
			for (std::size_t i = 0; i < vec.size(); i++) {\
				vec[i] = Mirror::convertType<T>(arrayTest[i]);\
			}\
			return vec;\
		}\
		if (propertyName == "arrayPointerTest") {\
			std::vector<T> vec(5);\
			for (std::size_t i = 0; i < vec.size(); i++) {\
				vec[i] = Mirror::convertType<T>(arrayPointerTest[i]);\
			}\
			return vec;\
		}\
		if (propertyName == "longVar") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "innerEnumProperty") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "reloadTest") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Test::Sup::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Test::Sup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Test::Sup::" + propertyName + " does not exist!");\
}\
template<typename T>\
T getArrayElementValue_impl(std::string propertyName, std::size_t index) {\
	try {\
		if (propertyName == "string") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "stringVector") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "intVector") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "templateParseTester") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "arrayTest") {\
			if (index >= 5) throw std::out_of_range("getArrayElementValue_impl::ERROR index out of range.");\
			return Mirror::convertType<T>(arrayTest[index]);\
		}\
		if (propertyName == "arrayPointerTest") {\
			if (index >= 5) throw std::out_of_range("getArrayElementValue_impl::ERROR index out of range.");\
			return Mirror::convertType<T>(arrayPointerTest[index]);\
		}\
		if (propertyName == "longVar") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "innerEnumProperty") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "reloadTest") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Test::Sup::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Test::Sup::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "string") {\
			Mirror::setVariableValue(string, value);\
			return true;\
		}\
		if (propertyName == "stringVector") {\
			Mirror::setVariableValue(stringVector, value);\
			return true;\
		}\
		if (propertyName == "intVector") {\
			Mirror::setVariableValue(intVector, value);\
			return true;\
		}\
		if (propertyName == "templateParseTester") {\
			Mirror::setVariableValue(templateParseTester, value);\
			return true;\
		}\
		if (propertyName == "arrayTest") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is an array!");\
		}\
		if (propertyName == "arrayPointerTest") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is an array!");\
		}\
		if (propertyName == "longVar") {\
			Mirror::setVariableValue(longVar, value);\
			return true;\
		}\
		if (propertyName == "innerEnumProperty") {\
			Mirror::setVariableValue(innerEnumProperty, value);\
			return true;\
		}\
		if (propertyName == "reloadTest") {\
			Mirror::setVariableValue(reloadTest, value);\
			return true;\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Test::Sup::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	return false;\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "string") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "stringVector") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "intVector") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "templateParseTester") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "arrayTest") {\
			for (std::size_t i = 0; i < 5; i++) {\
				Mirror::setVariableValue(arrayTest[i], value[i]);\
			}\
			return true;\
		}\
		if (propertyName == "arrayPointerTest") {\
			for (std::size_t i = 0; i < 5; i++) {\
				Mirror::setVariableValue(arrayPointerTest[i], value[i]);\
			}\
			return true;\
		}\
		if (propertyName == "longVar") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "innerEnumProperty") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "reloadTest") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Test::Sup::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	return false;\
}\
template<typename T>\
bool setArrayElementValue_impl(std::string propertyName, std::size_t index, T value) {\
	try {\
		if (propertyName == "string") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "stringVector") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "intVector") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "templateParseTester") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "arrayTest") {\
			if (index >= 5) throw std::out_of_range("setArrayElementValue_impl::ERROR index out of range.");\
			Mirror::setVariableValue(arrayTest[index], value);\
			return true;\
		}\
		if (propertyName == "arrayPointerTest") {\
			if (index >= 5) throw std::out_of_range("setArrayElementValue_impl::ERROR index out of range.");\
			Mirror::setVariableValue(arrayPointerTest[index], value);\
			return true;\
		}\
		if (propertyName == "longVar") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "innerEnumProperty") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
			return true;\
		}\
		if (propertyName == "reloadTest") {\
			throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
			return true;\
		}\
	} catch(std::exception& e) {\
		std::cout << e.what() << std::endl;\
		throw e;\
	}\
	return false;\
}\
bool onUpdate(std::string propertyName) {\
	if (propertyName == "string") {\
		return true;\
	}\
	if (propertyName == "stringVector") {\
		return true;\
	}\
	if (propertyName == "intVector") {\
		return true;\
	}\
	if (propertyName == "templateParseTester") {\
		return true;\
	}\
	if (propertyName == "arrayTest") {\
		return true;\
	}\
	if (propertyName == "arrayPointerTest") {\
		return true;\
	}\
	if (propertyName == "longVar") {\
		return true;\
	}\
	if (propertyName == "innerEnumProperty") {\
		return true;\
	}\
	if (propertyName == "reloadTest") {\
		reload();\
		return true;\
	}\
	return false;\
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
	return 2;\
}\
static std::vector<std::size_t> getBaseTypeIDs() {\
	return std::vector<std::size_t>{};\
}\
static Mirror::Class getClassType() {\
	static Mirror::Class type = getTypeImpl();\
	return type;\
}\
std::size_t getArraySize(std::string propertyName) {\
	if (propertyName == "string") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "stringVector") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "intVector") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "templateParseTester") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "arrayTest") {\
		return 5;\
	}\
	if (propertyName == "arrayPointerTest") {\
		return 5;\
	}\
	if (propertyName == "longVar") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "innerEnumProperty") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "reloadTest") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	std::cout << "Warning: The property Test::Sup::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Test::Sup::" + propertyName + " does not exist or the property is const!");\
}\
void* getPointer(std::string propertyName) {\
	if (propertyName == "string") {\
		return &string;\
	}\
	if (propertyName == "stringVector") {\
		return &stringVector;\
	}\
	if (propertyName == "intVector") {\
		return &intVector;\
	}\
	if (propertyName == "templateParseTester") {\
		return &templateParseTester;\
	}\
	if (propertyName == "arrayTest") {\
		return &arrayTest;\
	}\
	if (propertyName == "arrayPointerTest") {\
		return &arrayPointerTest;\
	}\
	if (propertyName == "longVar") {\
		return &longVar;\
	}\
	if (propertyName == "innerEnumProperty") {\
		return &innerEnumProperty;\
	}\
	if (propertyName == "reloadTest") {\
		return &reloadTest;\
	}\
	std::cout << "Warning: The property Test::Sup::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Test::Sup::" + propertyName + " does not exist or the property is const!");\
}\
std::vector<void*> getArrayElementPointers(std::string propertyName) {\
	if (propertyName == "string") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "stringVector") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "intVector") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "templateParseTester") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "arrayTest") {\
		std::vector<void*> vec(5);\
		for (std::size_t i = 0; i < vec.size(); i++) {\
			vec[i] = &arrayTest[i];\
		}\
		return vec;\
	}\
	if (propertyName == "arrayPointerTest") {\
		std::vector<void*> vec(5);\
		for (std::size_t i = 0; i < vec.size(); i++) {\
			vec[i] = &arrayPointerTest[i];\
		}\
		return vec;\
	}\
	if (propertyName == "longVar") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "innerEnumProperty") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	if (propertyName == "reloadTest") {\
		throw std::invalid_argument("The property Test::Sup::" + propertyName + " is not an array!");\
	}\
	std::cout << "Warning: The property Test::Sup::" + propertyName + " does not exist or the property is const!" << "\n";\
	throw std::invalid_argument("The property Test::Sup::" + propertyName + " does not exist or the property is const!");\
}\
void serialize(std::ostream& os) const {\
		Mirror::serialize(string, os);\
		Mirror::serialize(stringVector, os);\
		Mirror::serialize(intVector, os);\
		Mirror::serialize(templateParseTester, os);\
		Mirror::serialize(arrayTest, os);\
		Mirror::serialize(longVar, os);\
		Mirror::serialize(innerEnumProperty, os);\
		Mirror::serialize(reloadTest, os);\
}\
void deserialize(std::istream& is) {\
		Mirror::deserialize(string, is);\
		Mirror::deserialize(stringVector, is);\
		Mirror::deserialize(intVector, is);\
		Mirror::deserialize(templateParseTester, is);\
		Mirror::deserialize(arrayTest, is);\
		Mirror::deserialize(longVar, is);\
		Mirror::deserialize(innerEnumProperty, is);\
		Mirror::deserialize(reloadTest, is);\
		reload();\
}
#endif