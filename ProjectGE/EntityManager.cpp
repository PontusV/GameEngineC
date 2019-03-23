#include "EntityManager.h"
#include "ParentEntity.h"
#include "ChildManager.h"
#include "ChunkEntityHandle.h"

using namespace Core;

/* Returns nullptr if entity does not have ParentEntity component or if the parent is nullptr. */
Entity* EntityManager::getParent(Entity entity) {
	ParentEntity* parent = getComponent<ParentEntity>(entity);
	if (parent) { // If component exists
		if (parent->getParent().getID() == Entity::INVALID_ID) return nullptr;
		return &parent->getParent();
	}
	return nullptr;
}

/* Returns nullptr if entity does not have ChildRefCollection component or if the index is out of bounds. */
ChunkEntityHandle* EntityManager::getChild(Entity entity, int index) {
	ChildManager* childRefs = getComponent<ChildManager>(entity);
	if (childRefs) { // If component exists
		return childRefs->getChild(index);
	}
	return nullptr;
}

std::size_t	EntityManager::getChildCount(Entity entity) {
	ChildManager* childRefs = getComponent<ChildManager>(entity);
	if (childRefs) { // If component exists
		return childRefs->getChildCount();
	}
	return 0;
}

/* Removes specified entity and its components from the Archetype that stores it. Destroy shows if the Entity is being moved or not. */
void EntityManager::removeEntity(Entity entity, bool destroy) {
	// Destroy children first. The children will destroy their own children.
	if (destroy) {
		std::size_t childCount = getChildCount(entity);
		for (std::size_t i = childCount; i-- != 0;) { // Destroy from back to start
			ChunkEntityHandle* child = getChild(entity, i);
			if (child) {
				removeEntity(child->getEntity(), true);
			}
		}
	}

	// Notify parents
	Entity* parent = getParent(entity);
	while (parent) {
		Entity currentParent = *parent;

		ChildManager* childManager = getComponent<ChildManager>(currentParent);
		if (childManager) childManager->childRemoved(entity);

		// Prep for next iteration
		parent = getParent(currentParent);

		// Remove child ref container if no children exist
		if (getChildCount(currentParent) == 0) {
			removeComponent<ChildManager>(currentParent);
		}
	}

	// Remove
	auto it = entityMap.find(entity);
	if (it == entityMap.end()) {
		std::cout << "EntityManager::removeEntity::ERROR The entity does not exist in this manager!\n";
		throw std::invalid_argument("EntityManager::removeEntity::ERROR The entity does not exist in this manager!");
	}
	Archetype* archetype = it->second;

	std::vector<Component*> components = archetype->getComponents(entity);
	for (Component* component : components) {
		component->end(); // Notify components that they are about to be removed
		//Check if marked for destruction
		if (component->isDestroyed()) {
			component->~Component();
		}
	}

	entityMap.erase(entity);
	archetype->removeEntity(entity, destroy);

	if (archetype->isEmpty()) {
		removeArchetype(archetype);
	}
}

/* Prepares recently added entity to be used by the systems. */
void EntityManager::prepEntity(Entity entity, ChunkEntityHandle owner, Archetype* target) {
	// Set owner
	std::vector<Component*> components = target->getComponents(entity);
	for (Component* component : components) {
		component->setOwner(owner);
		component->init();
	}

	// Notify parents
	Entity* parent = getParent(entity);
	if (!parent && hasComponent<ParentEntity>(entity)) { // If parent is invalid. Remove the parent component
		removeEntity(entity);
	} else {
		while (parent) {
			Entity currentParent = *parent;
			if (getChildCount(currentParent) == 0) { // Check if first child
				addComponent(currentParent, new ChildManager());
			}
			ChildManager* childManager = getComponent<ChildManager>(currentParent);
			if (childManager) childManager->childAdded(owner);
			// Prep for next iteration
			parent = getParent(currentParent);
		}
	}
}

void EntityManager::removeComponent(Entity entity, ComponentTypeInfo type) {
	Archetype* src = entityMap.at(entity);
	std::vector<ComponentTypeInfo> destTypes = src->getTypes();

	auto it = std::find(destTypes.begin(), destTypes.end(), type);
	if (it != destTypes.end()) {
		destTypes.erase(it);

		Archetype* dest = getArchetype(destTypes);
		ChunkEntityHandle owner = moveEntity(entity, src, dest);
		prepEntity(entity, owner, dest);
	}
}

Component* EntityManager::getComponent(Entity entity, ComponentTypeID id) {
	auto it = entityMap.find(entity);
	if (it == entityMap.end()) return nullptr;

	Archetype* archetype = it->second;
	return archetype->getComponent(entity, id);
}

bool EntityManager::hasComponent(Entity entity, ComponentTypeID id) {
	auto it = entityMap.find(entity);
	if (it == entityMap.end()) return false;

	Archetype* archetype = it->second;
	std::vector<ComponentTypeID> typeIDs = archetype->getTypeIDs();
	for (ComponentTypeID& typeID : typeIDs) {
		if (typeID == id) return true;
	}
	return false;
}


Archetype* EntityManager::getArchetype(std::vector<ComponentTypeInfo> types) {
	for (Archetype* archetype : archetypes) {
		if (archetype->match(types))
			return archetype;
	}
	return createArchetype(types);
}

Archetype* EntityManager::createArchetype(std::vector<ComponentTypeInfo> types) {
	archetypes.emplace_back(new Archetype(types));
	return archetypes.back();
}

/* Removes Archetypes from vector archetypes and notifies listeners. This deletes all entities and components currently contained by the Archetype. */
void EntityManager::removeArchetype(Archetype* archetype) {
	for (std::size_t i = 0; i < archetypes.size(); i++) {
		if (archetypes[i] == archetype) {
			//Removes archetype at i
			std::swap(archetypes[i], archetypes.back());
			archetypes.pop_back();
		}
	}
	delete archetype;
}

std::vector<Component*> EntityManager::getComponents(Entity entity) {
	Archetype* archetype = entityMap.at(entity);
	return archetype->getComponents(entity);
}

ChunkEntityHandle EntityManager::moveEntity(Entity entity, Archetype* src, Archetype* dest) {
	ChunkEntityHandle owner = dest->addEntity(entity);
	dest->copyEntity(entity, src->getComponentDataBlocks(entity));
	removeEntity(entity, false);
	entityMap.insert(std::make_pair(entity, dest));
	return owner;
}

void EntityManager::clear() {
	// Clears and deletes all archetypes
	for (Archetype* archetype : archetypes) {
		archetype->clear();
		delete archetype;
	}
	// Clears all collections
	archetypes.clear();
	entityMap.clear();
}