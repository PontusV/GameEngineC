#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Archetype.h"
#include "entity/Entity.h"
#include "EntityLocation.h"
#include "HideFlags.h"
#include "component/ComponentTypeInfo.h"
#include "utils/FunctionCaller.h"

#include <map>
#include <vector>
#include <deque>
#include <stdexcept>
#include <iostream>
#include <memory>


namespace Core {

	class EventSystem; // Forward declare
	class IComponentData; // Forward declare
	typedef void (*EntitiesChangedCallbackFun)(void*, std::size_t);

	/* Manages Entities. Emits events regarding Entities */
	class EntityManager {
	public:
		EntityManager(EventSystem* eventSystem) : eventSystem(eventSystem), entityIDCounter(1) {}
		~EntityManager() {
			clear();
		}

		std::vector<Entity> getAllEntities();

		/* Creates an Entity and adds to the Entity ID counter. The returned Entity is suitable for addition into this instance. */
		Entity generateEntity();
		/* Creates a new Entity and adds it to the EntityManager. */
		template <typename... Ts>
		Entity createEntity(Ts&&... components);
		template<typename... Ts>
		Entity createEntityQueued(Ts& ... components);

		/* Adds the Entity to the EntityManager. */
		template <typename... Ts>
		EntityLocationDetailed addEntity(Entity entity, Ts& ... components);

		/* Immediately destroys and removes the Entity from the EntityManager. */
		bool destroyEntity(Entity entity);
		void destroyEntityQueued(Entity entity);
		/* Immediately destroys and removes the Entity, at the specified location, from the EntityManager. */
		bool destroyEntity(EntityLocationDetailed location);

		/* Returns a pointer to the component. Returns nullptr if the Entity does not exist or if the Entity does not have that component. */
		template <typename T>
		T* getComponent(Entity entity);
		/* Returns a pointer to the component. Returns nullptr if the Entity does not exist or if the Entity does not have that component. */
		IComponentData* getComponent(Entity entity, ComponentType type);
		/* Returns a pointer to the component. Returns nullptr if the Entity does not exist or if the Entity does not have that component. */
		IComponentData* getComponent(Entity entity, ComponentTypeID typeID);

		/* Attaches the component to the Entity. Creates the component with the default constructor. */
		template <typename T>
		EntityLocationDetailed addComponent(Entity entity);
		/* Attaches the component to the Entity. Creates the component with the default constructor. Returns temporary pointer to the added component. This pointer will be invalid next frame. */
		template<typename T>
		T* addComponentQueued(Entity entity);
		/* Attaches the component to the Entity. */
		template <typename T>
		EntityLocationDetailed addComponent(Entity entity, T& component);
		/* Attaches the component to the Entity. Returns temporary pointer to the added component. This pointer will be invalid next frame. */
		template<typename T>
		T* addComponentQueued(Entity entity, T& component);
		EntityLocationDetailed addComponent(Entity entity, ComponentTypeID typeID);
		/* Adds a component of given component type id to the Entity, next frame. */
		void addComponentQueued(Entity entity, ComponentTypeID componentTypeID);
		/* Attaches the component to the Entity at the specified location. Creates the component with the default constructor. */
		template <typename T>
		EntityLocationDetailed addComponent(EntityLocationDetailed location);
		/* Attaches the component to the Entity at the specified location. */
		template <typename T>
		EntityLocationDetailed addComponent(EntityLocationDetailed location, T& component);
		EntityLocationDetailed addComponent(EntityLocationDetailed location, ComponentTypeID typeID);

		/* If the Entity has a component of type T, it is removed. It does not remove components deriving from T. The destructor of the component is called and it is removed from the Entity. */
		template <typename T>
		EntityLocationDetailed removeComponent(Entity entity);
		template<typename T>
		void removeComponentQueued(Entity entity);
		EntityLocationDetailed removeComponent(Entity entity, ComponentTypeID typeID);
		void removeComponentQueued(Entity entity, ComponentTypeID componentTypeID);
		/* If the Entity, at the specified location, has a component of type T, it is removed. It does not remove components deriving from T. The destructor of the component is called and it is removed from the Entity. */
		template <typename T>
		EntityLocationDetailed removeComponent(EntityLocationDetailed location);
		EntityLocationDetailed removeComponent(EntityLocationDetailed location, ComponentTypeID typeID);

		template <typename T>
		bool hasComponent(Entity entity);
		bool hasComponent(Entity entity, ComponentType type);
		template <typename T>
		bool hasComponent(EntityLocationDetailed location);
		bool hasComponent(EntityLocationDetailed location, ComponentType type);

		std::vector<IComponentData*> getComponents(Entity entity);
		template<typename T>
		std::vector<T*> getComponents(Entity entity);
		std::vector<IComponentTypeInfo> getComponentTypes(Entity entity);

		/* Throws error if Entity is not mapped to HideFlags inside entityHideFlags. TODO: Move EntityHideFlags to component */
		HideFlags getEntityHideFlags(Entity entity);
		/* Throws error if Entity is not mapped to HideFlags inside entityHideFlags. TODO: Move EntityHideFlags to component */
		void setEntityHideFlags(Entity entity, HideFlags hideFlags);
		EntityLocation getLocation(Entity entity);
		EntityLocationDetailed getLocationDetailed(Entity entity);

		/* Clears this instance of EntityManager. Removes all Entities managed by this instance. */
		void clear();

		/* Activates the Entity and its children. Cannot activate an Entity with an inactive parent. Returns true if successful. */
		bool activate(Entity entity);
		/* Activates the Entity and its children next frame. Cannot activate an Entity with an inactive parent. */
		void activateQueued(Entity entity);
		/* Deactivates the Entity and its children. Returns true if successful. */
		bool deactivate(Entity entity);
		/* Deactivates the Entity and its children next frame. */
		void deactivateQueued(Entity entity);
		/* Activates the Entity, at the specified location, and its children. Cannot activate an Entity with an inactive parent. Returns true if successful. */
		bool activate(EntityLocationDetailed location);
		/* Deactivates the Entity, at the specified location, and its children. Returns true if successful. */
		bool deactivate(EntityLocationDetailed location);
		/* Returns true if the Entity is active */
		bool isActive(Entity entity);
		/* Returns true if the Entity, at the specified location, is active */
		bool isActive(EntityLocationDetailed location);

		void setParent(Entity entity, Entity parent, bool keepPosition = true);
		void setParent(EntityLocationDetailed location, Entity parent, bool keepPosition = true);
		void setParentQueued(Entity entity, Entity parent, bool keepPosition = true);
		void setSiblingIndex(Entity entity, std::size_t index);
		void setSiblingIndex(EntityLocationDetailed location, std::size_t index);
		void setSiblingIndexQueued(Entity entity, std::size_t index);

		/* Calls all the functions in queue and clears it. */
		void processQueue();
		/* Retrieves component from queued createEntity and addComponent */
		template<typename T>
		T* getComponentFromQueue(Entity entity);
		/* Retrieves components from queued createEntity and addComponent */
		template<typename T>
		std::vector<T*> getComponentsFromQueue(Entity entity);

		/* Used by DLL interface. Calls the given callback whenever an Entity has been renamed */
		void setEntitiesChangedCallback(EntitiesChangedCallbackFun callback);
		/* Used by DLL interface. Provides this ptr as an argument in the callback functions */
		void setCallbackPtr(void* ptr);
		
		/* Sets the Entity ID counter. Only allows to set the counter to a value higher than the current one. Returns true if successful */
		bool setEntityIDCounter(std::size_t value);
		std::size_t getEntityIDCounter() const;

	private:
		void removeArchetype(std::shared_ptr<Archetype> archetype);
		std::shared_ptr<Archetype> createArchetype(std::vector<IComponentTypeInfo> types);
		template<typename... Ts>
		std::shared_ptr<Archetype> getArchetype();
		std::shared_ptr<Archetype> getArchetype(std::vector<IComponentTypeInfo> types);

		/* Moves the Entity to another archetype. Helper function for RemoveComponent and AddComponent */
		EntityLocationDetailed moveEntity(Entity entity, std::shared_ptr<Archetype> src, std::shared_ptr<Archetype> dest);

		/* Returns the typeID of the component instance. Helper function to prevent include of IComponentData in this header */
		std::size_t getTypeID(IComponentData* component);

		/* Dispatches Entity added event to EventSystem. Helper function used by addEntity */
		void dispatchEntityAddedEvent(Entity entity, EntityLocationDetailed location);
	private:
		std::deque<IFunctionCaller*> functionQueue;
		std::map<Entity, std::vector<std::size_t>> entityQueueMap;

		std::map<Entity, HideFlags> entityHideFlags;
		std::vector<std::shared_ptr<Archetype>> archetypes;
		std::size_t entityIDCounter;

		EventSystem* eventSystem;

		EntitiesChangedCallbackFun entitiesChangedCallback = nullptr;
		void* callbackPtr = nullptr; // A pointer sent to the callback functions
	};


	// --------------------------- Template Function Definitions --------------------------------


	/* Creates an entity and adds it and its components to a matching Archetype. Returns the Entity. */
	template <typename... Ts>
	Entity EntityManager::createEntity(Ts&&... components) {
		Entity entity = generateEntity();
		addEntity(entity, components...);
		return entity;
	}

	template<typename... Ts>
	Entity EntityManager::createEntityQueued(Ts&... components) {
		Entity entity = generateEntity();
		// Add queue index to map
		entityQueueMap[entity].push_back(functionQueue.size());
		functionQueue.push_back(new FunctionCaller<EntityLocationDetailed, EntityManager, Entity, Ts&...>(&EntityManager::addEntity<Ts...>, *this, entity, components...));
		return entity;
	}

	/* Adds entity and its components to matching Archetype. */
	template <typename... Ts>
	EntityLocationDetailed EntityManager::addEntity(Entity entity, Ts&... components) {
		// Call implementation
		try {
			std::shared_ptr<Archetype> archetype = getArchetype<Ts...>();
			EntityLocation location = archetype->addEntity(entity, components...);
			EntityLocationDetailed locationDetailed = EntityLocationDetailed(location.index, location.chunk, archetype);
			dispatchEntityAddedEvent(entity, locationDetailed);
			if (entitiesChangedCallback) {
				entitiesChangedCallback(callbackPtr, entity.getID());
			}
			return locationDetailed;
		}
		catch (const std::exception& e) {
			std::cout << "Failed to add Entity.\n";
			std::cout << "Exception: " << e.what() << "\n";
			throw e;
		}
	}

	template <typename T>
	T* EntityManager::getComponent(Entity entity) {
		EntityLocation location = getLocation(entity);
		if (!location.isValid()) return nullptr;
		return location.chunk.lock()->getComponent<T>(location.index);
	}

	template<typename T>
	std::vector<T*> EntityManager::getComponents(Entity entity) {
		EntityLocation location = getLocation(entity);
		if (!location.isValid()) return std::vector<T*>();

		std::vector<IComponentData*> components = location.chunk.lock()->getComponents(entity, typeof(T));
		std::vector<T*> castedPtrs; // Copying
		castedPtrs.reserve(components.size());
		for (IComponentData* ptr : components) {
			castedPtrs.push_back(static_cast<T*>(ptr));
		}
		return castedPtrs;
	}

	template <typename T>
	bool EntityManager::hasComponent(Entity entity) {
		return hasComponent(entity, typeof(T));
	}

	template <typename T>
	bool EntityManager::hasComponent(EntityLocationDetailed location) {
		return hasComponent(location, typeof(T));
	}

	template <typename T>
	EntityLocationDetailed EntityManager::addComponent(Entity entity) {
		T component;
		return addComponent(entity, component);
	}

	template<typename T>
	T* EntityManager::addComponentQueued(Entity entity) {
		T component;
		return addComponentQueued(entity, component);
	}

	/* Adds given components to the given Entity. */
	template <typename T>
	EntityLocationDetailed EntityManager::addComponent(Entity entity, T& component) {
		EntityLocationDetailed location = getLocationDetailed(entity);
		return addComponent(location, component);
	}

	template<typename T>
	T* EntityManager::addComponentQueued(Entity entity, T& component) {
		auto function = new FunctionCaller<EntityLocationDetailed, EntityManager, Entity, T&>(&EntityManager::addComponent<T>, *this, entity, component);
		// Add queue index to map
		entityQueueMap[entity].push_back(functionQueue.size());
		functionQueue.push_back(function);
		return function->getArgument<T>();
	}

	template <typename T>
	EntityLocationDetailed EntityManager::addComponent(EntityLocationDetailed location) {
		T component;
		return addComponent(location, component);
	}

	template <typename T>
	EntityLocationDetailed EntityManager::addComponent(EntityLocationDetailed location, T& component) {
		if (!location.isValid()) {
			std::cout << "EntityManager::addComponent::ERROR Cannot add component. The given location is invalid" << std::endl;
			return EntityLocationDetailed(); // Returns invalid location
		}
		Entity entity = location.chunk.lock()->getEntity(location.index);
		auto src = location.archetype.lock();

		std::vector<IComponentTypeInfo> destTypes = src->getTypes();
		destTypes.push_back(IComponentTypeInfo(typeof(T), nameof(T), sizeof(T)));

		std::shared_ptr<Archetype> dest = getArchetype(destTypes);
		EntityLocationDetailed newLocation = moveEntity(entity, src, dest);

		dest->setComponent(entity, component);
		return newLocation;
	}

	/* Removes components with types (specified in template) from the given Entity. */
	template <typename T>
	EntityLocationDetailed EntityManager::removeComponent(Entity entity) {
		return removeComponent(entity, typeIDof(T));
	}

	template<typename T>
	void EntityManager::removeComponentQueued(Entity entity) {
		functionQueue.push_back(new FunctionCaller<EntityLocationDetailed, EntityManager, Entity>(&EntityManager::removeComponent<T>, *this, entity));
	}

	template <typename T>
	EntityLocationDetailed EntityManager::removeComponent(EntityLocationDetailed location) {
		return removeComponent(location, typeIDof(T));
	}

	/* Returns reference to ArchetypeEntry containing Archetype with specified types. Creates a new Archetype (and entry) if no match was found. */
	template<typename... Ts>
	std::shared_ptr<Archetype> EntityManager::getArchetype() {
		// Look for archetype with matching template
		std::vector<IComponentTypeInfo> types = { IComponentTypeInfo(Core::ComponentTypeInfo<Ts>::getType(), Core::ComponentTypeInfo<Ts>::getName(), sizeof(Ts))... }; // Initialize list with TypeIDs from template
		return getArchetype(types);
	}
	
	template<typename T>
	T* EntityManager::getComponentFromQueue(Entity entity) {
		auto it = entityQueueMap.find(entity);
		if (it == entityQueueMap.end()) return nullptr;

		for (const std::size_t& queueIndex : it->second) {
			std::vector<void*> argumentPtrs = functionQueue[queueIndex]->getArgumentPointers();
			for (std::size_t i = 1; i < argumentPtrs.size(); i++) {
				IComponentData* component = static_cast<IComponentData*>(argumentPtrs[i]);
				if (typeof(T) == getTypeID(component)) return static_cast<T*>(component);
			}
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*> EntityManager::getComponentsFromQueue(Entity entity) {
		std::vector<T*> components;
		auto it = entityQueueMap.find(entity);
		if (it == entityQueueMap.end()) return components;

		for (const std::size_t& queueIndex : it->second) {
			std::vector<void*> argumentPtrs = functionQueue[queueIndex]->getArgumentPointers();
			for (std::size_t i = 1; i < argumentPtrs.size(); i++) {
				IComponentData* component = static_cast<IComponentData*>(argumentPtrs[i]);
				if (typeof(T) == getTypeID(component)) components.push_back(static_cast<T*>(component));
			}
		}
		return components;
	}
} // End of namespace
#endif