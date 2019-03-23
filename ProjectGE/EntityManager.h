#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Archetype.h"
#include "Entity.h"
#include "ComponentTypeInfo.h"

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

		template <typename... Ts> Entity	createEntity(Ts*... components);
		template <typename... Ts> bool		addEntity(Entity entity, Ts*... components);
		void								removeEntity(Entity entity, bool destroy = true);

		template <typename T> T*			getComponent(Entity entity);
		Component*							getComponent(Entity entity, ComponentTypeID id);
		template <typename T> void			addComponent(Entity entity, T* component);
		template <typename T> void			removeComponent(Entity entity);
		void								removeComponent(Entity entity, ComponentTypeInfo type);
		template <typename T> bool			hasComponent(Entity entity);
		bool								hasComponent(Entity entity, ComponentTypeID id);

		std::vector<Component*>				getComponents(Entity entity);

		Entity*								getParent(Entity entity);
		ChunkEntityHandle*					getChild(Entity entity, int index);
		std::size_t							getChildCount(Entity entity);

		void								clear();

	private:
		void								removeArchetype(Archetype* archetype);
		Archetype*							createArchetype(std::vector<ComponentTypeInfo> types);
		template<typename... Ts> Archetype*	getArchetype();
		Archetype*							getArchetype(std::vector<ComponentTypeInfo> types);


		/* Inits components and notifies parent Entities. */
		void prepEntity(Entity entity, ChunkEntityHandle owner, Archetype* target);

		/* Helper function for RemoveComponent and AddComponent */
		inline ChunkEntityHandle moveEntity(Entity entity, Archetype* src, Archetype* dest);

	private:
		std::map<Entity, Archetype*> entityMap;
		std::vector<Archetype*> archetypes;
		std::size_t entityIDCounter;
	};


	// --------------------------- Template Function Definitions --------------------------------


	/* Creates an entity and adds it and its components to a matching Archetype. Returns a handle to the Entity. */
	template <typename... Ts>
	Entity EntityManager::createEntity(Ts*... components) {
		Entity entity(entityIDCounter++);
		if (addEntity(entity, components...))
			return entity;
		else
			return Entity(Entity::INVALID_ID); // Returns entity with invalid ID
	}

	/* Adds entity and its components to matching Archetype. */
	template <typename... Ts>
	bool EntityManager::addEntity(Entity entity, Ts*... components) {
		bool success = true;
		// Call implementation
		try {
			Archetype* archetype = getArchetype<Ts...>();
			entityMap.insert(std::make_pair(entity, archetype));
			ChunkEntityHandle owner = archetype->addEntity(entity, components...);
			prepEntity(entity, owner, archetype);
		}
		catch (const std::exception& e) {
			success = false;
			std::cout << "Failed to add Entity.\n";
			std::cout << "Exception: " << e.what() << "\n";
		}

		// Clean up
		std::vector<Component*> componentList{ components... };
		for (Component* component : componentList) {
			delete component;
		}
		return success;
	}

	template <typename T>
	T* EntityManager::getComponent(Entity entity) {
		return static_cast<T*>(getComponent(entity, T::TYPE_ID));
	}


	template <typename T>
	bool EntityManager::hasComponent(Entity entity) {
		return hasComponent(entity, T::TYPE_ID);
	}

	/* Adds given components to the given Entity. */
	template <typename T>
	void EntityManager::addComponent(Entity entity, T* component) {
		Archetype* src = entityMap.at(entity);
		std::vector<ComponentTypeInfo> destTypes = src->getTypes();
		destTypes.push_back({ sizeof(T), T::TYPE_ID });

		Archetype* dest = getArchetype(destTypes);
		ChunkEntityHandle owner = moveEntity(entity, src, dest);

		dest->setComponent(entity, component);
		prepEntity(entity, owner, dest);

		// Clean up
		delete component;
	}

	/* Removes components with types (specified in template) from the given Entity. */
	template <typename T>
	void EntityManager::removeComponent(Entity entity) {
		removeComponent(entity, ComponentTypeInfo(sizeof(T), T::TYPE_ID));
	}

	/* Returns reference to ArchetypeEntry containing Archetype with specified types. Creates a new Archetype (and entry) if no match was found. */
	template<typename... Ts>
	Archetype* EntityManager::getArchetype() {
		// Look for archetype with matching template
		std::vector<ComponentTypeInfo> types = { {sizeof(Ts), Ts::TYPE_ID}... }; // Initialize list with TypeIDs from template
		return getArchetype(types);
	}
} // End of namespace
#endif