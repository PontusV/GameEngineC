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
	entityHideFlags[entity] = HideFlags::None;
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
	awakeEntity(getEntityHandle(entity));
}

void EntityManager::awakeEntity(Handle entity) {
	if (!isAwake) return; // This instance has to be awake first
	std::vector<Behaviour*> scripts = entity.getComponents<Behaviour>();
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
void EntityManager::setParent(Handle entityHandle, Handle parentHandle) {
	if (parentHandle.getEntity().getID() == Entity::INVALID_ID) return; // Cannot set Invalid Entity to parent
	Entity entity = entityHandle.getEntity();
	Entity parent = parentHandle.getEntity();
	// Notify previous parent of removal
	Handle currentParent = getParent(entity);
	if (currentParent.refresh()) {
		for (Behaviour* behaviour : currentParent.getComponents<Behaviour>()) {
			behaviour->onChildRemoved(entityHandle);
		}

		ChildManager* childManager = currentParent.getComponent<ChildManager>();
		childManager->onChildRemoved(entityHandle);
		if (childManager->getChildCount() == 0)
			removeComponent<ChildManager>(currentParent.getEntity());
	}

	// Add ChildManager to new parent if none already exists
	if (!hasComponent<ChildManager>(parent)) {
		ChildManager childManager;
		addComponent<ChildManager>(parent, childManager);
	}

	// Modify ParentEntity component
	ParentEntity* parentComponent = getComponent<ParentEntity>(entity);
	if (!parentComponent) {
		ParentEntity component;
		addComponent<ParentEntity>(entity, component);
		parentComponent = getComponent<ParentEntity>(entity);
	}
	parentComponent->setParent(parentHandle);

	// Notify new parent of added child
	parentHandle.getComponent<ChildManager>()->onChildAdded(entityHandle);
	for (Behaviour* behaviour : parentHandle.getComponentsUpwards<Behaviour>()) {
		behaviour->onChildAdded(entityHandle);
	}
}

void EntityManager::onEntityCreated(Entity entity) {
	onEntityCreated(getEntityHandle(entity));
}

void EntityManager::onEntityCreated(Handle entity) {
	// Awake the entity
	awakeEntity(entity);

	// Should not have to handle any ParentEntiy component here
}
void EntityManager::onEntityDestroyed(Entity entity) {
	Handle handle = getEntityHandle(entity);
	// Check for parent & call onChildDestroyed
	Handle parent = handle.getParent();
	if (parent.refresh()) {
		parent.getComponent<ChildManager>()->onChildRemoved(handle);
		for (Behaviour* behaviour : parent.getComponentsUpwards<Behaviour>()) {
			behaviour->onChildRemoved(handle);
		}
	}
}

void EntityManager::onEntityChanged(Handle entity) {
	// Check for parent & call onChildChanged
	Handle parent = entity.getParent();
	for (Behaviour* behaviour : parent.getComponentsUpwards<Behaviour>()) {
		behaviour->onChildChanged(entity);
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
	std::cout << "EntityManager::getEntityName::ERROR There is no such entity stored in this Level!" << std::endl;
	throw std::invalid_argument("EntityManager::getEntityName::ERROR There is no such entity stored in this Level!");
}

bool EntityManager::removeEntityName(Entity entity) {
	// Erase Entity name to make it available again
	auto it = entityNameMap.begin();
	while (it != entityNameMap.end()) {
		if (it->second == entity) {
			entityNameMap.erase(it);
			return true;
		}
		it++;
	}
	return false;
}

HideFlags EntityManager::getEntityHideFlags(Entity entity) {
	auto it = entityHideFlags.find(entity);
	if (it == entityHideFlags.end()) {
		std::cout << "EntityManager::getEntityHideFlags::ERROR The Entity does not have any hideflags registered!" << std::endl;
		throw std::invalid_argument("EntityManager::getEntityHideFlags::ERROR The Entity does not have any hideflags registered!");
	}
	return it->second;
}
void EntityManager::setEntityHideFlags(Entity entity, HideFlags hideFlags) {
	auto it = entityHideFlags.find(entity);
	if (it == entityHideFlags.end()) {
		std::cout << "EntityManager::setEntityHideFlags::ERROR The Entity does not have any hideflags registered!" << std::endl;
		throw std::invalid_argument("EntityManager::setEntityHideFlags::ERROR The Entity does not have any hideflags registered!");
	}
	it->second = hideFlags;
}

void EntityManager::destroyEntity(Entity entity) {
	// Exception check
	auto it = entityMap.find(entity);
	if (it == entityMap.end()) {
		std::cout << "EntityManager::destroyEntity::ERROR The entity does not exist in this manager!\n";
		throw std::invalid_argument("EntityManager::destroyEntity::ERROR The entity does not exist in this manager!");
	}
	Archetype* archetype = it->second;
	onEntityDestroyed(entity);

	// Call onDestroy
	std::vector<Behaviour*> scripts = getComponents<Behaviour>(entity);
	for (Behaviour* script : scripts) {
		script->onDestroy();
	}

	// Destroy children first. The children will destroy their own children.
	// Destroy Children
	ChildManager* childManager = getComponent<ChildManager>(entity);
	if (childManager) {
		bool updateIt = childManager->getChildCount() > 0;
		while (childManager->getChildCount() > 0) {
			destroyEntity(getChild(entity, 0).getEntity()); // TODO: Tell it to not remove the ChildManager component to prevent unnecessary work.
		}
		// Update the iterator if children were removed.
		if (updateIt) {
			it = entityMap.find(entity);
		}
	}

	// Remove hideflags entry
	auto iterator = entityHideFlags.find(entity);
	if (iterator == entityHideFlags.end()) {
		std::cout << "EntityManager::removeEntity::ERROR There was an error when trying to remove the hideflags of Entity: " << entity.getID() << std::endl;
		throw std::invalid_argument("EntityManager::removeEntity::ERROR The Entity does not have hideflags!");
	}
	entityHideFlags.erase(iterator);
	removeEntityName(entity);

	removeEntity(entity, archetype);
	entityMap.erase(it);

	archetype->destroyEntity(entity);

	if (archetype->isEmpty()) {
		removeArchetype(archetype);
	}
}

/* Removes specified entity and its components from the Archetype that stores it. */
void EntityManager::removeEntity(Entity entity) {
	// Exception check
	auto it = entityMap.find(entity);
	if (it == entityMap.end()) {
		std::cout << "EntityManager::removeEntity::ERROR The entity does not exist in this manager!\n";
		throw std::invalid_argument("EntityManager::removeEntity::ERROR The entity does not exist in this manager!");
	}
	Archetype* archetype = it->second;

	removeEntity(entity, archetype);
	entityMap.erase(it);

	archetype->removeEntity(entity);

	if (archetype->isEmpty()) {
		removeArchetype(archetype);
	}
}

void EntityManager::removeEntity(Entity entity, Archetype* archetype) {
	// Call destructor on components marked for destruction
	std::vector<Component*> components = archetype->getComponents(entity);
	for (Component* component : components) {
		if (component->isDestroyed()) {
			component->~Component();
		}
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
		onEntityChanged(owner);
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
	removeEntity(entity);
	entityMap.insert(std::make_pair(entity, dest));
	return owner;
}

void EntityManager::clear() {
	// Clears and deletes all stored function calls
	while (!functionQueue.empty()) {
		delete functionQueue.front();
		functionQueue.pop();
	}
	// Clears and deletes all archetypes
	for (Archetype* archetype : archetypes) {
		archetype->clear();
		delete archetype;
	}
	// Clears all collections
	archetypes.clear();
	entityMap.clear();
	entityNameMap.clear();
	entityHideFlags.clear();
}

void EntityManager::processQueue() {
	while (!functionQueue.empty()) {
		IFunctionCaller* fun = functionQueue.front();
		fun->call();
		delete fun;
		functionQueue.pop();
	}
}

void EntityManager::destroyEntityQueued(Entity entity) {
	removeEntityName(entity);
	functionQueue.push(new FunctionCaller<void, EntityManager, Entity>(&EntityManager::destroyEntity, *this, entity));
}

void EntityManager::setParentQueued(Handle entity, Handle parent) {
	functionQueue.push(new FunctionCaller<void, EntityManager, Handle, Handle>(&EntityManager::setParent, *this, entity, parent));
}