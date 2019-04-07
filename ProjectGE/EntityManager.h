#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Archetype.h"
#include "Entity.h"
#include "ComponentTypeInfo.h"
#include "EntityLocation.h"
#include "Handle.h"

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

		template <typename... Ts> Handle	createEntity(Ts*... components);
		template <typename... Ts> Handle	addEntity(Entity entity, Ts*... components);
		void								destroyEntity(Entity entity);

		/* Returns a pointer to the component. Returns nullptr if the Entity does not exist or if the Entity does not have that component. */
		template <typename T> T*			getComponent(Entity entity);
		/* Returns a pointer to the component. Returns nullptr if the Entity does not exist or if the Entity does not have that component. */
		Component*							getComponent(Entity entity, ComponentTypeID id);
		template <typename T> void			addComponent(Entity entity, T* component);
		template <typename T> void			removeComponent(Entity entity);
		void								removeComponent(Entity entity, ComponentTypeInfo type);
		template <typename T> bool			hasComponent(Entity entity);
		bool								hasComponent(Entity entity, ComponentTypeID id);

		std::vector<Component*>				getComponents(Entity entity);

		Handle								getParent(Entity entity);
		Handle*								getChild(Entity entity, int index);
		std::size_t							getChildCount(Entity entity);

		Handle								getEntityHandle(Entity entity);
		EntityLocation						getLocation(Entity entity);

		void								clear();

	private:
		void								removeEntity(Entity entity, bool destroy = true);

		void								removeArchetype(Archetype* archetype);
		Archetype*							createArchetype(std::vector<ComponentTypeInfo> types);
		template<typename... Ts> Archetype*	getArchetype();
		Archetype*							getArchetype(std::vector<ComponentTypeInfo> types);


		/* Inits components and notifies parent Entities. */
		void prepEntity(Entity entity, Handle owner, Archetype* target);

		/* Helper function for RemoveComponent and AddComponent */
		inline Handle moveEntity(Entity entity, Archetype* src, Archetype* dest);

	private:
		std::map<Entity, Archetype*> entityMap;
		std::vector<Archetype*> archetypes;
		std::size_t entityIDCounter;
	};


	// --------------------------- Template Function Definitions --------------------------------


	/* Creates an entity and adds it and its components to a matching Archetype. Returns a handle to the Entity. */
	template <typename... Ts>
	Handle EntityManager::createEntity(Ts*... components) {
		Entity entity(entityIDCounter++);
		return addEntity(entity, components...);
	}

	/* Adds entity and its components to matching Archetype. */
	template <typename... Ts>
	Handle EntityManager::addEntity(Entity entity, Ts*... components) {
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

		// Clean up
		std::vector<Component*> componentList{ components... };
		for (Component* component : componentList) {
			delete component;
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
		Handle owner = moveEntity(entity, src, dest);

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