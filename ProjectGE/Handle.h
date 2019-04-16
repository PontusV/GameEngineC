#ifndef HANDLE_H
#define HANDLE_H

#include "Entity.h"
#include "EntityLocation.h"
#include "ComponentTypeInfo.h"
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
		/* Returns true if Handle is valid. If not, the handle is updated and returns true if the update was successful. */
		bool refresh();

		/* Updates the Handle so it points towards the target Entity. */
		void update();
		void updateLocation(EntityLocation location);
		const EntityLocation& getLocation() const;

		template<typename T> bool				hasComponent();

		template<typename T> T*					getComponent();
		Component*								getComponent(ComponentType type);
		Component*								getComponent(ComponentTypeID typeID);
		std::vector<Component*>					getComponents();
		template<typename T> std::vector<T*>	getComponents();
		std::vector<Component*>					getComponents(ComponentType type);

		template<typename T> T*					getComponentInChildren();
		std::vector<Component*>					getComponentsInChildren();
		template<typename T> std::vector<T*>	getComponentsInChildren();

		template<typename T> T*					getComponentInParent();
		std::vector<Component*>					getComponentsInParent();
		template<typename T> std::vector<T*>	getComponentsInParent();

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
	bool Handle::hasComponent() {
		return manager->hasComponent<T>(entity);
	}

	template<typename T>
	T* Handle::getComponent() {
		return static_cast<T*>(getComponent(typeof(T)));
	}

	template<typename T>
	std::vector<T*> Handle::getComponents() {
		std::vector<Component*> components = getComponents(typeof(T));
		std::vector<T*> castedPtrs; // Copying
		castedPtrs.reserve(components.size());
		for (Component* ptr : components) {
			castedPtrs.push_back(static_cast<T*>(ptr));
		}
		return castedPtrs;
	}

	template<typename T>
	T* Handle::getComponentInChildren() {
		std::size_t childCount = getChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle* child = getChild(i);
			if (T* ptr = child->getComponent<T>()) // Check if child has the component
				return ptr;
		}
	}

	template<typename T>
	std::vector<T*> Handle::getComponentsInChildren() {
		std::vector<T*> components;
		if (refresh()) {
			std::size_t childCount = getChildCount();
			for (std::size_t i = 0; i < childCount; i++) {
				Handle* child = getChild(i);
				std::vector<T*> childComponents = child->getComponents<T>();
				components.insert(components.end(), childComponents.begin(), childComponents.end());
			}
		}
		return components;
	}

	template<typename T>
	T* Handle::getComponentInParent() {
		if (refresh()) {
			Handle* parent = getParent();
			while (parent) {
				if (T* ptr = parent->getComponent<T>()) // Check if parent has the component
					return ptr;

				parent = parent->getParent(); // Check next parent
			}
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*> Handle::getComponentsInParent() {
		std::vector<Component*> components;
		if (refresh()) {
			Handle* parent = getParent();
			if (parent) {
				std::vector<Component*> parentComponents = parent->getComponents<T>();
				components.insert(components.end(), parentComponents.begin(), parentComponents.end());
				parentComponents = parent->getComponentsInParent();
				components.insert(components.end(), parentComponents.begin(), parentComponents.end());
			}
		}
		return components;
	}
}
#endif