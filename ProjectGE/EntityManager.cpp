#include "EntityManager.h"
#include "ParentEntity.h"
#include "ChildManager.h"
#include "Handle.h"

using namespace Core;

/* Returns invalid handle if entity does not have ParentEntity component or if the parent is nullptr. */
Handle EntityManager::getParent(Entity entity) {
	ParentEntity* parent = getComponent<ParentEntity>(entity);
	if (parent) { // If component exists
		if (parent->getParent()->isValid()) {
			return *parent->getParent();
		}
	}
	return Handle();
}

/* Returns nullptr if entity does not have ChildRefCollection component or if the index is out of bounds. */
Handle* EntityManager::getChild(Entity entity, int index) {
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

EntityLocation EntityManager::getLocation(Entity entity) {
	auto it = entityMap.find(entity);
	if (it == entityMap.end()) return EntityLocation(); // Returns invalid location

	return it->second->getLocation(entity);
}

Handle EntityManager::getEntityHandle(Entity entity) {
	Handle handle = Handle(entity, this);
	handle.update();
	return handle;
}

void EntityManager::destroyEntity(Entity entity) {
	removeEntity(entity, true);
}

/* Removes specified entity and its components from the Archetype that stores it. Destroy shows if the Entity is being moved or not. */
void EntityManager::removeEntity(Entity entity, bool destroy) {
	// Exception check
	auto it = entityMap.find(entity);
	if (it == entityMap.end()) {
		std::cout << "EntityManager::removeEntity::ERROR The entity does not exist in this manager!\n";
		throw std::invalid_argument("EntityManager::removeEntity::ERROR The entity does not exist in this manager!");
	}

	// Destroy children first. The children will destroy their own children.
	if (destroy) {
		std::size_t childCount = getChildCount(entity);
		for (std::size_t i = childCount; i-- != 0;) { // Destroy from back to start
			Handle* child = getChild(entity, i);
			if (child) {
				destroyEntity(child->getEntity());
			}
		}

		// Notify parents
		Handle parent = getParent(entity);
		while (parent.isValid()) {
			Entity currentParent = parent.getEntity();

			ChildManager* childManager = getComponent<ChildManager>(currentParent);
			if (childManager) childManager->childRemoved(entity);

			// Prep for next iteration
			parent = getParent(currentParent);

			// Remove child ref container if no children exist
			if (getChildCount(currentParent) == 0) {
				removeComponent<ChildManager>(currentParent);
			}
		}
	}

	// Remove
	Archetype* archetype = it->second;

	std::vector<Component*> components = archetype->getComponents(entity);
	for (Component* component : components) {
		// Notify components that they are about to be removed from its current location
		component->end();
		// If the Entity is being destroyed, destroy its components
		// The chunk will call the destructors of all components if the whole Entity is being destroyed
		if (!destroy && component->isDestroyed()) {
			component->~Component();
		}
	}
	entityMap.erase(it);
	archetype->removeEntity(entity, destroy);

	if (archetype->isEmpty()) {
		removeArchetype(archetype);
	}
}

/* Prepares recently added entity to be used by the systems. */
void EntityManager::prepEntity(Entity entity, Handle owner, Archetype* target) {
	// Set owner
	std::vector<Component*> components = target->getComponents(entity);
	for (Component* component : components) {
		component->setOwner(owner);
		component->init();
	}

	// Update parent ref in Children
	std::size_t childCount = getChildCount(entity);
	for (std::size_t i = 0; i < childCount; i++) {
		Handle* child = getChild(entity, i);
		if (child) {
			ParentEntity* parentCmp = child->getComponent<ParentEntity>();
			if (parentCmp) {
				parentCmp->setParent(owner); // Update ref
			}
		}
	}

	// Notify parents
	ParentEntity* parentCmp = getComponent<ParentEntity>(entity);
	if (parentCmp) {
		Handle parent = *parentCmp->getParent();
		if (!parent.isValid()) { // If parent is invalid. Remove the Entity
			destroyEntity(entity);
		}
		else {
			while (parent.isValid()) {
				Entity currentParent = parent.getEntity();
				if (!hasComponent<ChildManager>(currentParent)) { // Check if childManager exists
					addComponent(currentParent, new ChildManager());
					parentCmp->setParent(getEntityHandle(currentParent));
				}
				ChildManager* childManager = getComponent<ChildManager>(currentParent);
				if (childManager) {
					childManager->childAdded(owner); // Notify parent
					std::size_t childCount = getChildCount(currentParent);
				}
				else {
					std::cout << "EntityManager::prepEntity::ERROR Failed to add ChildManager!" << "\n";
					throw std::invalid_argument("EntityManager::prepEntity::ERROR Failed to add ChildManager!");
				}
				// Prep for next iteration
				parent = getParent(currentParent);
			}
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
		Handle owner = moveEntity(entity, src, dest);
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

Handle EntityManager::moveEntity(Entity entity, Archetype* src, Archetype* dest) {
	Handle owner = Handle(entity, this);
	EntityLocation location = dest->addEntity(entity);
	owner.updateLocation(location);
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