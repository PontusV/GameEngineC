#ifndef REFLECTION_TYPES_H
#define REFLECTION_TYPES_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <tuple>
#include <functional> // std::ref
#include "Utility.h"


namespace Mirror {
	// Helper for invoke
	// Invoke const member function
	template<typename R, typename Class, typename... FnArgs, typename ClassType, typename... Args>
	std::enable_if_t<std::is_invocable<R(Class::*)(FnArgs...) const, ClassType, Args&& ...>::value, void> invoke(R(Class::* callable)(FnArgs...) const, ClassType* instance, Args&& ... args) {
		(instance->*callable)(args...);
	}
	// Invoke non-const member function
	template<typename R, typename Class, typename... FnArgs, typename ClassType, typename... Args>
	std::enable_if_t<std::is_invocable<R(Class::*)(FnArgs...), ClassType, Args&& ...>::value, void> invoke(R(Class::* callable)(FnArgs...), ClassType* instance, Args&& ... args) {
		(instance->*callable)(args...);
	}
	// Invoke static function
	template<typename R, typename... FnArgs, typename ClassType, typename... Args>
	std::enable_if_t<std::is_invocable<R(*)(FnArgs...), Args&& ...>::value, void> invoke(R(*callable)(FnArgs...), ClassType* instance, Args&& ... args) {
		(*callable)((FnArgs)args...);
	}

	template<typename Fn, typename... Args>
	std::enable_if_t<!std::is_invocable<Fn, Args&& ...>::value, void> invoke(Fn callable, Args&& ... args) {
		std::cout << "Cannot invoke with incompatible arguments!\n";
	}

	struct ReflectedType {
		std::size_t typeID;
		std::string typeName;
	};

	enum class AccessSpecifier {
		INVALID = 0,
		PUBLIC,
		PROTECTED,
		PRIVATE
	};

	struct Type {
		std::string name;
		std::size_t size;
	};

	struct VariableType : public Type {
		bool isPointer = false;
		bool isReference = false;
		bool isConst = false;
		bool isEnumeration = false;
		bool isArray = false;
		bool isArithmetic = false;
		bool isIntegral = false;
		bool isFloatingPoint = false;
		bool isSigned = false;
		bool isUnsigned = false;
		std::size_t arraySize = 0;
		std::vector<VariableType> templateParams;
		bool operator==(const VariableType& other) const {
			int pointerCount = isPointer + isArray;
			int otherPointerCount = other.isPointer + other.isArray;
			return isReference == other.isReference &&
				isConst == other.isConst &&
				pointerCount == otherPointerCount &&
				name == other.name &&
				templateParams == other.templateParams;
		}
		bool operator!=(const VariableType& other) const {
			return !(*this == other);
		}
		bool isTemplate() const {
			return templateParams.size() > 0;
		}
		std::vector<VariableType> getTemplateParameters() {
			return templateParams;
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

	struct Annotation {
		Annotation(std::string name, std::vector<std::string> values) : name(name), values(values) {}
		Annotation() : name("INVALID_ANNOTATION"), values(std::vector<std::string>()) {}
		std::string name;
		std::vector<std::string> values;
	};

	struct AnnotatedType {
		std::vector<Annotation> annotatedAttributes;
		bool hasAnnotation(std::string annotation) const {
			for (const Annotation& entry : annotatedAttributes) {
				if (entry.name == annotation)
					return true;
			}
			return false;
		}
		Annotation getAnnotation(std::string name) const {
			for (const Annotation& annotation : annotatedAttributes) {
				if (name == annotation.name) {
					return annotation;
				}
			}
			return Annotation();
		}
		std::vector<std::string> getAnnotationValue(std::size_t index) const {
			return annotatedAttributes[index].values;
		}
		std::vector<std::string> getAnnotationValue(std::string name) const {
			for (std::size_t i = 0; i < annotatedAttributes.size(); i++) {
				if (annotatedAttributes[i].name == name)
					return getAnnotationValue(i);
			}
			return std::vector<std::string>();
		}
	};

	struct ClassMember : public AnnotatedType {
		bool isStatic = false;
		AccessSpecifier accessSpecifier = AccessSpecifier::INVALID;
	};

	struct Property : public Variable, public ClassMember {
		template<typename T, typename ClassType>
		T getValue(ClassType* instance) {
			return instance->getValue_impl<T>(name);
		}
		template<typename T, typename ClassType>
		std::vector<T> getArrayValue(ClassType* instance) {
			return instance->getArrayValue_impl<T>(name);
		}
		template<typename T, typename ClassType>
		T getArrayElementValue(ClassType* instance, std::size_t index) {
			return instance->getArrayElementValue_impl<T>(name, index);
		}
		template<typename ClassType, typename T>
		void setValue(ClassType* instance, T value) {
			instance->setValue_impl(name, value);
		}
		template<typename T, std::size_t N, typename ClassType>
		void setArrayValue(ClassType* instance, T(&value)[N]) {
			instance->setArrayValue_impl<T, N>(name, value);
		}
		template<typename T, typename ClassType>
		void setArrayElementValue(ClassType* instance, std::size_t index, T value) {
			instance->setArrayElementValue_impl<T>(name, index, value);
		}
		template<typename ClassType>
		void* getPointer(ClassType* instance) {
			return instance->getPointer(name);
		}
		template<typename ClassType>
		std::vector<void*> getArrayElementPointers(ClassType* instance) {
			return instance->getArrayElementPointers(name);
		}
		template<typename ClassType>
		std::size_t getArraySize(ClassType* instance) {
			return instance->getArraySize(name);
		}
		template<typename ClassType>
		void onUpdate(ClassType* instance) {
			instance->onUpdate(name);
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
			for (std::size_t i = 0; i < parameters.size(); i++) {
				if (parameters[i] != other.parameters[i]) return false;
			}
			return true;
		}
	};

	struct Class : public Type, public AnnotatedType {
		std::vector<Property> properties;
		std::vector<Function> functions;
		std::vector<Type> baseClasses;
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
			Function invalid;
			invalid.name = "INVALID_FUNCTION";
			return invalid;
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
		bool isAbstract;
	};
}

#endif