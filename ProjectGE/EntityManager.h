#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "ComponentType.h"
#include "Archetype.h"
#include "ArchetypeEntry.h"
#include "Entity.h"
#include "Component.h"
#include "QuickSort.h"

#include <map>
#include <vector>
#include <memory>
#include <utility>
#include <array>
#include <tuple>
#include <stdexcept>
#include <iostream>

using namespace GameEngine::util;

namespace GameEngine {
	namespace util {
		template <class Tuple>
		constexpr std::vector<Component*> tupleToVector(Tuple&& tuple) {
			return std::apply([](auto&&... elems) {
				return std::vector<Component*>{elems...};
			}, std::forward<Tuple>(tuple));
		}

		template <typename... Ts, std::size_t... Is>
		inline auto sortComponentPack_impl(std::index_sequence<Is...>, Ts*... components) { //Return tuple
			std::vector<Component*> componentVec{ components... };
			std::sort(componentVec.begin(), componentVec.end(), [](Component* l, Component* r) {
				return l->getTypeID() > r->getTypeID();
			});
			constexpr auto sortedTypeIDs = sort(std::array{ Ts::TYPE_ID... }); //Change to std::sort in c++ 20 and remove QuickSort.h
			return std::make_tuple(static_cast<typename decltype(getComponentType< sortedTypeIDs[Is] >())::type*>(componentVec[Is])...);
		}

		template <typename... Ts>
		auto sortComponentPack(Ts*... components) { //Return tuple
			return sortComponentPack_impl(std::make_index_sequence<sizeof...(Ts)>{}, components...);
		}
	}


	class EntityManager {
	public:
		EntityManager() {}
		~EntityManager() {
		}

		/* Creates an entity and adds it and its components to a matching Archetype. Returns a handle to the Entity. */
		template <typename... Ts>
		Entity createEntity(Ts...);

		/* Removes specified entity and its components from the Archetype that stores it. */
		void removeEntity(Entity entity);

		/* Adds entity and its components to matching Archetype. */
		template <typename... Ts>
		void addEntity(Entity entity, Ts*... components);

		/* Adds Entity with no components. */
		void addEntity(Entity entity);

		template <typename T>
		Component* getComponent(Entity entity);

		/* Adds given components to the given Entity. */
		template <typename T>
		void addComponent(Entity entity, T* component);

		/* Removes components with types (specified in template) from the given Entity. */
		template <typename T>
		void removeComponent(Entity entity);

		std::vector<Component*> getComponents(Entity entity);

		void clear();
	private:

		/* Returns reference to ArchetypeEntry containing Archetype with specified types. Creates a new Archetype (and entry) if no match was found. */
		template<typename... Ts>
		ArchetypeEntry getArchetype();

		/* Returns pointer to ArchetypeEntry containing Archetype with specified types. Returns nullptr if no match was found. */
		ArchetypeEntry* getArchetype(std::vector<ComponentTypeID> types);

		template<typename... Ts>
		ArchetypeEntry createArchetype();

		/* Removes Archetypes from vector archetypes and notifies listeners. This deletes all entities and components currently contained by the Archetype. */
		void removeArchetype(std::shared_ptr<IArchetype> archetype);

		/* ComponentPack must be sorted by ComponentTypeID (from lowest to highest) */
		template <typename... Ts>
		inline void addEntity_impl(Entity entity, std::tuple<Ts*...>& componentPack);

		/* Helper function for RemoveComponent and AddComponent */
		inline void moveEntity(Entity entity, std::shared_ptr<IArchetype> a, std::vector<Component*> components);

		//Variables
		std::map<Entity, ArchetypeEntry> entityMap;
		std::vector<ArchetypeEntry> archetypes;
		std::size_t entityIDCounter;
	};
	//-------------------------------------


	/* Creates an entity and adds it and its components to a matching Archetype. Returns a handle to the Entity. */
	template <typename... Ts>
	Entity EntityManager::createEntity(Ts... components) {
		Entity entity(entityIDCounter++);
		addEntity(entity, components...);

		return entity;
	}

	/* Adds entity and its components to matching Archetype. */
	template <typename... Ts>
	void EntityManager::addEntity(Entity entity, Ts*... components) {
		auto sortedComponentPack = sortComponentPack(components...);

		//Call implementation
		try {
			addEntity_impl(entity, sortedComponentPack);
		}
		catch (const std::exception&) {
			std::cout << "Failed to add Entity with component pack: " << typeid(sortedComponentPack).name() << ".\n";
			//std::cout << e.what();
		}

		std::vector<Component*> componentList{ components... };
		//Clean up
		for (Component* component : componentList) {
			delete component;
		}
	}

	template <typename T>
	Component* EntityManager::getComponent(Entity entity) {
		/*if (entityMap.find(entity) != entityMap.end()) {
			std::shared_ptr<IArchetype> archetype = entityMap.at(entity).getArchetype();

			return archetype->getComponent(entity, T::TYPE_ID);
		}
		return nullptr;*/
		std::shared_ptr<IArchetype> archetype = entityMap.at(entity).getArchetype();
		return archetype->getComponent(entity, T::TYPE_ID);
	}

	template <typename... Ts>
	inline void EntityManager::addEntity_impl(Entity entity, std::tuple<Ts*...>& componentPack) {
		ArchetypeEntry archetypeEntry = getArchetype<Ts...>();
		entityMap.insert(std::make_pair(entity, archetypeEntry));
		std::vector<Component*> components = tupleToVector(componentPack);
		archetypeEntry.getArchetype()->addEntity(entity, components);
	}

	/* Adds given components to the given Entity. */
	template <typename T>
	void EntityManager::addComponent(Entity entity, T* component) {
		ArchetypeEntry archetypeEntry = entityMap.at(entity);
		std::shared_ptr<IArchetype> archetype = archetypeEntry.getArchetype();

		std::vector<Component*> components = archetype->getData(entity);
		components.push_back(component);
		std::sort(components.begin(), components.end(), [](Component* l, Component* r) {
			return l->getTypeID() > r->getTypeID();
		});

		std::shared_ptr<IArchetype> a = archetype->addComponentType(T::TYPE_ID);
		moveEntity(entity, a, components);

		delete component;
	}

	/* Removes components with types (specified in template) from the given Entity. */
	template <typename T>
	void EntityManager::removeComponent(Entity entity) {
		ArchetypeEntry archetypeEntry = entityMap.at(entity);
		std::shared_ptr<IArchetype> archetype = archetypeEntry.getArchetype();

		std::vector<Component*> tempCmps = archetype->getData(entity);
		std::vector<Component*> components;
		for (Component* cmp : tempCmps) {
			if (cmp->getTypeID() != T::TYPE_ID)
				components.push_back(cmp);
		}

		std::shared_ptr<IArchetype> a = archetype->removeComponentType(T::TYPE_ID);
		moveEntity(entity, a, components);
	}

	/* Returns reference to ArchetypeEntry containing Archetype with specified types. Creates a new Archetype (and entry) if no match was found. */
	template<typename... Ts>
	ArchetypeEntry EntityManager::getArchetype() {
		//Look for archetype with matching template
		std::vector<ComponentTypeID> types = { Ts::TYPE_ID... }; //Initialize list with TypeIDs from template
		if (ArchetypeEntry* entryPtr = getArchetype(types))
			return *entryPtr;
		return createArchetype<Ts...>();
	}

	template<typename... Ts>
	ArchetypeEntry EntityManager::createArchetype() {
		std::shared_ptr<IArchetype> archetype = std::make_shared<Archetype<Ts...>>();
		archetypes.emplace_back(ArchetypeEntry(archetype, { Ts::TYPE_ID... }));
		return archetypes.back();
	}

	//Helper
	void EntityManager::moveEntity(Entity entity, std::shared_ptr<IArchetype> target, std::vector<Component*> components) {
		ArchetypeEntry entry = ArchetypeEntry(target, target->getTypeIDs());
		if (ArchetypeEntry* entryPtr = getArchetype(target->getTypeIDs()))
			entry = *entryPtr;

		entry.getArchetype()->addEntity(entity, components);
		removeEntity(entity);
		entityMap.insert(std::make_pair(entity, archetypes.emplace_back(entry)));
	}
} //End of namespace
#endif