#ifndef HANDLE_H
#define HANDLE_H

#include "entity/Entity.h"
#include "entity/HideFlags.h"
#include "entity/EntityLocation.h"
#include "entity/component/ComponentTypeInfo.h"
#include <vector>

typedef std::size_t ComponentTypeID;

namespace Core {

	class EntityManager;
	class IComponentData;

	/* A handle for an Entity. Used for management of an Entity, including its hierarchy */
	class Handle {
	public:
		Handle(Entity entity, EntityManager* entityManager);
		Handle(Entity entity, EntityManager* entityManager, EntityLocationDetailed locationData);
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
		EntityManager* getEntityManager() const;

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
		void updateLocation(EntityLocationDetailed location);
		const EntityLocationDetailed& getLocation() const;

		/* Returns the number of parents above in the hierarchy. */
		std::size_t getDepth();

		/* Adds the component next frame */
		void addComponent(ComponentTypeID componentTypeID);
		/* Adds the component immediately. Warning: may cause unknown behaviour */
		IComponentData* addComponentImmediate(ComponentTypeID componentTypeID);
		/* Removes the component next frame */
		void removeComponent(ComponentTypeID componentTypeID);
		/* Removes the component immediate. Warning: may cause unknown behaviour */
		bool removeComponentImmediate(ComponentTypeID componentTypeID);
		/* Destroys the Entity next frame */
		void destroy();
		/* Destroys the Entity immediately. Warning: may cause unknown behaviour */
		void destroyImmediate();

		template<typename T>
		bool hasComponent();
		bool hasComponent(ComponentType type);
		bool hasComponent(ComponentTypeID typeID);

		/* Returns the number of components attached to the Entity */
		std::size_t getComponentCount();
		/* Returns the number of components attached to the Entity, matching the given ComponentTypeID */
		std::size_t getComponentCount(ComponentTypeID typeID);

		template<typename T>
		T* getComponent();
		IComponentData* getComponent(ComponentType type);
		IComponentData* getComponent(ComponentTypeID typeID);
		std::vector<IComponentData*> getComponents();
		template<typename T>
		std::vector<T*> getComponents();
		std::vector<IComponentData*> getComponents(ComponentType type);
		std::vector<IComponentData*> getComponents(ComponentTypeID typeID);

		template<typename T>
		T* getComponentInChildren(bool includeInactive = false);
		std::vector<IComponentData*> getComponentsInChildren(bool includeInactive = false);
		template<typename T>
		std::vector<T*> getComponentsInChildren(bool includeInactive = false);

		template<typename T>
		T* getComponentInImmediateChildren(bool includeInactive = false);
		std::vector<IComponentData*> getComponentsInImmediateChildren(bool includeInactive = false);
		template<typename T>
		std::vector<T*> getComponentsInImmediateChildren(bool includeInactive = false);

		template<typename T>
		T* getComponentInParents();
		std::vector<IComponentData*> getComponentsInParents();
		template<typename T>
		std::vector<T*> getComponentsInParents();

		/* Returns all components attached to the entity and its parents. */
		std::vector<IComponentData*> getComponentsUpwards();
		/* Returns all components (deriving from T) attached to the entity and its parents. */
		template<typename T>
		std::vector<T*> getComponentsUpwards();

		/* Returns all components attached to the entity, its children and the childrens children, etc... */
		std::vector<IComponentData*> getComponentsDownwards(bool includeInactive = false);
		/* Returns all components (deriving from T) attached to the entity and its parent and the parent of the parent, etc... */
		template<typename T>
		std::vector<T*> getComponentsDownwards(bool includeInactive = false);

		std::vector<IComponentTypeInfo> getComponentTypes();
		HideFlags getEntityHideFlags();
		void setEntityHideFlags(HideFlags hideFlags);
		
		bool hasParent();
		void setParent(const Entity& entity, bool keepPosition = false);
		void setParent(Handle entity, bool keepPosition = false);
		void removeParent(bool keepPosition = false);
		Handle getParent();
		Handle getChild(std::size_t index);
		Entity getParentEntity();
		Entity getChildEntity(std::size_t index);
		std::size_t getImmediateChildCount();
		std::size_t getChildCount();

		void setSiblingIndex(std::size_t index);
		void setSiblingIndexQueued(std::size_t index);
		std::size_t getSiblingIndex();

		template<typename Archetype>
		void serialize(Archetype& ar) const {
			ar(entity);
		}
		template<typename Archetype>
		void deserialize(Archetype& ar) {
			ar(entity);
		}

	protected:
		// Data required to update itself
		EntityManager* entityManager = nullptr;
		Entity entity = Entity(0);
		EntityLocationDetailed locationData = EntityLocationDetailed();
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
		std::vector<IComponentData*> components = getComponents(typeof(T));
		std::vector<T*> castedPtrs; // Copying
		castedPtrs.reserve(components.size());
		for (IComponentData* ptr : components) {
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
		Handle parent = getParent();
		if (parent.refresh()) {
			if (T* ptr = parent.getComponent<T>()) // Check if parent has the component
				return ptr;
			return parent.getComponentInParents<T>();
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