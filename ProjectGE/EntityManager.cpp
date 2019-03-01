#include "EntityManager.h"
using namespace GameEngine;

/* Removes specified entity and its components from the Archetype that stores it. */
void EntityManager::removeEntity(Entity entity) {
	std::shared_ptr<IArchetype> archetype = entityMap.at(entity).getArchetype();
	entityMap.erase(entity);
	archetype->removeEntity(entity);

	if (archetype->isEmpty()) {
		removeArchetype(archetype);
	}
}

/* Returns ArchetypeEntry containing Archetype with specified types. Throws exception if no match was found. */
ArchetypeEntry* EntityManager::getArchetype(std::vector<ComponentTypeID> types) {
	for (ArchetypeEntry& entry : archetypes) {
		if (entry.match(types))
			return &entry;
	}
	return nullptr;
}

/* Removes Archetypes from vector archetypes and notifies listeners. This deletes all entities and components currently contained by the Archetype. */
void EntityManager::removeArchetype(std::shared_ptr<IArchetype> archetype) {
	for (std::size_t i = 0; i < archetypes.size(); i++) {
		if (archetypes[i].getArchetype() == archetype) {
			//Removes archetype at i
			std::swap(archetypes[i], archetypes.back());
			archetypes.pop_back();
		}
	}
}

void EntityManager::addEntity(Entity entity) {
	ArchetypeEntry archetypeEntry = getArchetype<>();
	entityMap.insert(std::make_pair(entity, archetypeEntry));
	archetypeEntry.getArchetype()->addEntity(entity, {});
}

std::vector<Component*> EntityManager::getComponents(Entity entity) {
	std::shared_ptr<IArchetype> archetype = entityMap.at(entity).getArchetype();
	return archetype->getData(entity);
}

void EntityManager::clear() {
	archetypes.clear();
	entityMap.clear();
}