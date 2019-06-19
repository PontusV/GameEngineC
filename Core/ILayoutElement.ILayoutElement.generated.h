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
	Mirror::Class newClass(18);\
	newClass.name = "ILayoutElement";\
	newClass.size = sizeof(ILayoutElement);\
	newClass.annotatedAttributes = {"Reflect"};\
	return newClass;\
}\
protected:\
virtual float getValue2797886853000(std::string propertyName) {\
	throw std::invalid_argument("Could not find the property!");\
}\
virtual Core::Alignment getValue2036400663000(std::string propertyName) {\
	throw std::invalid_argument("Could not find the property!");\
}\
virtual glm::vec2 getValue226795177000(std::string propertyName) {\
	throw std::invalid_argument("Could not find the property!");\
}\
virtual bool getValue3365180733000(std::string propertyName) {\
	throw std::invalid_argument("Could not find the property!");\
}\
template<typename T>\
T getValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "paddingTop") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
		if (propertyName == "paddingRight") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
		if (propertyName == "paddingBottom") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
		if (propertyName == "paddingLeft") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
		if (propertyName == "spacing") {\
			return Mirror::convertType<float, T>(getValue2797886853000(propertyName));\
		}\
		if (propertyName == "childAlignment") {\
			return Mirror::convertType<Core::Alignment, T>(getValue2036400663000(propertyName));\
		}\
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
		if (propertyName == "shrinkableChildWidth") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "childForceExpandWidth") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "childForceExpandHeight") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "shrinkableChildWidth") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "childForceExpandWidth") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
		if (propertyName == "childForceExpandHeight") {\
			return Mirror::convertType<bool, T>(getValue3365180733000(propertyName));\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::ILayoutElement::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::ILayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
std::array<T, N> getArrayValue_impl(std::string propertyName) {\
	try {\
		if (propertyName == "paddingTop") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "paddingRight") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "paddingBottom") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "paddingLeft") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "spacing") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childAlignment") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "minSize") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSize") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSize") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "minSizeEnabled") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSizeEnabled") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildWidth") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandWidth") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandHeight") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildWidth") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandWidth") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandHeight") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::ILayoutElement::" + propertyName + " cannot be converted to the specified type!" << "\n";\
		throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " cannot be converted to the specified type!");\
	}\
	std::cout << "Warning: The property Core::ILayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " does not exist!");\
}\
virtual bool setValue(std::string propertyName, float value) {\
	return false;\
}\
virtual bool setValue(std::string propertyName, Core::Alignment value) {\
	return false;\
}\
virtual bool setValue(std::string propertyName, glm::vec2 value) {\
	return false;\
}\
virtual bool setValue(std::string propertyName, bool value) {\
	return false;\
}\
template<typename T>\
bool setValue_impl(std::string propertyName, T value) {\
	try {\
		if (propertyName == "paddingTop") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
		if (propertyName == "paddingRight") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
		if (propertyName == "paddingBottom") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
		if (propertyName == "paddingLeft") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
		if (propertyName == "spacing") {\
			if (setValue(propertyName, Mirror::convertType<T, float>(value))) return true;\
		}\
		if (propertyName == "childAlignment") {\
			if (setValue(propertyName, Mirror::convertType<T, Core::Alignment>(value))) return true;\
		}\
		if (propertyName == "minSize") {\
			if (setValue(propertyName, Mirror::convertType<T, glm::vec2>(value))) return true;\
		}\
		if (propertyName == "prefSize") {\
			if (setValue(propertyName, Mirror::convertType<T, glm::vec2>(value))) return true;\
		}\
		if (propertyName == "flexibleSize") {\
			if (setValue(propertyName, Mirror::convertType<T, glm::vec2>(value))) return true;\
		}\
		if (propertyName == "minSizeEnabled") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "prefSizeEnabled") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "shrinkableChildWidth") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "childForceExpandWidth") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "childForceExpandHeight") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "shrinkableChildWidth") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "childForceExpandWidth") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
		if (propertyName == "childForceExpandHeight") {\
			if (setValue(propertyName, Mirror::convertType<T, bool>(value))) return true;\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::ILayoutElement::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::ILayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " does not exist!");\
}\
template<typename T, std::size_t N>\
bool setArrayValue_impl(std::string propertyName, T (&value)[N]) {\
	try {\
		if (propertyName == "paddingTop") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "paddingRight") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "paddingBottom") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "paddingLeft") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "spacing") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childAlignment") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "minSize") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSize") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSize") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "minSizeEnabled") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "prefSizeEnabled") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "flexibleSizeEnabled") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildWidth") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandWidth") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandHeight") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildWidth") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "shrinkableChildHeight") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandWidth") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
		if (propertyName == "childForceExpandHeight") {\
			throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " is not an array!");\
		}\
	} catch(std::exception&) {\
		std::cout << "Warning: The property Core::ILayoutElement::" + propertyName + " was set to a value with an incompatible type!\n";\
		throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " was set to a value with an incompatible type!");\
	}\
	std::cout << "Warning: The property Core::ILayoutElement::" + propertyName + " does not exist!" << "\n";\
	throw std::invalid_argument("The property Core::ILayoutElement::" + propertyName + " does not exist!");\
}\
template<typename... Ts>\
void invoke_impl(std::string functionName, Ts... args) {}\
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
}\
virtual void deserialize(std::istream& is) {\
}
#endif