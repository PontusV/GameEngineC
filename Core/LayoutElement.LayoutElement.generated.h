#ifndef REFLECTION_PARSER
#pragma once
#include <ReflectionParser/ReflectionParser.h>
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
	newClass.name = "Core::LayoutElement";\
	newClass.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::Component" });\
	newClass.baseClasses.push_back(Mirror::Type{ "Core::ILayoutElement" });\
\
	Mirror::Property newProperty;\
	newProperty.name = "minSize";\
	newProperty.type.name = "glm::vec2";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "prefSize";\
	newProperty.type.name = "glm::vec2";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "flexibleSize";\
	newProperty.type.name = "glm::vec2";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "minSizeEnabled";\
	newProperty.type.name = "bool";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "prefSizeEnabled";\
	newProperty.type.name = "bool";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
\
	newProperty = {};\
	newProperty.name = "flexibleSizeEnabled";\
	newProperty.type.name = "bool";\
	newProperty.type.isConst = false;\
	newProperty.type.isPointer = false;\
	newProperty.type.isReference = false;\
	newProperty.type.isArray = false;\
	newProperty.type.arraySize = 0;\
	newProperty.isStatic = false;\
	newProperty.accessSpecifier = Mirror::AccessSpecifier::PRIVATE;\
	newProperty.annotatedAttributes = {Mirror::Annotation{"Reflect", {}}};\
	newClass.properties.push_back(newProperty);\
	return newClass;\
}\
protected:\
glm::vec2 getValue226795177000(std::string propertyName) {\
	if (propertyName == "minSize") {\
		return this->minSize;\
	}\
	if (propertyName == "prefSize") {\
		return this->prefSize;\
	}\
	if (propertyName == "flexibleSize") {\
		return this->flexibleSize;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
bool getValue3365180733000(std::string propertyName) {\
	if (propertyName == "minSizeEnabled") {\
		return this->minSizeEnabled;\
	}\
	if (propertyName == "prefSizeEnabled") {\
		return this->prefSizeEnabled;\
	}\
	if (propertyName == "flexibleSizeEnabled") {\
		return this->flexibleSizeEnabled;\
	}\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "minSize") {\
			return Mirror::convertType<glm::vec2, T>(getValue226795177000(propertyName));\
		}\
		if (propertyName == "prefSize") {\
			return Mirror::convertType<glm::vec2, T>(getValue226795177000(propertyName));\
		}\
		if (propertyName == "flexibleSize") {\
			return Mirror::convertType<glm::vec2, T>(getValue226795177000(propertyName));\
		}\
		if (propertyName == "minSizeEnabled") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "prefSizeEnabled") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getValue_impl<T>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "minSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "minSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (Core::Component::hasProperty(propertyName))\
			return Core::Component::getArrayValue_impl<T, N>(propertyName);\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist!");\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "minSize") {\
			this->minSize = Mirror::convertType<T, glm::vec2>(value);\
			return true;\
		}\
		if (propertyName == "prefSize") {\
			this->prefSize = Mirror::convertType<T, glm::vec2>(value);\
			return true;\
		}\
		if (propertyName == "flexibleSize") {\
			this->flexibleSize = Mirror::convertType<T, glm::vec2>(value);\
			return true;\
		}\
		if (propertyName == "minSizeEnabled") {\
			this->minSizeEnabled = Mirror::convertType<T, bool>(value);\
			return true;\
		}\
		if (propertyName == "prefSizeEnabled") {\
			this->prefSizeEnabled = Mirror::convertType<T, bool>(value);\
			return true;\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			this->flexibleSizeEnabled = Mirror::convertType<T, bool>(value);\
			return true;\
		}\
		if (Core::Component::setValue_impl<T>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "minSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSize") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "minSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " is not an array!");\
		}\
		if (Core::Component::setArrayValue_impl<T, N>(propertyName, value)) return true;\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::LayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::LayoutElement::" + propertyName + " does not exist!");\
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
		Mirror::serialize(minSize, os);\
		Mirror::serialize(prefSize, os);\
		Mirror::serialize(flexibleSize, os);\
		Mirror::serialize(minSizeEnabled, os);\
		Mirror::serialize(prefSizeEnabled, os);\
		Mirror::serialize(flexibleSizeEnabled, os);\
	Core::Component::serialize(os);\
}\
virtual void deserialize(std::istream& is) {\
		Mirror::deserialize(minSize, is);\
		Mirror::deserialize(prefSize, is);\
		Mirror::deserialize(flexibleSize, is);\
		Mirror::deserialize(minSizeEnabled, is);\
		Mirror::deserialize(prefSizeEnabled, is);\
		Mirror::deserialize(flexibleSizeEnabled, is);\
	Core::Component::deserialize(is);\
}
#endif