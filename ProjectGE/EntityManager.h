#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Archetype.h"
#include "Entity.h"
#include "ComponentTypeInfo.h"
#include "EntityLocation.h"
#include "Handle.h"
#include "FunctionCaller.h"

#include <map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <functional>

namespace Core {

	class Component; // Forward declare

	class EntityManager {
	public:
		EntityManager() : entityIDCounter(1), isAwake(false) {}
		~EntityManager() {
			clear();
		}

		template <typename... Ts>
		Handle createEntity(std::string name, Ts&... components);
		void destroyEntity(Entity entity);

		/* Returns a pointer to the component. Returns nullptr if the Entity does not exist or if the Entity does not have that component. */
		template <typename T>
		T* getComponent(Entity entity);
		/* Returns a pointer to the component. Returns nullptr if the Entity does not exist or if the Entity does not have that component. */
		Component* getComponent(Entity entity, ComponentType type);
		/* Attaches the component to the Entity. */
		template <typename T>
		void addComponent(Entity entity, T& component);
		/* If the Entity has a component of type T it is removed/destroyed. It does not removed components deriving from T. The destructor of the component is called and it is removed from the Entity. */
		template <typename T>
		void removeComponent(Entity entity);
		template <typename T>
		bool hasComponent(Entity entity);
		bool hasComponent(Entity entity, ComponentType type);

		std::vector<Component*> getComponents(Entity entity);
		template<typename T>
		std::vector<T*> getComponents(Entity entity);
		std::vector<IComponentTypeInfo> getComponentTypes(Entity entity);

		Handle getParent(Entity entity);
		Handle getChild(Entity entity, int index);
		std::size_t getImmediateChildCount(Entity entity);

		Handle getEntityHandle(Entity entity);
		Handle getEntityHandle(std::string entityName);
		std::string getEntityName(Entity entity);
		EntityLocation getLocation(Entity entity);

		void clear();
		/* Calls awake on all scripts managed by this instance. */
		void awake();
		void processQueue();

		template<typename... Ts>
		Entity createEntityQueued(std::string name, Ts&... components);
		void destroyEntityQueued(Entity entity);
		/* Returns temporary pointer to the added component. This pointer will be invalid next frame. */
		template<typename T>
		T* addComponentQueued(Entity entity, T& component);
		template<typename T>
		void removeComponentQueued(Entity entity);

	private:
		template <typename... Ts>
		Handle addEntity(Entity entity, Ts&... components);
		Entity generateEntity(std::string name);
		void removeEntity(Entity entity, bool destroy = true);
		void removeComponent(Entity entity, ComponentTypeID type);

		void removeArchetype(Archetype* archetype);
		Archetype* createArchetype(std::vector<IComponentTypeInfo> types);
		template<typename... Ts>
		Archetype* getArchetype();
		Archetype* getArchetype(std::vector<IComponentTypeInfo> types);

		/* Inits components and notifies parent Entities. */
		void prepEntity(Entity entity, Handle owner, Archetype* target);

		/* Helper function for RemoveComponent and AddComponent */
		inline Handle moveEntity(Entity entity, Archetype* src, Archetype* dest);

		/* Awakes all scripts attached to the Entity. */
		void awakeEntity(Entity entity);

		/* Awakes the component if it is a type of Script. */
		void awakeComponent(Entity entity, ComponentType type);

	private:
		std::map<Entity, Archetype*> entityMap;
		std::map<std::string, Entity> entityNameMap;
		std::vector<Archetype*> archetypes;
		std::size_t entityIDCounter;

		std::vector<IFunctionCaller*>	functionQueue;
		bool isAwake;
	};


	// --------------------------- Template Function Definitions --------------------------------


	/* Creates an entity and adds it and its components to a matching Archetype. Returns a handle to the Entity. */
	template <typename... Ts>
	Handle EntityManager::createEntity(std::string name, Ts&... components) {
		Entity entity = generateEntity(name);
		Handle handle = addEntity(entity, components...);

		// Awake the entity
		awakeEntity(entity);

		return handle;
	}

	/* Adds entity and its components to matching Archetype. */
	template <typename... Ts>
	Handle EntityManager::addEntity(Entity entity, Ts&... components) {
		Handle owner;
		// Call implementation
		try {
			Archetype* archetype = getArchetype<Ts...>();
			entityMap.insert(std::make_pair(entity, archetype));
			owner = Handle(entity, this);
			EntityLocation location = archetype->addEntity(entity, components...);
			owner.updateLocation(location);
			prepEntity(entity, owner, archetype);
		}
		catch (const std::exception& e) {
			std::cout << "Failed to add Entity.\n";
			std::cout << "Exception: " << e.what() << "\n";
		}
		return owner;
	}

	template <typename T>
	T* EntityManager::getComponent(Entity entity) {
		auto it = entityMap.find(entity);
		if (it == entityMap.end()) return nullptr;

		Archetype* archetype = it->second;
		return archetype->getComponent<T>(entity);
	}


	template<typename T>
	std::vector<T*> EntityManager::getComponents(Entity entity) {
		auto it = entityMap.find(entity);
		if (it == entityMap.end()) return std::vector<T*>();

		Archetype* archetype = it->second;
		std::vector<Component*> components = archetype->getContainer(entity)->getComponents(entity, typeof(T));
		std::vector<T*> castedPtrs; // Copying
		castedPtrs.reserve(components.size());
		for (Component* ptr : components) {
			castedPtrs.push_back(static_cast<T*>(ptr));
		}
		return castedPtrs;
	}


	template <typename T>
	bool EntityManager::hasComponent(Entity entity) {
		return hasComponent(entity, typeof(T));
	}

	/* Adds given components to the given Entity. */
	template <typename T>
	void EntityManager::addComponent(Entity entity, T& component) {

		Archetype* src = entityMap.at(entity);
		std::vector<IComponentTypeInfo> destTypes = src->getTypes();
		destTypes.push_back(IComponentTypeInfo(typeof(T), nameof(T), sizeof(T)));

		Archetype* dest = getArchetype(destTypes);
		Handle owner = moveEntity(entity, src, dest);

		dest->setComponent(entity, component);
		prepEntity(entity, owner, dest);

		// Awake the component
		awakeComponent(entity, typeof(T));
	}

	/* Removes components with types (specified in template) from the given Entity. */
	template <typename T>
	void EntityManager::removeComponent(Entity entity) {
		removeComponent(entity, typeIDof(T));
	}

	/* Returns reference to ArchetypeEntry containing Archetype with specified types. Creates a new Archetype (and entry) if no match was found. */
	template<typename... Ts>
	Archetype* EntityManager::getArchetype() {
		// Look for archetype with matching template
		std::vector<IComponentTypeInfo> types = { IComponentTypeInfo(Core::ComponentTypeInfo<Ts>::getType(), Core::ComponentTypeInfo<Ts>::getName(), sizeof(Ts))... }; // Initialize list with TypeIDs from template
		return getArchetype(types);
	}

	template<typename... Ts>
	Entity EntityManager::createEntityQueued(std::string name, Ts&... components) {
		Entity entity = generateEntity(name);
		functionQueue.push_back(new FunctionCaller<Handle, EntityManager, Entity, Ts&...>(&EntityManager::addEntity<Ts...>, *this, entity, components...));
		return entity;
	}
	template<typename T>
	T* EntityManager::addComponentQueued(Entity entity, T& component) {
		auto function = new FunctionCaller<void, EntityManager, Entity, T&>(&EntityManager::addComponent<T>, *this, entity, component);
		functionQueue.push_back(function);
		return function->getArgument<T>();
	}
	template<typename T>
	void EntityManager::removeComponentQueued(Entity entity) {
		functionQueue.push_back(new FunctionCaller<void, EntityManager, Entity>(&EntityManager::removeComponent<T>, *this, entity));
	}
} // End of namespace
#endif