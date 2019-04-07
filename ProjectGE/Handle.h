#ifndef HANDLE_H
#define HANDLE_H

#include "Entity.h"
#include "EntityLocation.h"
#include <vector>

typedef std::size_t ComponentTypeID;

namespace Core {

	class EntityManager; // Forward declare
	class Component;

	/* A handle for Entities and Components */
	class Handle {
	public:
		Handle(Entity entity, EntityManager* manager);
		Handle(); // Invalid handle constructor
		virtual ~Handle();

		const Entity& getEntity() const;

		/* Checks if the handle is still pointing to the same Entity. */
		bool isValid();

		/* Updates the Handle so it points towards the target Entity. */
		void update();
		void updateLocation(EntityLocation location);
		const EntityLocation& getLocation() const;

		template<typename T> T*					getComponent();
		std::vector<Component*>					getComponents();
		template<typename T> std::vector<T*>	getComponents();

		template<typename T> T*					getComponentInChildren();
		std::vector<Component*>					getComponentsInChildren();
		template<typename T> std::vector<T*>	getComponentsInChildren();

		template<typename T> T*					getComponentInParent();
		std::vector<Component*>					getComponentsInParent();
		template<typename T> std::vector<T*>	getComponentsInParent();

		/* Creates a new vector containing all the components from the paramater who were deriving from T. */
		template<typename T> std::vector<T*>	filterComponents(std::vector<Component*> components);
		/* Returns nullptr if no match was found. */
		Component*								getFirstDerivingComponent(std::vector<Component*> components, ComponentTypeID typeID);

		Handle*									getParent();
		Handle*									getChild(std::size_t index);
		std::size_t								getChildCount();
	protected:
		// Data required to update itself
		EntityManager* manager;
		Entity entity;

	private:
		EntityLocation locationData;
	};

	template<typename T>
	T* Handle::getComponent() {
		std::vector<Component*> components = getComponents();
		return static_cast<T*>(getFirstDerivingComponent(components, T::TYPE_ID));
	}

	template<typename T>
	std::vector<T*> Handle::getComponents() {
		std::vector<Component*> components = getComponents();
		return filterComponents<T>(components);
	}

	template<typename T>
	T* Handle::getComponentInChildren() {
		std::vector<Component*> components = getComponentsInChildren();
		return static_cast<T*>(getFirstDerivingComponent(components, T::TYPE_ID));
	}

	template<typename T>
	std::vector<T*> Handle::getComponentsInChildren() {
		std::vector<Component*> components = getComponentsInChildren();
		return filterComponents<T>(components);
	}

	template<typename T>
	T* Handle::getComponentInParent() {
		std::vector<Component*> components = getComponentsInParent();
		return static_cast<T*>(getFirstDerivingComponent(components, T::TYPE_ID));
	}

	template<typename T>
	std::vector<T*> Handle::getComponentsInParent() {
		std::vector<Component*> components = getComponentsInParent();
		return filterComponents<T>(components);
	}

	template<typename T>
	std::vector<T*> Handle::filterComponents(std::vector<Component*> components) {
		std::vector<T*> componentsWithEqualType; // Fill collection with components deriving from T
		// Filter out components that do not derive from T
		for (Component* component : components) {
			// Push components with same type as T
			if (component->equalType(T::TYPE_ID)) {
				componentsWithEqualType.push_back(static_cast<T*>(component));
			}
		}
		return componentsWithEqualType;
	}
}
#endif