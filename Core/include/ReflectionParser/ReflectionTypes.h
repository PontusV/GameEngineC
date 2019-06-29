#ifndef REFLECTION_TYPES_H
#define REFLECTION_TYPES_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <array>
#include <tuple>
#include <functional> // std::ref


namespace Mirror {
	// Type Conversion helper for the generated headers
	template<typename From, typename To>
	std::enable_if_t<std::is_convertible<From, To>::value, To> convertType(From from) {
		return static_cast<To>(from);
	}

	template<typename From, typename To>
	std::enable_if_t<!std::is_convertible<From, To>::value, To> convertType(From from) {
		throw std::invalid_argument("Cannot convert type!\n");
	}

	// Array
	/*template<typename From, typename To>
	std::enable_if_t<std::is_array<From>::value && std::is_array<To>::value && std::is_same<From, To>::value, To*> convertType(From from) {
		return from;
	}

	template<typename From, typename To>
	std::enable_if_t<!std::is_array<From>::value && std::is_array<To>::value, To*> convertType(From from) {
		throw std::invalid_argument("Cannot convert type!\n");
	}

	template<typename From, typename To>
	std::enable_if_t<std::is_array<From>::value && !std::is_array<To>::value, To*> convertType(From from) {
		throw std::invalid_argument("Cannot convert type!\n");
	}

	template<typename From, typename To>
	std::enable_if_t<std::is_array<From>::value && std::is_array<To>::value && !std::is_same<From, To>::value, To*> convertType(From from) {
		throw std::invalid_argument("Cannot convert type!\n");
	}*/

	// Array
	template<typename From, std::size_t FromN, typename To, std::size_t ToN>
	std::enable_if_t<std::is_convertible<From, To>::value, std::array<To, ToN>> convertArrayType(From from[FromN]) {
		// Array conversion for each element
		if (FromN > ToN)
			throw std::invalid_argument("Too many elements given!");
		else if (ToN < FromN)
			throw std::invalid_argument("Too few elements given!");
		std::array<To, ToN> result;
		for (std::size_t i = 0; i < FromN; i++) {
			result[i] = static_cast<To>(from[i]);
		}
		return result;
	}

	template<typename From, std::size_t FromN, typename To, std::size_t ToN>
	std::enable_if_t<!std::is_convertible<From, To>::value, std::array<To, ToN>> convertArrayType(From from[FromN]) {
		throw std::invalid_argument("Cannot convert type!\n");
	}

	// Helper for invoke
	// Invoke const member function
	template<typename R, typename Class, typename... FnArgs, typename ClassType, typename... Args>
	std::enable_if_t<std::is_invocable<R(Class::*)(FnArgs...) const, ClassType, Args&&...>::value, void> invoke(R(Class::*callable)(FnArgs...) const, ClassType* instance, Args&&... args) {
		(instance->*callable)(args...);
	}
	// Invoke non-const member function
	template<typename R, typename Class, typename... FnArgs, typename ClassType, typename... Args>
	std::enable_if_t<std::is_invocable<R(Class::*)(FnArgs...), ClassType, Args&&...>::value, void> invoke(R(Class::* callable)(FnArgs...), ClassType* instance, Args&&... args) {
		(instance->*callable)(args...);
	}
	// Invoke static function
	template<typename R, typename... FnArgs, typename ClassType, typename... Args>
	std::enable_if_t<std::is_invocable<R(*)(FnArgs...), Args&&...>::value, void> invoke(R(*callable)(FnArgs...), ClassType* instance, Args&&... args) {
		(*callable)((FnArgs)args...);
	}

	template<typename Fn, typename... Args>
	std::enable_if_t<!std::is_invocable<Fn, Args&&...>::value, void> invoke(Fn callable, Args&&... args) {
		std::cout << "Cannot invoke with incompatible arguments!\n";
	}

	enum class AccessSpecifier {
		INVALID = 0,
		PUBLIC,
		PROTECTED,
		PRIVATE
	};

	struct Type {
		std::string name;
	};

	struct VariableType : public Type {
		bool isPointer = false;
		bool isReference = false;
		bool isConst = false;
		bool isArray = false;
		std::size_t arraySize = 0;
		bool operator==(const VariableType& other) const {
			int pointerCount = isPointer + isArray;
			int otherPointerCount = other.isPointer + other.isArray;
			return isReference == other.isReference &&
				isConst == other.isConst &&
				pointerCount == otherPointerCount &&
				name == other.name;
		}
		bool operator!=(const VariableType& other) const {
			return !(*this == other);
		}
		// Data Type Queries
		bool isDecimal() const {
			if (name == "float") return true;
			if (name == "double") return true;
			if (name == "long double") return true;
			return false;
		}
		bool isNumber() const {
			if (name == "std::size_t") return true;
			if (name == "short int") return true;
			if (name == "unsigned short int") return true;
			if (name == "unsigned int") return true;
			if (name == "int") return true;
			if (name == "long int") return true;
			if (name == "unsigned long int") return true;
			if (name == "long long int") return true;
			if (name == "unsigned long long int") return true;
			if (name == "char") return true;
			if (name == "signed char") return true;
			if (name == "unsigned char") return true;
			if (name == "float") return true;
			if (name == "double") return true;
			if (name == "long double") return true;
			if (name == "wchar_t") return true;
			return false;
		}
		bool isUnsignedNumber() const {
			if (name == "std::size_t") return true;
			if (name == "unsigned short int") return true;
			if (name == "unsigned int") return true;
			if (name == "unsigned long int") return true;
			if (name == "unsigned long long int") return true;
			if (name == "unsigned char") return true;
			return false;
		}
		bool isSignedNumber() const {
			if (name == "short int") return true;
			if (name == "int") return true;
			if (name == "long int") return true;
			if (name == "long long int") return true;
			if (name == "char") return true;
			if (name == "signed char") return true;
			if (name == "float") return true;
			if (name == "double") return true;
			if (name == "long double") return true;
			if (name == "wchar_t") return true;
			return false;
		}
		bool isChar() const {
			if (name == "char") return true;
			if (name == "signed char") return true;
			if (name == "wchar_t") return true;
			return false;
		}
		bool isBool() const {
			if (name == "bool") return true;
			return false;
		}
		bool isCArray() const {
			return isArray;
		}
		bool isString() const {
			if (name == "std::string") return true;
			return false;
		}
		bool isWideString() const {
			if (name == "std::wstring") return true;
			return false;
		}
		bool isVector() const {
			if (name.find("std::vector<") != std::string::npos) return true;
			return false;
		}
		bool isEnum() const {
			return false;
		}
		bool isObject() const {
			if (isNumber() || isBool() || isString() || isVector() || isEnum())
				return false;
			else
				return true;
		}
		// End of Data Type Queries
		/* Returns the Type from the template. */
		Type getTemplateType() const {
			std::size_t index = name.find('<');
			if (index != std::string::npos) {
				std::size_t endIndex = name.find('>');
				if (endIndex != std::string::npos) {
					std::string typeName = name.substr(index + 1, endIndex - index);
					return Type{ typeName };
				}
			}
			return Type{};
		}
	};

	struct Variable {
		VariableType type;
		std::string name;
		bool operator==(const Variable& other) const {
			return name == other.name && type == other.type;
		}
		bool operator!=(const Variable& other) const {
			return !(*this == other);
		}
	};

	struct ClassMember {
		bool isStatic = false;
		AccessSpecifier accessSpecifier = AccessSpecifier::INVALID;
		std::vector<std::string> annotatedAttributes;
		bool hasAttribute(std::string attribute) {
			for (const std::string& entry : annotatedAttributes) {
				if (entry == attribute)
					return true;
			}
			return false;
		}
	};

	struct Property : public Variable, public ClassMember {
		template<typename T, typename ClassType>
		T getValue(ClassType* instance) {
			return instance->getValue_impl<T>(name);
		}
		template<typename T, std::size_t N, typename ClassType>
		std::array<T, N> getArrayValue(ClassType* instance) {
			return instance->getArrayValue_impl<T, N>(name);
		}
		template<typename ClassType, typename T>
		void setValue(ClassType* instance, T value) {
			instance->setValue_impl(name, value);
		}
		template<typename T, std::size_t N, typename ClassType>
		void setArrayValue(ClassType* instance, T(&value)[N]) {
			instance->setArrayValue_impl<T, N>(name, value);
		}
	};

	struct Function : public ClassMember {
		bool isConst = false;
		VariableType returnType;
		std::string name;
		std::vector<Variable> parameters;
		template<typename ClassType, typename... Ts>
		void invoke(ClassType* instance, Ts... args) {
			if (!instance->template invoke_impl<Ts...>(name, args...))
				std::cout << "Warning: Could not invoke " + instance->getType().name + "::" + name + ". The arguments are either invalid or the function does not exist!" << std::endl;
		}
		bool operator==(const Function& other) const {
			if (name != other.name) return false;
			if (parameters.size() != other.parameters.size()) return false;
			for (std::size_t i = 0; i < parameters.size();i++) {
				if (parameters[i] != other.parameters[i]) return false;
			}
			return true;
		}
	};

	struct Class : public Type {
		std::vector<Property> properties;
		std::vector<Function> functions;
		std::vector<Type> baseClasses;
		std::vector<std::string> annotatedAttributes;
		Property getProperty(std::string propertyName) {
			for (const Property& prop : properties) {
				if (prop.name == propertyName)
					return prop;
			}
			std::cout << "Property(" << name << "::" << propertyName << ") not found!\n";
			return Property{};
		}
		Function getFunction(std::string functionName) {
			for (const Function& fun : functions) {
				if (fun.name == functionName)
					return fun;
			}
			std::cout << "Function(" << name << "::" << functionName << ") not found!\n";
			Function invalid{};
			invalid.name = "INVALID_FUNCTION";
			return invalid;
		}
		bool hasAttribute(std::string attribute) {
			for (const std::string& entry : annotatedAttributes) {
				if (entry == attribute)
					return true;
			}
			return false;
		}
		bool operator==(const Class& other) const {
			return typeID == other.typeID;
		}
		bool operator!=(const Class& other) const {
			return typeID != other.typeID;
		}
		Class(unsigned int typeID) : typeID(typeID) {}
	public:
		const unsigned int typeID;
	};
}

#endif