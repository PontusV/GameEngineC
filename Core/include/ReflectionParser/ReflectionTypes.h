#ifndef REFLECTION_TYPES_H
#define REFLECTION_TYPES_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <type_traits>


namespace Mirror {
	// Type Conversion helper for the generated headers
	template<typename From, typename To>
	std::enable_if_t<std::is_convertible<From, To>::value, To> convertType(From from) {
		return To(from);
	}

	template<typename From, typename To>
	std::enable_if_t<!std::is_convertible<From, To>::value, To> convertType(From from) {
		throw std::invalid_argument("Cannot convert type!\n");
	}

	// Helper for invoke
	// Invoke member function
	template<typename Fn, typename ClassType, typename... Args>
	std::enable_if_t<std::is_member_function_pointer<Fn>::value && std::is_invocable<Fn, ClassType, Args...>::value, void> invoke(Fn callable, ClassType* instance, Args... args) {
		(instance->*callable)(args...);
	}
	// Invoke static function
	template<typename Fn, typename ClassType, typename... Args>
	std::enable_if_t<!std::is_member_function_pointer<Fn>::value && std::is_invocable<Fn, Args...>::value, void> invoke(Fn callable, ClassType* instance, Args... args) {
		(*callable)(args...);
	}

	template<typename Fn, typename... Args>
	std::enable_if_t<!std::is_invocable<Fn, Args...>::value, void> invoke(Fn callable, Args... args) {
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
		size_t size = 0;
	};

	struct VariableType : public Type {
		bool isPointer = false;
		bool isReference = false;
		bool isConst = false;
	};

	struct Variable {
		VariableType type;
		std::string name;
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
			return ClassType::template getValue<T>(instance, name);
		}
		template<typename ClassType, typename T>
		void setValue(ClassType* instance, T value) {
			return ClassType::setValue(instance, name, value);
		}
	};

	struct Function : public ClassMember {
		bool isConst = false;
		VariableType returnType;
		std::string name;
		std::vector<Variable> parameters;
		template<typename ClassType, typename... Ts>
		void invoke(ClassType* instance, Ts... args) {
			ClassType::invoke(instance, name, args...);
		}
	};

	struct Class : public Type {
		std::vector<Property> properties;
		std::vector<Function> functions;
		std::vector<Type> baseClasses;
		std::vector<std::string> annotatedAttributes;
		Property getProperty(std::string name) {
			for (const Property& prop : properties) {
				if (prop.name == name)
					return prop;
			}
			std::cout << "Property(" << name << ") not found!\n";
			return Property{};
		}
		Function getFunction(std::string name) {
			for (const Function& fun : functions) {
				if (fun.name == name)
					return fun;
			}
			std::cout << "Function(" << name << ") not found!\n";
			return Function{};
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
	private:
		const unsigned int typeID;
	};
}

#endif