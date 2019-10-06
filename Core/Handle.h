#ifndef HANDLE_H
#define HANDLE_H

#include "Entity.h"
#include "EntityLocation.h"
#include "ComponentTypeInfo.h"
#include "ObjectType.h"
#include <vector>

typedef std::size_t ComponentTypeID;

namespace Core {

	class Scene;
	class Component;

	/* A handle for Entities */
	class Handle {
	public:
		Handle(Entity entity, Scene* scene);
		Handle(); // Invalid handle constructor
		virtual ~Handle();
		bool operator==(const Handle& other);
		bool operator!=(const Handle& other);
		/* Returns true if the given Entity is a child of the Entity this instance points to. */
		bool isChild(Entity entity);
		/* Returns true if the given Entity is an immediate child of the Entity this instance points to. */
		bool isImmediateChild(Entity entity);

		const Entity& getEntity() const;

		/* Checks if the handle is still pointing to the same Entity. */
		bool isValid();
		/* Returns true if Handle is valid. If not, the handle is updated and returns true if the update was successful. */
		bool refresh();

		/* Updates the Handle so it points towards the target Entity. */
		void update();
		void updateLocation(EntityLocation location);
		const EntityLocation& getLocation() const;

		template<typename T>
		bool hasComponent();
		bool hasComponent(ComponentType type);

		template<typename T>
		T* getComponent();
		Component* getComponent(ComponentType type);
		Component* getComponent(ComponentTypeID typeID);
		std::vector<Component*> getComponents();
		template<typename T>
		std::vector<T*> getComponents();
		std::vector<Component*> getComponents(ComponentType type);

		template<typename T>
		T* getComponentInChildren();
		std::vector<Component*> getComponentsInChildren();
		template<typename T>
		std::vector<T*> getComponentsInChildren();

		template<typename T>
		T* getComponentInImmediateChildren();
		std::vector<Component*> getComponentsInImmediateChildren();
		template<typename T>
		std::vector<T*> getComponentsInImmediateChildren();

		template<typename T>
		T* getComponentInParents();
		std::vector<Component*> getComponentsInParents();
		template<typename T>
		std::vector<T*> getComponentsInParents();

		/* Returns all components attached to the entity and its parents. */
		std::vector<Component*> getComponentsUpwards();
		/* Returns all components (deriving from T) attached to the entity and its parents. */
		template<typename T>
		std::vector<T*> getComponentsUpwards();

		/* Returns all components attached to the entity, its children and the childrens children, etc... */
		std::vector<Component*> getComponentsDownwards();
		/* Returns all components (deriving from T) attached to the entity and its parent and the parent of the parent, etc... */
		template<typename T>
		std::vector<T*> getComponentsDownwards();

		ObjectType getObjectType();

		Handle getParent();
		Handle getChild(std::size_t index);
		std::size_t getImmediateChildCount();
		std::size_t getChildCount();

	protected:
		// Data required to update itself
		Scene* scene = nullptr;
		Entity entity = Entity(0);

	private:
		EntityLocation locationData = EntityLocation();
	};
	template<typename T>
	bool Handle::hasComponent() {
		return hasComponent(typeof(T));
	}

	template<typename T>
	T* Handle::getComponent() {
		return (T*)getComponent(typeof(T));
	}

	template<typename T>
	std::vector<T*> Handle::getComponents() {
		std::vector<Component*> components = getComponents(typeof(T));
		std::vector<T*> castedPtrs; // Copying
		castedPtrs.reserve(components.size());
		for (Component* ptr : components) {
			castedPtrs.push_back((T*)ptr);
		}
		return castedPtrs;
	}

	template<typename T>
	T* Handle::getComponentInImmediateChildren() {
		std::size_t childCount = getImmediateChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle child = getChild(i);
			if (T * ptr = child.getComponent<T>()) // Check if child has the component
				return ptr;
		}
	}
	template<typename T>
	std::vector<T*> Handle::getComponentsInImmediateChildren() {
		std::vector<T*> components;
		std::size_t childCount = getImmediateChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle child = getChild(i);
			std::vector<T*> childComponents = child.getComponents<T>();
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}
		return components;
	}

	template<typename T>
	T* Handle::getComponentInChildren() {
		std::size_t childCount = getChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle child = getChild(i);
			if (T* ptr = child.getComponent<T>()) // Check if child has the component
				return ptr;
		}
	}

	template<typename T>
	std::vector<T*> Handle::getComponentsInChildren() {
		std::vector<T*> components;
		std::size_t childCount = getChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle child = getChild(i);
			std::vector<T*> childComponents = child.getComponents<T>();
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}
		return components;
	}

	template<typename T>
	T* Handle::getComponentInParents() {
		Handle* parent = getParent();
		while (parent) {
			if (T* ptr = parent->getComponent<T>()) // Check if parent has the component
				return ptr;

			parent = parent->getParent(); // Check next parent
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*> Handle::getComponentsInParents() {
		std::vector<T*> components;
		Handle parent = getParent();
		if (parent.refresh()) {
			std::vector<T*> parentComponents = parent.getComponents<T>();
			components.insert(components.end(), parentComponents.begin(), parentComponents.end());
			parentComponents = parent.getComponentsInParents<T>();
			components.insert(components.end(), parentComponents.begin(), parentComponents.end());
		}
		return components;
	}

	template<typename T>
	std::vector<T*> Handle::getComponentsUpwards() {
		std::vector<T*> components = getComponents<T>();
		std::vector<T*> parentComponents = getComponentsInParents<T>();
		components.insert(components.end(), parentComponents.begin(), parentComponents.end());
		return components;
	}

	template<typename T>
	std::vector<T*> Handle::getComponentsDownwards() {
		std::vector<T*> components = getComponents<T>();
		std::vector<T*> childComponents = getComponentsInChildren<T>();
		components.insert(components.end(), childComponents.begin(), childComponents.end());
		return components;
	}
}
#endif