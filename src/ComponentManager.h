#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <vector>
#include <map>
#include <memory>
#include "Entity.h"
#include "Handle.h"

template <typename T> class ComponentListener;
class HandleManager;
//template <typename T> class ComponentListener;

/* Management of a single component type */
template <typename T>
class ComponentManager {
public:
	static ComponentManager<T>& getInstance() {
		static ComponentManager<T> instance;
		return instance;
	}
	ComponentManager(ComponentManager const&) = delete;
	void operator=(ComponentManager const&) = delete;
	ComponentManager();
	~ComponentManager();

	void add(Entity& entity, std::shared_ptr<T> component, HandleManager& handleManager);
	void remove(Entity& entity, HandleManager& handleManager);
	void update(Entity& entity, T& component, HandleManager& handleManager);
	Handle get(Entity& entity);
	T* fetch(Entity& entity, HandleManager& handleManager);

	void registerListener(ComponentListener<T>* listener);
private:
	std::vector<std::shared_ptr<T>> unclaimed; //Collection of unclaimed components. If there are no listeners for T the components will be stored here before they get fetched.
	std::vector<ComponentListener<T>*> listeners;
	std::map<Entity, std::vector<Handle>> entityMap;

	std::map<Entity, int> unclaimedMap;
	std::map<int, Entity> unclaimedIndexMap;

	//Help
	void mapEntity(Entity& entity, Handle componentHandle);
	void removeUnclaimed(int index);
};

//------------------------------------------------------------
//---------------------IMPLEMENTATION-------------------------
//------------------------------------------------------------

#include "HandleManager.h"
#include "ComponentListener.h"

//Definitions
template<typename T>
ComponentManager<T>::ComponentManager() {}

template<typename T>
ComponentManager<T>::~ComponentManager() {
	unclaimed.clear();
}

/* Sends copies of the given component to all listeners. */
template<typename T>
void ComponentManager<T>::add(Entity& entity, std::shared_ptr<T> component, HandleManager& handleManager) {
	Handle componentID = handleManager.add(component.get(), T::TYPE_ID);
	component->setID(componentID);
	mapEntity(entity, componentID);

	bool componentAdded = false;
	if (listeners.size() > 0) { //If there are listeners
		for (ComponentListener<T>* listener : listeners) {
			if (listener->componentAdded(entity, component))
				componentAdded = true;
		}
		if (componentAdded) //If a main listener was found do not add component to unclaimed map
			return;
	}
	//No main listeners
	unclaimedMap.insert(std::pair<Entity, int>(entity, unclaimed.size()));
	unclaimedIndexMap.insert(std::pair<int, Entity>(unclaimed.size(), entity));
	unclaimed.push_back(component);
}

/* Removes all copies of the component mapped to the given entity in the listeners */
template<typename T>
void ComponentManager<T>::remove(Entity& entity, HandleManager& handleManager) {
	if (!entityMap.count(entity))
		throw std::invalid_argument(std::string("Unable to remove component. There was no component of type ") + typeid(T).name() + " mapped to the entity!");

	if (unclaimedMap.count(entity)) {
		int index = unclaimedMap.at(entity);
		removeUnclaimed(index);
	}
	else {
		for (ComponentListener<T>* listener : listeners)
			listener->componentRemoved(entity);
	}
	//Remove handles pointed towards removed components
	std::vector<Handle>& cmpHandles = entityMap.at(entity);
	for (Handle handle : cmpHandles) {
		handleManager.remove(handle);
	}
	//Remove entity map to removed component
	entityMap.erase(entity);
}

/* Updates values of all copies of the component to the values of the given one */
template<typename T>
void ComponentManager<T>::update(Entity& entity, T& component, HandleManager& handleManager) {
	if (!entityMap.count(entity))
		throw std::invalid_argument(std::string("Unable to update component. There was no component of type ") + typeid(T).name() + " mapped to the entity!");

	std::vector<Handle> handles = entityMap.at(entity);
	for (Handle handle : handles) {
		//Update components
		T& cmp = *handleManager.get<T>(handle);
		cmp = component;
	}
}

/* Retrieves and returns a handle to the first copy of component mapped to the given entity */
template<typename T>
Handle ComponentManager<T>::get(Entity& entity) {
	if (!entityMap.count(entity))
		throw std::invalid_argument(std::string("Could not get component. There was no component of type ") + typeid(T).name() + " mapped to the entity!");

	std::vector<Handle>& cmpHandles = entityMap.at(entity);
	return cmpHandles[0];
}

/* Retrieves and returns a copy of the component mapped to the given entity. */
template<typename T>
T* ComponentManager<T>::fetch(Entity& entity, HandleManager& handleManager) {
	if (unclaimedMap.count(entity)) { //Claim unclaimed component
		//Get component
		int index = unclaimedMap.at(entity);
		T* component = unclaimed[index].get();
		removeUnclaimed(index);

		return component;
	}
	Handle cmpHandle = get(entity);
	T* cmp = handleManager.get<T>(cmpHandle);
	T* component = new T(std::move(*cmp));
	//Give new ID
	Handle componentID = handleManager.add(component, T::TYPE_ID);
	component->setID(componentID);
	mapEntity(entity, componentID);
	//
	return component;
}

/* Adds listener to the list. */
template<typename T>
void ComponentManager<T>::registerListener(ComponentListener<T>* listener) {
	listeners.push_back(listener);
}

template<typename T>
void ComponentManager<T>::mapEntity(Entity& entity, Handle componentHandle) {
	if (entityMap.count(entity)) { //If entity is already in map
		std::vector<Handle>& handles = entityMap.at(entity);
		handles.push_back(componentHandle);
	}
	else { //Insert new key value pair
		std::vector<Handle> vec;
		vec.push_back(componentHandle);
		entityMap.insert(std::pair<Entity, std::vector<Handle>>(entity, vec));
	}
}

template<typename T>
void ComponentManager<T>::removeUnclaimed(int index) {
	Entity& entity = unclaimedIndexMap[index];
	//Remove claimed element from unclaimed collection
	std::swap(unclaimed[index], unclaimed[unclaimed.size() - 1]);
	unclaimed.pop_back();
	//Re-map swapped component and erase old map entries
	unclaimedIndexMap[index] = unclaimedIndexMap[unclaimed.size()]; //Map entity to index <index, entity>
	unclaimedIndexMap.erase(unclaimed.size());
	unclaimedMap[unclaimedIndexMap[index]] = index; //Map index to entity <entity, index>. Gets entity from indexmap
	unclaimedMap.erase(entity);
}
#endif