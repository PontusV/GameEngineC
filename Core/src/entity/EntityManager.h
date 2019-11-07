#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Archetype.h"
#include "entity/Entity.h"
#include "EntityLocation.h"
#include "HideFlags.h"
#include "component/ComponentTypeInfo.h"

#include <map>
#include <vector>
#include <stdexcept>
#include <iostream>


namespace Core {

	class Component; // Forward declare

	class EntityManager {
	public:
		EntityManager() : entityIDCounter(1) {}
		~EntityManager() {
			clear();
		}

		template <typename... Ts>
		Entity createEntity(std::string name, Ts&... components);
		template <typename... Ts>
		EntityLocation addEntity(Entity entity, Ts& ... components);
		void destroyEntity(Entity entity);

		/* Returns a pointer to the component. Returns nullptr if the Entity does not exist or if the Entity does not have that component. */
		template <typename T>
		T* getComponent(Entity entity);
		/* Returns a pointer to the component. Returns nullptr if the Entity does not exist or if the Entity does not have that component. */
		Component* getComponent(Entity entity, ComponentType type);
		/* Attaches the component to the Entity. */
		template <typename T>
		EntityLocation addComponent(Entity entity, T& component);
		/* If the Entity has a component of type T it is removed/destroyed. It does not removed components deriving from T. The destructor of the component is called and it is removed from the Entity. */
		template <typename T>
		EntityLocation removeComponent(Entity entity);
		EntityLocation removeComponent(Entity entity, ComponentTypeID type);
		template <typename T>
		bool hasComponent(Entity entity);
		bool hasComponent(Entity entity, ComponentType type);

		std::vector<Component*> getComponents(Entity entity);
		template<typename T>
		std::vector<T*> getComponents(Entity entity);
		std::vector<IComponentTypeInfo> getComponentTypes(Entity entity);

		Entity getEntity(std::string entityName);
		std::string getEntityName(Entity entity);
		HideFlags getEntityHideFlags(Entity entity);
		void setEntityHideFlags(Entity entity, HideFlags hideFlags);
		EntityLocation getLocation(Entity entity);

		void clear();

		Entity generateEntity(std::string name);
		/* @return Returns true if the Entity had a name to remove. */
		bool removeEntityName(Entity entity);

	private:
		void removeArchetype(Archetype* archetype);
		Archetype* createArchetype(std::vector<IComponentTypeInfo> types);
		template<typename... Ts>
		Archetype* getArchetype();
		Archetype* getArchetype(std::vector<IComponentTypeInfo> types);

		/* Helper function for RemoveComponent and AddComponent */
		EntityLocation moveEntity(Entity entity, Archetype* src, Archetype* dest);

	private:
		std::map<Entity, Archetype*> entityMap;
		std::map<std::string, Entity> entityNameMap;
		std::map<Entity, HideFlags> entityHideFlags;
		std::vector<Archetype*> archetypes;
		std::size_t entityIDCounter;
	};


	// --------------------------- Template Function Definitions --------------------------------


	/* Creates an entity and adds it and its components to a matching Archetype. Returns the Entity. */
	template <typename... Ts>
	Entity EntityManager::createEntity(std::string name, Ts&... components) {
		Entity entity = generateEntity(name);
		addEntity(entity, components...);
		return entity;
	}

	/* Adds entity and its components to matching Archetype. */
	template <typename... Ts>
	EntityLocation EntityManager::addEntity(Entity entity, Ts&... components) {
		// Call implementation
		try {
			Archetype* archetype = getArchetype<Ts...>();
			entityMap.insert(std::make_pair(entity, archetype));
			return archetype->addEntity(entity, components...);
		}
		catch (const std::exception& e) {
			std::cout << "Failed to add Entity.\n";
			std::cout << "Exception: " << e.what() << "\n";
			throw e;
		}
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
	EntityLocation EntityManager::addComponent(Entity entity, T& component) {
		Archetype* src = entityMap.at(entity);
		std::vector<IComponentTypeInfo> destTypes = src->getTypes();
		destTypes.push_back(IComponentTypeInfo(typeof(T), nameof(T), sizeof(T)));

		Archetype* dest = getArchetype(destTypes);
		EntityLocation location = moveEntity(entity, src, dest);

		dest->setComponent(entity, component);
		return location;
	}

	/* Removes components with types (specified in template) from the given Entity. */
	template <typename T>
	EntityLocation EntityManager::removeComponent(Entity entity) {
		return removeComponent(entity, typeIDof(T));
	}

	/* Returns reference to ArchetypeEntry containing Archetype with specified types. Creates a new Archetype (and entry) if no match was found. */
	template<typename... Ts>
	Archetype* EntityManager::getArchetype() {
		// Look for archetype with matching template
		std::vector<IComponentTypeInfo> types = { IComponentTypeInfo(Core::ComponentTypeInfo<Ts>::getType(), Core::ComponentTypeInfo<Ts>::getName(), sizeof(Ts))... }; // Initialize list with TypeIDs from template
		return getArchetype(types);
	}
} // End of namespace
#endif