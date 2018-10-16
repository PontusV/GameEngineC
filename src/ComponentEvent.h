#ifndef COMPONENT_EVENT_H
#define COMPONENT_EVENT_H
#include <memory>

enum class ComponentEventType {
	add,
	remove
};

class Entity;

template<typename T>
class ComponentEvent {
public:
	ComponentEvent(std::shared_ptr<T> component, Entity entity, ComponentEventType type);
	ComponentEvent(Entity entity, ComponentEventType type);
	~ComponentEvent();

	ComponentEventType type;
	std::shared_ptr<T> component;
	Entity entity;
};

//------------------------------------------------------------
//---------------------IMPLEMENTATION-------------------------
//------------------------------------------------------------

//Definitions
template<typename T>
ComponentEvent<T>::ComponentEvent(std::shared_ptr<T> component, Entity entity, ComponentEventType type) : component(component), entity(entity), type(type) {}

template<typename T>
ComponentEvent<T>::ComponentEvent(Entity entity, ComponentEventType type) : entity(entity), type(type) {}

template<typename T>
ComponentEvent<T>::~ComponentEvent() {}
#endif