#ifndef REFLECTION_PARSER
#pragma once
#include <ReflectionParser/ReflectionTypes.h>
#include <ReflectionParser/TypeList.h>
#include <stdexcept>

#include "G:/source/repos/CppMirror/ReflectionExample/Header2.h"
#include "G:/source/repos/CppMirror/ReflectionExample/Header.h"

namespace Mirror {
template<typename ClassType>
inline ClassType* createInstance(std::size_t typeID, void* dest) {
	if (typeID == 1) {
		return new(dest) Foo2();
	}
	else if (typeID == 2) {
		return new(dest) Test::Sup();
	}
	throw std::invalid_argument("createInstance::ERROR Unable to create instance of unknown class");
}
inline std::string getName(std::size_t typeID) {
	if (typeID == 1) {
		return "Foo2";
	}
	else if (typeID == 2) {
		return "Test::Sup";
	}
	throw std::invalid_argument("getName::ERROR Unknown class");
}
inline std::size_t getSize(std::size_t typeID) {
	if (typeID == 1) {
		return sizeof(Foo2);
	}
	else if (typeID == 2) {
		return sizeof(Test::Sup);
	}
	throw std::invalid_argument("getName::ERROR Unknown class");
}
inline std::vector<std::size_t> polyGetDerivedTypeIDs(std::size_t typeID) {
	if (typeID == 1) {
		return std::vector<std::size_t>{};
	}
	else if (typeID == 2) {
		return std::vector<std::size_t>{};
	}
	return std::vector<std::size_t>{};
}
/* Converts the given pointer to the derived type. Calls invoke on the function instance with the casted pointer. */
template<typename ClassType, typename... Args>
void polyInvoke(Mirror::Function fun, ClassType* instance, std::size_t typeID, Args... args) {
	if (typeID == 1)
		return fun.invoke(reinterpret_cast<Foo2*>(instance), args...);
	else if (typeID == 2)
		return fun.invoke(reinterpret_cast<Test::Sup*>(instance), args...);
	throw std::invalid_argument("polyInvoke::ERROR");
}
template<typename T, typename ClassType>
T polyGetValue(Mirror::Property prop, ClassType* instance, std::size_t typeID) {
	if (typeID == 1)
		return prop.getValue<T>(reinterpret_cast<Foo2*>(instance));
	else if (typeID == 2)
		return prop.getValue<T>(reinterpret_cast<Test::Sup*>(instance));
	throw std::invalid_argument("polyGetValue::ERROR");
}
template<typename T, typename ClassType>
std::vector<T> polyGetArrayValue(Mirror::Property prop, ClassType* instance, std::size_t typeID) {
	if (typeID == 1)
		return prop.getArrayValue<T>(reinterpret_cast<Foo2*>(instance));
	else if (typeID == 2)
		return prop.getArrayValue<T>(reinterpret_cast<Test::Sup*>(instance));
	throw std::invalid_argument("polyGetArrayValue::ERROR");
}
template<typename T, typename ClassType>
T polyGetArrayElementValue(Mirror::Property prop, std::size_t index, ClassType* instance, std::size_t typeID) {
	if (typeID == 1)
		return prop.getArrayElementValue<T>(reinterpret_cast<Foo2*>(instance), index);
	else if (typeID == 2)
		return prop.getArrayElementValue<T>(reinterpret_cast<Test::Sup*>(instance), index);
	throw std::invalid_argument("polyGetArrayElementValue::ERROR");
}
template<typename T, typename ClassType>
void polySetValue(Mirror::Property prop, ClassType* instance, std::size_t typeID, T value) {
	if (typeID == 1)
		return prop.setValue(reinterpret_cast<Foo2*>(instance), value);
	else if (typeID == 2)
		return prop.setValue(reinterpret_cast<Test::Sup*>(instance), value);
	throw std::invalid_argument("polySetValue::ERROR");
}
template<typename T, std::size_t N, typename ClassType>
void polySetArrayValue(Mirror::Property prop, ClassType* instance, std::size_t typeID, T(&value)[N]) {
	if (typeID == 1)
		return prop.setArrayValue<T, N>(reinterpret_cast<Foo2*>(instance), value);
	else if (typeID == 2)
		return prop.setArrayValue<T, N>(reinterpret_cast<Test::Sup*>(instance), value);
	throw std::invalid_argument("polySetArrayValue::ERROR");
}
template<typename T, typename ClassType>
void polySetArrayElementValue(Mirror::Property prop, std::size_t index, ClassType* instance, std::size_t typeID, T value) {
	if (typeID == 1)
		return prop.setArrayElementValue(reinterpret_cast<Foo2*>(instance), index, value);
	else if (typeID == 2)
		return prop.setArrayElementValue(reinterpret_cast<Test::Sup*>(instance), index, value);
	throw std::invalid_argument("polySetValue::ERROR");
}
inline std::size_t getArraySize(Mirror::Property prop, void* instance, std::size_t typeID) {
	if (typeID == 1)
		return prop.getArraySize(reinterpret_cast<Foo2*>(instance));
	else if (typeID == 2)
		return prop.getArraySize(reinterpret_cast<Test::Sup*>(instance));
	throw std::invalid_argument("getArraySize::ERROR");
}
inline void* getPointer(Mirror::Property prop, void* instance, std::size_t typeID) {
	if (typeID == 1)
		return prop.getPointer(reinterpret_cast<Foo2*>(instance));
	else if (typeID == 2)
		return prop.getPointer(reinterpret_cast<Test::Sup*>(instance));
	throw std::invalid_argument("getPointer::ERROR");
}
inline std::vector<void*> getArrayElementPointers(Mirror::Property prop, void* instance, std::size_t typeID) {
	if (typeID == 1)
		return prop.getArrayElementPointers(reinterpret_cast<Foo2*>(instance));
	else if (typeID == 2)
		return prop.getArrayElementPointers(reinterpret_cast<Test::Sup*>(instance));
	throw std::invalid_argument("getArrayElementPointers::ERROR");
}
namespace {
	template<typename T>
	std::enable_if_t<std::is_copy_constructible<T>::value, T*> memcpyImpl(void* placement, T* instance) {
		return new (placement) T(*instance);
	}
	template<typename T>
	std::enable_if_t<!std::is_copy_constructible<T>::value, T*> memcpyImpl(void* placement, T* instance) {
		throw std::invalid_argument("memcpy::ERROR typeID pointing to non copy constructible class");
	}
}
/* Places a new copy of the given instance to the given location in memory. Makes use of the copy constructor of the type assigned to the given typeID. */
inline void* memcpy(void* placement, void* instance, std::size_t typeID) {
	switch(typeID) {
		case 1:
			return memcpyImpl(placement, static_cast<Foo2*>(instance));
		case 2:
			return memcpyImpl(placement, static_cast<Test::Sup*>(instance));
		default:
			throw std::invalid_argument("memcpy::ERROR Invalid typeID");
	}
}
inline void serialize(void* instance, std::size_t typeID, std::ostream& os) {
	if (typeID == 1)
		return reinterpret_cast<Foo2*>(instance)->serialize(os);
	else if (typeID == 2)
		return reinterpret_cast<Test::Sup*>(instance)->serialize(os);
	throw std::invalid_argument("serialize::ERROR");
}
inline void deserialize(void* instance, std::size_t typeID, std::istream& is) {
	if (typeID == 1)
		return reinterpret_cast<Foo2*>(instance)->deserialize(is);
	else if (typeID == 2)
		return reinterpret_cast<Test::Sup*>(instance)->deserialize(is);
	throw std::invalid_argument("deserialize::ERROR");
}
inline void onUpdate(void* instance, std::size_t typeID, std::string propName) {
	if (typeID == 1)
		return reinterpret_cast<Foo2*>(instance)->onUpdate(propName);
	if (typeID == 2)
		return reinterpret_cast<Test::Sup*>(instance)->onUpdate(propName);
	throw std::invalid_argument("onUpdate::ERROR");
}
inline bool isReflected(std::string className) {
	if (className == "Foo2")
		return true;
	if (className == "Test::Sup")
		return true;
	return false;
}
inline std::size_t getTypeID(std::string className) {
	if (className == "Foo2")
		return 1;
	if (className == "Test::Sup")
		return 2;
	return 0;
}
inline Mirror::Class getType(std::size_t typeID) {
	if (typeID == "1")
		return Foo2::getClassType();
	if (typeID == "2")
		return Test::Sup::getClassType();
	throw std::invalid_argument("Mirror::getType::ERROR There is no such reflected class!");
}
inline Mirror::Class getType(std::string className) {
	if (className == "Foo2")
		return Foo2::getClassType();
	if (className == "Test::Sup")
		return Test::Sup::getClassType();
	throw std::invalid_argument("Mirror::getType::ERROR There is no such reflected class!");
}
inline bool isArrayType(const Mirror::VariableType& type) {
	if (type.isArray) return true;
	if (type.name == "std::vector")
		return true;
	if (type.name == "std::array")
		return true;
	return false;
}
typedef TypeList<Foo2,Test::Sup> ReflectedTypes;
}

#endif