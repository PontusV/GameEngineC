#ifndef HANDLE_H
#define HANDLE_H

#include "entity/Entity.h"
#include "entity/HideFlags.h"
#include "entity/EntityLocation.h"
#include "entity/component/ComponentTypeInfo.h"
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
		bool operator==(const Handle& other) const;
		bool operator!=(const Handle& other) const;
		/* Returns true if the given Entity is a child of the Entity this instance points to. */
		bool isChild(Entity entity);
		/* Returns true if the given Entity is a parent of the Entity this instance points to. */
		bool isParent(Entity entity);
		/* Returns true if the given Entity is an immediate child of the Entity this instance points to. */
		bool isImmediateChild(Entity entity);

		/* Activates the Entity and enables all behaviours. */
		void activate();
		/* Deactivates the Entity and disables all behaviours. */
		void deactivate();
		bool isActive();

		const Entity& getEntity() const;
		Scene* getScene() const;

		/* Checks if pointed towards a valid Entity ID. */
		bool isValid();
		/* Checks if the Handle is pointing to the correct location in memory. */
		bool isUpdated();
		/* Returns true if the Handle is valid. If not, the handle is updated and returns true if the update was successful. */
		bool refresh();
		/* Makes the Handle invalid. */
		void clear();

		/* Updates the Handle so it points towards the target Entity. */
		void update();
		void updateLocation(EntityLocation location);
		const EntityLocation& getLocation() const;

		/* Returns the number of parents above in the hierarchy. */
		std::size_t getDepth();

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
		T* getComponentInChildren(bool includeInactive = false);
		std::vector<Component*> getComponentsInChildren(bool includeInactive = false);
		template<typename T>
		std::vector<T*> getComponentsInChildren(bool includeInactive = false);

		template<typename T>
		T* getComponentInImmediateChildren(bool includeInactive = false);
		std::vector<Component*> getComponentsInImmediateChildren(bool includeInactive = false);
		template<typename T>
		std::vector<T*> getComponentsInImmediateChildren(bool includeInactive = false);

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
		std::vector<Component*> getComponentsDownwards(bool includeInactive = false);
		/* Returns all components (deriving from T) attached to the entity and its parent and the parent of the parent, etc... */
		template<typename T>
		std::vector<T*> getComponentsDownwards(bool includeInactive = false);

		std::string getEntityName();
		HideFlags getEntityHideFlags();
		void setEntityHideFlags(HideFlags hideFlags);
		
		bool hasParent();
		void setParent(const Entity& entity);
		void setParent(Handle entity);
		void removeParent();
		Handle getParent();
		Handle getChild(std::size_t index);
		std::size_t getImmediateChildCount();
		std::size_t getChildCount();

		void setSiblingIndex(std::size_t index);
		void setSiblingIndexQueued(std::size_t index);
		std::size_t getSiblingIndex();

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
	T* Handle::getComponentInImmediateChildren(bool includeInactive) {
		std::size_t childCount = getImmediateChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle child = getChild(i);
			if (child.isActive() || includeInactive) {
				if (T* ptr = child.getComponent<T>()) // Check if child has the component
					return ptr;
			}
		}
		return nullptr;
	}
	template<typename T>
	std::vector<T*> Handle::getComponentsInImmediateChildren(bool includeInactive) {
		std::vector<T*> components;
		std::size_t childCount = getImmediateChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle child = getChild(i);
			if (child.isActive() || includeInactive) {
				std::vector<T*> childComponents = child.getComponents<T>();
				components.insert(components.end(), childComponents.begin(), childComponents.end());
			}
		}
		return components;
	}

	template<typename T>
	T* Handle::getComponentInChildren(bool includeInactive) {
		std::size_t childCount = getChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle child = getChild(i);
			if (child.isActive() || includeInactive) {
				if (T* ptr = child.getComponent<T>()) // Check if child has the component
					return ptr;
			}
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*> Handle::getComponentsInChildren(bool includeInactive) {
		std::vector<T*> components;
		std::size_t childCount = getChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle child = getChild(i);
			if (child.isActive() || includeInactive) {
				std::vector<T*> childComponents = child.getComponents<T>();
				components.insert(components.end(), childComponents.begin(), childComponents.end());
			}
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
	std::vector<T*> Handle::getComponentsDownwards(bool includeInactive) {
		std::vector<T*> components = getComponents<T>();
		std::vector<T*> childComponents = getComponentsInChildren<T>(includeInactive);
		components.insert(components.end(), childComponents.begin(), childComponents.end());
		return components;
	}
}
#endif