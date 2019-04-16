#include "ComponentRegistry.h"
#include "ComponentMetaInfo.h"
#include "ComponentLoader.h"
#include "ComponentType.h"
#include "ComponentTypeInfo.h"
#include <type_traits>
using namespace Core;


template<typename T>
typename std::enable_if_t<!std::is_abstract<T>::value && std::is_default_constructible<T>::value> registerToLoader() {
	ComponentLoader::registerComponent<T>(); // Registers Component in Loader
}
template<typename T> typename std::enable_if_t<std::is_abstract<T>::value || !std::is_default_constructible<T>::value> registerToLoader() {}

//--------------------------------------------Get Matching Component Types----------------------------------------------------------------
template<ComponentTypeID I, typename T>
std::vector<ComponentTypeID> getMatchingComponentTypeList(TypeList<>) {
	return std::vector<ComponentTypeID>(); // Empty vector
} // Ends match search
template<ComponentTypeID I, typename T, typename T2, typename... Ts>
std::vector<ComponentTypeID> getMatchingComponentTypeList(TypeList<T2, Ts...>) {
	std::vector<ComponentTypeID> derivedTypeIDList = getMatchingComponentTypeList<I + 1, T>(TypeList<Ts...>{});
	if (std::is_base_of<T, T2>::value) {
		// Append value to derivedTypeIDList
		derivedTypeIDList.push_back(I);
	}
	return derivedTypeIDList;
}

//-------------------------------------------Register Component Types Implementation-----------------------------------------------------------------
template<ComponentTypeID I=1, typename... Ts>
void registerComponentTypes_impl(TypeList<Ts...>) {} // End of registry
template<ComponentTypeID I=1, typename T, typename... Ts>
void registerComponentTypes_impl(TypeList<T, Ts...>) {
	// Check if base for any types in componentTypeList
	std::vector<ComponentTypeID> matchList = getMatchingComponentTypeList<1, T>(componentTypeList);

	// Set Component Type Info values
	ComponentTypeInfo<T>::setTypeID(I);
	ComponentTypeInfo<T>::setMatchingTypeIDs(matchList);
	registerToLoader<T>();
	//ComponentTypeInfo<T>::print(); // Debug information

	// Continue registry
	registerComponentTypes_impl<I+1>(TypeList<Ts...>{});
}

//------------------------------------------------------------------------------------------------------------
bool ComponentRegistry::registerComponentTypes() {
	registerComponentTypes_impl(componentTypeList);
	return true;
}