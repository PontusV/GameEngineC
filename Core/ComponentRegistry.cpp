#include "Component.h"
#include "ComponentRegistry.h"
#include "ComponentLoader.h"
#include "ComponentType.h"
#include "ComponentTypeInfo.h"
#include "ReflectionPolymorph.generated.h"
#include <type_traits>
using namespace Core;


template<typename T>
typename std::enable_if_t<!std::is_abstract<T>::value && std::is_default_constructible<T>::value> registerToLoader() {
	ComponentLoader::registerComponent<T>(); // Registers Component in Loader
}
template<typename T> typename std::enable_if_t<std::is_abstract<T>::value || !std::is_default_constructible<T>::value> registerToLoader() {}

//--------------------------------------------Get Matching Component Types----------------------------------------------------------------
template<typename T>
std::vector<ComponentTypeID> getMatchingComponentTypeList() {
	ComponentTypeID typeID = T::getTypeID();
	std::vector<ComponentTypeID> derivedTypeIDs = Mirror::polyGetDerivedTypeIDs(typeID);
	derivedTypeIDs.insert(derivedTypeIDs.begin(), typeID);
	return derivedTypeIDs;
}

//-------------------------------------------Register Component Types Implementation-----------------------------------------------------------------
template<typename... Ts>
void registerComponentTypes_impl(Mirror::TypeList<Ts...>) { std::cout << "LOL" << std::endl; } // End of registry

template<typename T, typename... Ts>
typename std::enable_if_t<std::is_base_of<Component, T>::value || std::is_same<Component, T>::value, void> registerComponentTypes_impl(Mirror::TypeList<T, Ts...>) {
	std::cout << "true" << std::endl;
	// Check if base for any types in componentTypeList
	std::vector<ComponentTypeID> matchList = getMatchingComponentTypeList<T>();

	// Set Component Type Info values
	ComponentTypeInfo<T>::setTypeID(T::getTypeID());
	ComponentTypeInfo<T>::setMatchingTypeIDs(matchList);
	registerToLoader<T>();
	ComponentTypeInfo<T>::print(); // Debug information

	// Continue registry
	registerComponentTypes_impl(Mirror::TypeList<Ts...>{});
}

template<typename T, typename... Ts>
typename std::enable_if_t<!std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value, void> registerComponentTypes_impl(Mirror::TypeList<T, Ts...>) {
	std::cout << "false" << sizeof...(Ts) << std::endl;
	// Continue registry
	registerComponentTypes_impl(Mirror::TypeList<Ts...>{});
}

//------------------------------------------------------------------------------------------------------------
bool ComponentRegistry::registerComponentTypes() {
	registerComponentTypes_impl(Mirror::ReflectedTypes{});
	return true;
}