#ifndef COMPONENT_DATA_H
#define COMPONENT_DATA_H

#include <tuple>
#include <vector>

class HandleManager;

/* Contains multiple components. Updates memory location of containing components until deletion; where it is removed from handleManager. */
template<std::size_t MAX_ENTRIES, typename... ComponentTypes>
class ComponentData
{
public:
	ComponentData(HandleManager& handleManager);
	~ComponentData();

	void clear();
	int size();
	int add(std::tuple<ComponentTypes...> entry); //Similar to std::vector::push_back();
	void remove(int index);
	std::tuple<ComponentTypes...>& get(int index);
	//Optional: Get iterator

	inline std::tuple<ComponentTypes...>& operator[](int index);
private:
	//ComponentGroup<ComponentTypes...> data[];
	std::vector<std::tuple<ComponentTypes...>> data;
	HandleManager& handleManager;
};

//------------------------------------------------------------
//---------------------IMPLEMENTATION-------------------------
//------------------------------------------------------------
#include "HandleManager.h"
#include <stdexcept>

namespace {
	template<std::size_t I = 0, class F, class... Ts>
	inline typename std::enable_if<I == sizeof...(Ts), void>::type
		for_each(std::tuple<Ts...>& tuple, F func) {}

	template<std::size_t I = 0, class F, class... Ts>
	inline typename std::enable_if<I < sizeof...(Ts), void>::type
		for_each(std::tuple<Ts...>& tuple, F func) {
		func(std::get<I>(tuple));
		for_each<I + 1, F, Ts...>(tuple, func);
	}
}
template<std::size_t MAX_ENTRIES, typename... ComponentTypes>
ComponentData<MAX_ENTRIES, ComponentTypes...>::ComponentData(HandleManager& handleManager) : handleManager(handleManager) {
	data.reserve(MAX_ENTRIES);
}


template<std::size_t MAX_ENTRIES, typename... ComponentTypes>
ComponentData<MAX_ENTRIES, ComponentTypes...>::~ComponentData()
{
	clear();
}

/* Deletes all active elements from memory and sets size to 0. */
template<std::size_t MAX_ENTRIES, typename... ComponentTypes>
void ComponentData<MAX_ENTRIES, ComponentTypes...>::clear() {
	data.clear();
}

/* Returns the number of active entries. */
template<std::size_t MAX_ENTRIES, typename... ComponentTypes>
int ComponentData<MAX_ENTRIES, ComponentTypes...>::size() {
	return data.size();
}
/*	Removes entry in given index from array. Deletes the entry from memory.
Switches place with entry at index and at the back. Updates handleManager with pointer to the entry that was previously at the back.
*/
template<std::size_t MAX_ENTRIES, typename... ComponentTypes>
void ComponentData<MAX_ENTRIES, ComponentTypes...>::remove(int index) {
	//Remove Handles of removed component copies
	for_each(data[index], [&](auto element) {
		Handle id = element.getID();
		handleManager.remove(id);
	});
	std::swap(data[index], data[size()-1]); //Swap, Moves element to be removed to back
	data.pop_back(); //Clear memory

	//Update HandleManager with new pointer to swapped entry (entry previously at back)
	for_each(data[index], [&](auto element) {
		Handle id = element.getID();
		handleManager.update(id, &element);
	});
}

/* Adds the given element to the end of the active entries of the array. */
template<std::size_t MAX_ENTRIES, typename... ComponentTypes>
int ComponentData<MAX_ENTRIES, ComponentTypes...>::add(std::tuple<ComponentTypes...> entry) {
	if (size() >= MAX_ENTRIES)
		throw std::length_error("Cannot add another component in ComponentData. ComponentData is already at max size!");

	int index = size();
	data.push_back(entry);

	//Update HandleManager with new pointers for handles contained in the given components
	for_each(data[index], [&](auto& element) {
		Handle id = element.getID();
		handleManager.update(id, &element);
	});
	return index;
}

template<std::size_t MAX_ENTRIES, typename... ComponentTypes>
std::tuple<ComponentTypes...>& ComponentData<MAX_ENTRIES, ComponentTypes...>::get(int index) {
	return data[index];
}

template<std::size_t MAX_ENTRIES, typename... ComponentTypes>
inline std::tuple<ComponentTypes...>& ComponentData<MAX_ENTRIES, ComponentTypes...>::operator[](int index) {
	return data[index];
}
#endif