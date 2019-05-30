#include "EntityManager.h"
#include "ParentEntity.h"
#include "ChildManager.h"
#include "Handle.h"
#include "Behaviour.h"

using namespace Core;


Entity EntityManager::generateEntity(std::string name) {
	Entity entity(entityIDCounter++);
	// Check if name is already taken
	auto it = entityNameMap.find(name);
	if (it != entityNameMap.end()) {
		std::cout << "Entity with the name " << name << " already exists!\n";
		throw std::invalid_argument("Entity with given name already exist!");
	}
	entityNameMap[name] = entity;
	return entity;
}

void EntityManager::awake() {
	if (isAwake) return;
	isAwake = true;
	// Loop through all entities
	for (auto it = entityMap.begin(); it != entityMap.end(); it++) {
		awakeEntity(it->first);
	}
}

void EntityManager::awakeEntity(Entity entity) {
	if (!isAwake) return; // This instance has to be awake first
	std::vector<Behaviour*> scripts = getComponents<Behaviour>(entity);
	for (Behaviour* script : scripts) {
		script->awake();
	}
}

void EntityManager::awakeComponent(Entity entity, ComponentType type) {
	if (!isAwake) return; // This instance has to be awake first
	// Awake script
	if (type == typeof(Behaviour)) {
		Behaviour* script = static_cast<Behaviour*>(getComponent(entity, type));
		if (script) {
			script->awake();
		}
	}
}

/* Returns invalid handle if entity does not have ParentEntity component or if the parent is nullptr. */
Handle EntityManager::getParent(Entity entity) {
	ParentEntity* parentCmp = getComponent<ParentEntity>(entity);
	if (parentCmp) { // If component exists
		if (parentCmp->getParent().refresh()) {
			return parentCmp->getParent();
		}
	}
	return Handle();
}

/* Returns nullptr if entity does not have ChildRefCollection component or if the index is out of bounds. */
Handle EntityManager::getChild(Entity entity, int index) {
	ChildManager* childRefs = getComponent<ChildManager>(entity);
	if (childRefs) { // If component exists
		return childRefs->getChild(index);
	}
	return Handle();
}

std::size_t	EntityManager::getImmediateChildCount(Entity entity) {
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
Handle EntityManager::getEntityHandle(std::string entityName) {
	Entity& entity = entityNameMap[entityName];
	return getEntityHandle(entity);
}
std::string EntityManager::getEntityName(Entity entity) {
	auto it = entityNameMap.begin();
	while (it != entityNameMap.end()) {
		if (it->second == entity) return it->first;
		it++;
	}
	std::cout << "EntityManager::getEntityName::ERROR There is no such entity stored in this Level!\n";
	throw std::invalid_argument("EntityManager::getEntityName::ERROR There is no such entity stored in this Level!");
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
		bool updateIterator = getImmediateChildCount(entity) > 0;
		while (getImmediateChildCount(entity) > 0) {
			destroyEntity(getChild(entity, 0).getEntity());
		}
		// Update the iterator if children were removed.
		if (updateIterator) {
			it = entityMap.find(entity);
		}

		// Erase Entity name
		auto iterator = entityNameMap.find(getEntityName(entity));
		if (iterator == entityNameMap.end()) throw "EntityManager::removeEntity::ERROR The Entity does not have a name!";
		entityNameMap.erase(iterator);
	}

	// Notify parent
	Handle parent = getParent(entity);
	ChildManager* childManager = parent.getComponent<ChildManager>();
	if (childManager) {
		childManager->childRemoved(entity);
		// Remove child ref container if no children exist
		if (childManager->getChildCount() == 0) {
			removeComponent<ChildManager>(parent.getEntity());
		}
	}

	// Remove
	Archetype* archetype = it->second;

	std::vector<Component*> components = archetype->getComponents(entity);
	for (Component* component : components) {
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
	}

	// Update parent ref in Children
	std::size_t childCount = getImmediateChildCount(entity);
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = getChild(entity, i);
		if (child.refresh()) {
			ParentEntity* parentCmp = child.getComponent<ParentEntity>();
			if (parentCmp) {
				parentCmp->setParent(owner); // Update ref
			}
		}
	}

	// Notify parent
	ParentEntity* parentCmp = getComponent<ParentEntity>(entity);
	if (parentCmp) {
		Handle parent = parentCmp->getParent();
		if (parent.refresh()) {
			Entity currentParent = parent.getEntity();
			ChildManager* childManager = getComponent<ChildManager>(currentParent);
			if (childManager == nullptr) {
				ChildManager newChildManager;
				addComponent(currentParent, newChildManager);
				childManager = getComponent<ChildManager>(currentParent);
				parentCmp->getParent().update();
			}
			childManager->childAdded(owner); // Notify parent
		}
		else { // If parent is invalid. Throw exception
			std::cout << "EntityManager::prepEntity::ERROR Cannot add invalid Parent to Entity(" << entity.getID() << ")\n";
			throw std::invalid_argument("EntityManager::prepEntity::ERROR Cannot add invalid Parent!");
		}
	}
}

void EntityManager::removeComponent(Entity entity, ComponentTypeID typeID) {
	Archetype* src = entityMap.at(entity);
	std::vector<IComponentTypeInfo> srcTypes = src->getTypes();

	auto it = std::find(srcTypes.begin(), srcTypes.end(), typeID);
	if (it != srcTypes.end()) {
		srcTypes.erase(it);

		Archetype* dest = getArchetype(srcTypes);
		Handle owner = moveEntity(entity, src, dest);
		prepEntity(entity, owner, dest);
	}
}

Component* EntityManager::getComponent(Entity entity, ComponentType type) {
	auto it = entityMap.find(entity);
	if (it == entityMap.end()) return nullptr;

	Archetype* archetype = it->second;
	return archetype->getComponent(entity, type);
}

bool EntityManager::hasComponent(Entity entity, ComponentType type) {
	auto it = entityMap.find(entity);
	if (it == entityMap.end()) return false;

	Archetype* archetype = it->second;
	std::vector<ComponentTypeID> typeIDs = archetype->getTypeIDs();
	for (ComponentTypeID& typeID : typeIDs) {
		if (type == typeID) return true;
	}
	return false;
}

std::vector<IComponentTypeInfo> EntityManager::getComponentTypes(Entity entity) {
	auto it = entityMap.find(entity);
	if (it == entityMap.end()) return std::vector<IComponentTypeInfo>();

	return it->second->getTypes();
}


Archetype* EntityManager::getArchetype(std::vector<IComponentTypeInfo> types) {
	// Type ID List
	std::vector<ComponentTypeID> typeIDs;
	typeIDs.reserve(types.size());

	for (IComponentTypeInfo& info : types) {
		typeIDs.push_back(info.type.getTypeID());
	}

	// Match check
	for (Archetype* archetype : archetypes) {
		if (archetype->match(typeIDs))
			return archetype;
	}
	return createArchetype(types);
}

Archetype* EntityManager::createArchetype(std::vector<IComponentTypeInfo> types) {
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
	// Clears and deletes all stored function calls
	for (IFunctionCaller* caller : functionQueue) {
		delete caller;
	}
	functionQueue.clear();
	// Clears and deletes all archetypes
	for (Archetype* archetype : archetypes) {
		archetype->clear();
		delete archetype;
	}
	// Clears all collections
	archetypes.clear();
	entityMap.clear();
	entityNameMap.clear();
}

void EntityManager::processQueue() {
	for (IFunctionCaller* caller : functionQueue) {
		caller->call();
		delete caller;
	}
	functionQueue.clear();
}

void EntityManager::destroyEntityQueued(Entity entity) {
	functionQueue.push_back(new FunctionCaller<void, EntityManager, Entity>(&EntityManager::destroyEntity, *this, entity));
}