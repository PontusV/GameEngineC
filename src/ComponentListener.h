#ifndef COMPONENT_LISTENER_H
#define COMPONENT_LISTENER_H

#include "ComponentEvent.h"
#include <vector>
#include "Entity.h"


/* Listens for updates in ComponentManager<T> and stores them as events for collection. Listens for updates on multiple components of same type. */
template <typename T>
class ComponentListener {
public:
	ComponentListener(bool main);
	ComponentListener();
	~ComponentListener();

	bool componentAdded(Entity entity, std::shared_ptr<T> component); //Gives a copy of the component and map it to the specified entity
	void componentRemoved(Entity entity); //Removes copy of component (removes the ComponentDataEntry)

	std::vector<ComponentEvent<T>>& getEvents();
	typedef T type;
private:
	bool main; //Determins if componentAdded(Entity, std::shared_ptr<T>) should be ignored. Dependencies of the main component are fetched instead
	std::vector<ComponentEvent<T>> events; //Collection of events sent from ComponentManager<T>
};

//------------------------------------------------------------
//---------------------IMPLEMENTATION-------------------------
//------------------------------------------------------------

#include "ComponentManager.h"
#include "ComponentDataManager.h"

template <typename T>
ComponentListener<T>::ComponentListener() {}

template <typename T>
ComponentListener<T>::ComponentListener(bool main) : main(main) {
	//Register in ComponentManager<T>
	ComponentManager<T>& manager = ComponentManager<T>::getInstance();
	manager.registerListener(this);
}

template <typename T>
ComponentListener<T>::~ComponentListener() {
	events.clear();
}
#include <iostream>
/* Creates a ComponentEvent (type: add) and stores it for collection. Makes a copy of the given component and places it in the ComponentEvent. */
template <typename T>
bool ComponentListener<T>::componentAdded(Entity entity, std::shared_ptr<T> component) {
	if (main)
		events.emplace_back(component, entity, ComponentEventType::add);
	return main;
}

/* Creates a ComponentEvent (type: remove) and stores it for collection. Makes a copy of the given component and places it in the ComponentEvent. */
template <typename T>
void ComponentListener<T>::componentRemoved(Entity entity) {
	events.emplace_back(entity, ComponentEventType::remove);
}

template <typename T>
std::vector<ComponentEvent<T>>& ComponentListener<T>::getEvents() {
	return events;
}
#endif