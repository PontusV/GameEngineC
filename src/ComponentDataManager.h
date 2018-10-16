#ifndef COMPONENT_DATA_MANAGER_H
#define COMPONENT_DATA_MANAGER_H

#include "ComponentData.h"
#include <tuple>
#include <vector>
#include <map>
#include <memory>
#include "ComponentEvent.h"
#include "ComponentListener.h"
#include "Entity.h"

class HandleManager;

/* Manages component data and stores a number of components in a cache friendly manner for cases when every iteration needs data from all specified components. */
template<std::size_t MAX_ENTRIES, typename T, typename... ComponentDependecies>
class ComponentDataManager
{
public:
	ComponentDataManager(HandleManager& handleManager);
	~ComponentDataManager();

	void update();
	ComponentData<MAX_ENTRIES, T, ComponentDependecies...>& getAll();

private:
	ComponentData<MAX_ENTRIES, T, ComponentDependecies...> componentData;
	std::tuple<ComponentListener<T>*, ComponentListener<ComponentDependecies>*...> listeners;
	std::map<Entity, int> entityMap; //Entity, ComponentDataIndex
	std::vector<Entity> indexMap;
	HandleManager& handleManager;

	void add(Entity entity, std::shared_ptr<T> component);
	void remove(Entity entity);
	template<typename Ty> void processMainEvent(ComponentEvent<Ty>& cmpEvent);
	template<typename Ty> void processEvent(ComponentEvent<Ty>& cmpEvent);
};

//------------------------------------------------------------
//---------------------IMPLEMENTATION-------------------------
//------------------------------------------------------------

#include "ComponentManager.h"
#include <type_traits>

//Definitions
template<std::size_t MAX_ENTRIES, typename T, typename... ComponentTypes>
ComponentDataManager<MAX_ENTRIES, T, ComponentTypes...>::ComponentDataManager(HandleManager& handleManager) : handleManager(handleManager), componentData(handleManager), listeners(std::make_tuple(new ComponentListener<T>(true), new ComponentListener<ComponentTypes>(false)...))
{
}

template<std::size_t MAX_ENTRIES, typename T, typename... ComponentTypes>
ComponentDataManager<MAX_ENTRIES, T, ComponentTypes...>::~ComponentDataManager()
{
	componentData.clear();
	//Deletes listeners
	for_each(listeners, [](auto element) {
		delete element;
	});
}

template<std::size_t MAX_ENTRIES, typename T, typename... ComponentTypes>
ComponentData<MAX_ENTRIES, T, ComponentTypes...>& ComponentDataManager<MAX_ENTRIES, T, ComponentTypes...>::getAll() {
	return componentData;
}

/* Retrieves and process ComponentEvents from all its listeners. */
template<std::size_t MAX_ENTRIES, typename T, typename... ComponentTypes>
void ComponentDataManager<MAX_ENTRIES, T, ComponentTypes...>::update() {
	ComponentListener<T>* mListener = std::get<0>(listeners);
	std::vector<ComponentEvent<T>>& mEvents = mListener->getEvents();
	for (ComponentEvent<T> cmpEvent : mEvents)
		processMainEvent(cmpEvent);
	mEvents.clear();

	//Loop through all listeners
	for_each<1>(listeners, [&](auto listener) {
		typedef std::remove_pointer<decltype(listener)>::type::type ComponentType;
		std::vector<ComponentEvent<ComponentType>>& events = listener->getEvents();
		for (ComponentEvent<ComponentType>& cmpEvent : events) {
			processEvent(cmpEvent);
		}
		events.clear();
	});
}

/* Process events of main ComponentListener. Looks for events unique for main ComponentListener and after sends the event to be processed by the regular event processor. */
template<std::size_t MAX_ENTRIES, typename T, typename... ComponentTypes>
template<typename Ty>
void ComponentDataManager<MAX_ENTRIES, T, ComponentTypes...>::processMainEvent(ComponentEvent<Ty>& cmpEvent) {
		if (cmpEvent.type == ComponentEventType::add) {
			add(cmpEvent.entity, cmpEvent.component);
		}
		else {
			processEvent(cmpEvent);
		}
}

/* Process events of regular ComponentListener */
template<std::size_t MAX_ENTRIES, typename T, typename... ComponentTypes>
template<typename Ty>
void ComponentDataManager<MAX_ENTRIES, T, ComponentTypes...>::processEvent(ComponentEvent<Ty>& cmpEvent) {
	if (cmpEvent.type == ComponentEventType::remove) {
		remove(cmpEvent.entity);
	}
}

/* Fetches all component dependencies from ComponentManagers mapped to entity and use them together with given component to create a ComponentData entry. */
template<std::size_t MAX_ENTRIES, typename T, typename... ComponentDependecies>
void ComponentDataManager<MAX_ENTRIES, T, ComponentDependecies...>::add(Entity entity, std::shared_ptr<T> component) {
	//Fetch all component dependencies from ComponentManagers and create new entry in componentData
	std::tuple<T, ComponentDependecies...> dataEntry = std::make_tuple(*component, *ComponentManager<ComponentDependecies>::getInstance().fetch(entity, handleManager)...);
	int index = componentData.add(dataEntry);

	//Map entity to ComponentData entry
	entityMap.insert(std::pair<Entity, int>(entity, index));
	indexMap.push_back(entity);
}

/* Removes ComponentData entry mapped to Entity. */
template<std::size_t MAX_ENTRIES, typename T, typename... ComponentTypes>
void ComponentDataManager<MAX_ENTRIES, T, ComponentTypes...>::remove(Entity entity) {
	int index = entityMap.at(entity);
	componentData.remove(index);

	//Update entityMap & indexMap
	if (index != componentData.size()) { //Is deleted entity at back? If not, do this
		Entity& newEntity = indexMap[componentData.size()]; //Entity at back of list
		std::swap(entityMap.at(newEntity), index); //Map entity to new index
		std::swap(indexMap[index], newEntity); //Map index to new entity
	}
	entityMap.erase(entity); //Remove deleted entity from map
	indexMap.pop_back();
}
#endif