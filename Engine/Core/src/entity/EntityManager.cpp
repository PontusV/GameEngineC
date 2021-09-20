#include "EntityManager.h"
#include "component/IComponentData.h"
#include "components/entity/DisabledComponent.h"
#include "ReflectionPolymorph.generated.h"

#include "entity/handle/Handle.h"
#include "event/EventSystem.h"
#include "event/events/EntityActivatedEvent.h"
#include "event/events/EntityDeactivatedEvent.h"
#include "event/events/EntityChangedArchetypeEvent.h"
#include "event/events/EntityCreatedEvent.h"
#include "event/events/EntityDestroyedEvent.h"
#include "event/events/EntityDestroyEvent.h"

#include "maths/MatrixTransform.h"

using namespace Core;


std::vector<Entity> EntityManager::getAllEntities() {
	std::vector<Entity> result;
	for (std::shared_ptr<Archetype> archetype : archetypes) {
		std::vector<Entity> entities = archetype->getAllEntities();
		result.insert(result.end(), entities.begin(), entities.end());
	}
	return result;
}

Entity EntityManager::generateEntity() {
	Entity entity(entityIDCounter++);
	return entity;
}

EntityLocation EntityManager::getLocation(Entity entity) {
	for (std::shared_ptr<Archetype> archetype : archetypes) {
		auto location = archetype->getLocation(entity);
		if (location.isValid()) {
			return location;
		}
	}
	return EntityLocation(); // Returns invalid location
}


EntityLocationDetailed EntityManager::getLocationDetailed(Entity entity) {
	for (std::shared_ptr<Archetype>& archetype : archetypes) {
		auto location = archetype->getLocation(entity);
		if (location.isValid()) {
			return EntityLocationDetailed(location.index, location.chunk, archetype);
		}
	}
	return EntityLocationDetailed(); // Returns invalid location
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

bool EntityManager::destroyEntity(Entity entity) {
	// Exception check
	EntityLocationDetailed location = getLocationDetailed(entity);
	return destroyEntity(location);
}

void EntityManager::destroyEntityQueued(Entity entity) {
	functionQueue.push_back(new FunctionCaller<bool, EntityManager, Entity>(&EntityManager::destroyEntity, *this, entity));
}

bool EntityManager::destroyEntity(EntityLocationDetailed location) {
	if (!location.isValid()) {
		return false;
		//std::cout << "EntityManager::removeEntity::ERROR The entity does not exist in this manager!\n";
		//throw std::invalid_argument("EntityManager::removeEntity::ERROR The entity does not exist in this manager!");
	}

	auto chunkPtr = location.chunk.lock();
	auto archetypePtr = location.archetype.lock();
	Entity entity = chunkPtr->getEntity(location.index);
	if (entity.getID() == Entity::INVALID_ID) {
		return false;
	}
	Handle entityHandle = Handle(entity, this, location);
	EntityDestroyEvent preEvent;
	preEvent.entityHandle = Handle(entity, this, location);
	eventSystem->dispatchEvent(preEvent);

	// Destroy immediate children (will chain down)
	std::size_t childCount = entityHandle.getImmediateChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = entityHandle.getChild(i);
		child.destroyImmediate();
	}

	// Check for parent & call onChildDestroyed
	Handle parent = entityHandle.getParent();
	if (parent.refresh()) {
		for (Behaviour* behaviour : parent.getComponentsUpwards<Behaviour>()) {
			behaviour->onChildRemoved(entityHandle);
		}
		if (ChildManager* childManager = parent.getComponent<ChildManager>()) {
			childManager->onChildRemoved(entityHandle);
			if (childManager->getChildCount() == 0) {
				parent.removeComponentImmediate(ChildManager::getClassTypeID());
			}
		}
	}

	// Remove Entity from chunk
	chunkPtr->remove(location.index);
	if (archetypePtr->isEmpty()) {
		removeArchetype(archetypePtr);
	}

	EntityDestroyedEvent postEvent;
	postEvent.entity = entity;
	eventSystem->dispatchEvent(postEvent);

	if (entitiesChangedCallback) {
		entitiesChangedCallback(callbackPtr, entity.getID());
	}

	// Remove hideflags entry
	auto iterator = entityHideFlags.find(entity);
	if (iterator == entityHideFlags.end()) {
		std::cout << "EntityManager::removeEntity::ERROR There was an error when trying to remove the hideflags of Entity: " << entity.getID() << std::endl;
		throw std::invalid_argument("EntityManager::removeEntity::ERROR The Entity does not have hideflags!");
	}
	iterator = entityHideFlags.erase(iterator);
	return true;
}

EntityLocationDetailed EntityManager::removeComponent(Entity entity, ComponentTypeID typeID) {
	EntityLocationDetailed location = getLocationDetailed(entity);
	return removeComponent(location, typeID);
}

void EntityManager::removeComponentQueued(Entity entity, ComponentTypeID componentTypeID) {
	functionQueue.push_back(new FunctionCaller<EntityLocationDetailed, EntityManager, Entity, ComponentTypeID>(&EntityManager::removeComponent, *this, entity, componentTypeID));
}

EntityLocationDetailed EntityManager::removeComponent(EntityLocationDetailed location, ComponentTypeID typeID) {
	if (!location.isValid()) {
		std::cout << "EntityManager::removeComponent::ERROR Cannot remove component. The given location is invalid" << std::endl;
		return EntityLocationDetailed(); // Returns invalid location
		//throw std::invalid_argument("EntityManager::removeComponent::ERROR Cannot remove component. The given location is invalid");
	}
	Entity entity = location.chunk.lock()->getEntity(location.index);
	auto src = location.archetype.lock();
	std::vector<IComponentTypeInfo> srcTypes = src->getTypes();

	auto it = std::find(srcTypes.begin(), srcTypes.end(), typeID);
	if (it == srcTypes.end()) {
		std::cout << "EntityManager::removeComponent::ERROR Cannot remove component of type " << typeID << " from Entity " << entity.getID() << std::endl;
		throw std::invalid_argument("EntityManager::removeComponent::ERROR Cannot remove component");
	}

	srcTypes.erase(it);
	std::shared_ptr<Archetype> dest = getArchetype(srcTypes);
	return moveEntity(entity, src, dest);
}

EntityLocationDetailed EntityManager::addComponent(Entity entity, ComponentTypeID typeID) {
	EntityLocationDetailed location = getLocationDetailed(entity);
	return addComponent(location, typeID);
}

void EntityManager::addComponentQueued(Entity entity, ComponentTypeID componentTypeID) {
	functionQueue.push_back(new FunctionCaller<EntityLocationDetailed, EntityManager, Entity, ComponentTypeID>(&EntityManager::addComponent, *this, entity, componentTypeID));
}

EntityLocationDetailed EntityManager::addComponent(EntityLocationDetailed location, ComponentTypeID typeID) {
	if (!location.isValid()) {
		std::cout << "EntityManager::addComponent::ERROR Cannot add component. The given location is invalid" << std::endl;
		return EntityLocationDetailed(); // Returns invalid location
		//throw std::invalid_argument("EntityManager::addComponent::ERROR Cannot add component. The given location is invalid");
	}
	Entity entity = location.chunk.lock()->getEntity(location.index);
	auto src = location.archetype.lock();
	std::vector<IComponentTypeInfo> destTypes = src->getTypes();
	destTypes.push_back(IComponentTypeInfo(ComponentType(typeID), Mirror::getName(typeID), Mirror::getSize(typeID)));

	std::shared_ptr<Archetype> dest = getArchetype(destTypes);
	EntityLocationDetailed newLocation = moveEntity(entity, src, dest);

	dest->setComponent(entity, typeID);
	return newLocation;
}

IComponentData* EntityManager::getComponent(Entity entity, ComponentType type) {
	EntityLocation location = getLocation(entity);
	if (!location.isValid()) {
		return nullptr;
	}

	return location.chunk.lock()->getComponent(entity, type);
}

IComponentData* EntityManager::getComponent(Entity entity, ComponentTypeID typeID) {
	EntityLocation location = getLocation(entity);
	if (!location.isValid()) {
		return nullptr;
	}

	return location.chunk.lock()->getComponent(entity, typeID);
}

bool EntityManager::hasComponent(Entity entity, ComponentType type) {
	EntityLocationDetailed location = getLocationDetailed(entity);
	return hasComponent(location, type);
}


bool EntityManager::hasComponent(EntityLocationDetailed location, ComponentType type) {
	if (!location.isValid()) {
		return false;
	}
	auto archetype = location.archetype.lock();
	std::vector<ComponentTypeID> typeIDs = archetype->getTypeIDs();
	for (ComponentTypeID& typeID : typeIDs) {
		if (type == typeID) return true;
	}
	return false;
}

std::vector<IComponentTypeInfo> EntityManager::getComponentTypes(Entity entity) {
	for (std::shared_ptr<Archetype>& archetype : archetypes) {
		if (archetype->containsEntity(entity)) {
			return archetype->getTypes();
		}
	}
	return std::vector<IComponentTypeInfo>();
}


std::shared_ptr<Archetype> EntityManager::getArchetype(std::vector<IComponentTypeInfo> types) {
	// Type ID List
	std::vector<ComponentTypeID> typeIDs;
	//typeIDs.reserve(types.size());

	for (IComponentTypeInfo& info : types) {
		typeIDs.push_back(info.type.getTypeID());
	}

	// Match check
	for (std::shared_ptr<Archetype>& archetype : archetypes) {
		if (archetype->match(typeIDs))
			return archetype;
	}
	return createArchetype(types);
}

std::shared_ptr<Archetype> EntityManager::createArchetype(std::vector<IComponentTypeInfo> types) {
	archetypes.push_back(std::make_shared<Archetype>(types));
	return archetypes.back();
}

/* Removes Archetypes from vector archetypes and notifies listeners. This deletes all entities and components currently contained by the Archetype. */
void EntityManager::removeArchetype(std::shared_ptr<Archetype> archetype) {
	for (std::size_t i = 0; i < archetypes.size(); i++) {
		if (archetypes[i] == archetype) {
			//Removes archetype at i
			std::swap(archetypes[i], archetypes.back());
			archetypes.pop_back();
		}
	}
}

std::vector<IComponentData*> EntityManager::getComponents(Entity entity) {
	EntityLocation location = getLocation(entity);
	if (!location.isValid()) {
		return std::vector<IComponentData*>();
	}

	return location.chunk.lock()->getComponents(entity);
}

EntityLocationDetailed EntityManager::moveEntity(Entity entity, std::shared_ptr<Archetype> src, std::shared_ptr<Archetype> dest) {
	std::shared_ptr<Chunk> chunk = src->getContainer(entity);
	EntityLocation basicLoc = dest->moveEntity(entity, chunk->getComponentDataBlocks(entity));
	EntityLocationDetailed location = EntityLocationDetailed(basicLoc.index, basicLoc.chunk, dest);

	src->removeEntity(entity);
	if (src->isEmpty()) {
		removeArchetype(src);
	}

	EntityChangedArchetypeEvent event;
	event.entityHandle = Handle(entity, this, location);
	eventSystem->dispatchEvent(event);

	return location;
}

void EntityManager::clear() {
	entityIDCounter = 0;
	// Clears and deletes all stored function calls
	while (!functionQueue.empty()) {
		delete functionQueue.front();
		functionQueue.pop_front();
	}
	// Clears all archetypes
	for (std::shared_ptr<Archetype>& archetype : archetypes) {
		archetype->clear();
	}
	// Clears all collections
	archetypes.clear();
	entityHideFlags.clear();
}

void EntityManager::setEntitiesChangedCallback(EntitiesChangedCallbackFun callback) {
	entitiesChangedCallback = callback;
}

void EntityManager::dispatchEntityAddedEvent(Entity entity, EntityLocationDetailed location) {
	EntityCreatedEvent event;
	event.entityHandle = Handle(entity, this, location);
	eventSystem->dispatchEvent(event);
}

bool EntityManager::activate(Entity entity) {
	EntityLocationDetailed location = getLocationDetailed(entity);
	return deactivate(location);
}

bool EntityManager::deactivate(Entity entity) {
	EntityLocationDetailed location = getLocationDetailed(entity);
	return deactivate(location);
}

void EntityManager::activateQueued(Entity entity) {
	functionQueue.push_back(new FunctionCaller<bool, EntityManager, Entity>(&EntityManager::activate, *this, entity));
}

void EntityManager::deactivateQueued(Entity entity) {
	functionQueue.push_back(new FunctionCaller<bool, EntityManager, Entity>(&EntityManager::deactivate, *this, entity));
}

bool EntityManager::activate(EntityLocationDetailed location) {
	if (location.isValid() && isActive(location)) {
		Entity entity = location.chunk.lock()->getEntity(location.index);
		EntityLocationDetailed newLocation = removeComponent<DisabledComponent>(location);
		EntityActivatedEvent event;
		event.entityHandle = Handle(entity, this, newLocation);
		eventSystem->dispatchEvent(event);

		// Activates all children
		Handle entityHandle = Handle(entity, this, newLocation);
		std::size_t childCount = entityHandle.getChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle child = entityHandle.getChild(i);
			child.activate();
		}
		return true;
	}
	return false;
}

bool EntityManager::deactivate(EntityLocationDetailed location) {
	if (!location.isValid() || isActive(location)) {
		return false;
	}
	Entity entity = location.chunk.lock()->getEntity(location.index);
	EntityLocationDetailed newLocation = addComponent<DisabledComponent>(entity);

	EntityDeactivatedEvent event;
	event.entityHandle = Handle(entity, this, newLocation);
	eventSystem->dispatchEvent(event);

	// Deactivates all children
	Handle entityHandle = Handle(entity, this, newLocation);
	std::size_t childCount = entityHandle.getChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = entityHandle.getChild(i);
		child.deactivate();
	}
	return true;
}

bool EntityManager::isActive(Entity entity) {
	return hasComponent<DisabledComponent>(entity);
}

bool EntityManager::isActive(EntityLocationDetailed location) {
	return hasComponent<DisabledComponent>(location);
}

void EntityManager::setParent(Entity entity, Entity parent, bool keepPosition) {
	if (entity == parent) {
		std::cout << "EntityManager::setParent::ERROR Cannot make an Entity a parent/child of itself!" << std::endl;
		return;
	}
	EntityLocationDetailed location = getLocationDetailed(entity);
	return setParent(location, parent, keepPosition);
}

void EntityManager::setParent(EntityLocationDetailed location, Entity parent, bool keepPosition) {
	if (!location.isValid()) {
		std::cout << "EntityManager::setParent::ERROR Cannot set parent of Entity at invalid location" << std::endl;
		return;
	}
	Entity entity = location.chunk.lock()->getEntity(location.index);
	if (entity == parent) {
		std::cout << "EntityManager::setParent::ERROR Unable to self as parent" << std::endl;
		return;
	}
	Handle entityHandle = Handle(entity, this, location);
	Handle parentHandle = Handle(parent, this);
	if (entityHandle.isChild(parent)) {
		std::cout << "EntityManager::setParent::ERROR Unable to set a child as parent" << std::endl;
		return;
	}

	// Notify previous parent of removal & remove
	Handle currentParent = entityHandle.getParent();
	if (currentParent.refresh()) {
		for (Behaviour* behaviour : currentParent.getComponentsUpwards<Behaviour>()) {
			behaviour->onChildRemoved(entityHandle);
		}

		if (ChildManager* childManager = currentParent.getComponent<ChildManager>()) {
			childManager->onChildRemoved(entityHandle);
			if (childManager->getChildCount() == 0)
				removeComponent<ChildManager>(currentParent.getEntity());
		}
	}

	if (parent.getID() != Entity::INVALID_ID) {
		// Check if parent is deactivated
		if (!parentHandle.isActive() && entityHandle.isActive()) {
			entityHandle.deactivate();
		}
		// Add ChildManager to new parent if none already exists
		ChildManager* childManager = parentHandle.getComponent<ChildManager>();
		if (!childManager) {
			ChildManager newChildManager;
			addComponent<ChildManager>(parent, newChildManager);
			childManager = parentHandle.getComponent<ChildManager>();
		}
		childManager->onChildAdded(entityHandle);

		// Modify ParentEntity component
		ParentEntity* parentComponent = entityHandle.getComponent<ParentEntity>();
		if (!parentComponent) {
			ParentEntity component;
			location = addComponent<ParentEntity>(location, component);
			parentComponent = entityHandle.getComponent<ParentEntity>();
		}
		parentComponent->setParent(parentHandle);

		// Notify new parent of added child
		for (Behaviour* behaviour : parentHandle.getComponentsUpwards<Behaviour>()) {
			behaviour->onChildAdded(entityHandle);
		}
	}
	else if (entityHandle.hasComponent<ParentEntity>()) {
		location = removeComponent<ParentEntity>(location);
	}
	if (entitiesChangedCallback) {
		entitiesChangedCallback(callbackPtr, entity.getID());
		entitiesChangedCallback(callbackPtr, parent.getID());
	}
	if (!keepPosition) return;
	if (Transform* transform = entityHandle.getComponent<Transform>()) {
		if (parent.getID() != Entity::INVALID_ID) {
			if (Transform* parentTransform = parentHandle.getComponent<Transform>()) {
				Vector2 position = transform->getPosition();
				Vector2 newPosition = parentTransform->getLocalToWorldMatrix() * parentTransform->getLocalModelMatrix() * transform->getLocalPosition();
				transform->moveX(position.x - newPosition.x);
				transform->moveY(position.y - newPosition.y);
			}
		}
		else {
			Vector2 position = transform->getPosition();
			Vector2 newPosition = transform->getLocalPosition();
			transform->moveX(position.x - newPosition.x);
			transform->moveY(position.y - newPosition.y);
		}
		transform->setChanged();
	}
}

void EntityManager::setParentQueued(Entity entity, Entity parent, bool keepPosition) {
	functionQueue.push_back(new FunctionCaller<void, EntityManager, Entity, Entity, bool>(&EntityManager::setParent, *this, entity, parent, keepPosition));
}

void EntityManager::setSiblingIndex(Entity entity, std::size_t index) {
	EntityLocationDetailed location = getLocationDetailed(entity);
	return setSiblingIndex(location, index);
}

void EntityManager::setSiblingIndex(EntityLocationDetailed location, std::size_t index) {
	if (!location.isValid()) {
		return;
	}

	Entity entity = location.chunk.lock()->getEntity(location.index);
	Handle entityHandle = Handle(entity, this, location);
	Handle parent = entityHandle.getParent();

	if (!parent.isValid()) {
		std::cout << "EntityManager::setSiblingIndex::ERROR Cannot set the sibling index of an Entity without a parent" << std::endl;
		return;
	}

	ChildManager* childManager = parent.getComponent<ChildManager>();
	if (childManager) {
		std::vector<Handle>& children = childManager->getChildren();
		auto it = std::find(children.begin(), children.end(), entityHandle);
		it = children.erase(it);
		if (it - children.begin() < index) index--;
		children.insert(children.begin() + index, entityHandle);

		if (entitiesChangedCallback) {
			entitiesChangedCallback(callbackPtr, entity.getID());
		}
	}
}

void EntityManager::setSiblingIndexQueued(Entity entity, std::size_t index) {
	functionQueue.push_back(new FunctionCaller<void, EntityManager, Entity, std::size_t>(&EntityManager::setSiblingIndex, *this, entity, index));
}

void EntityManager::setCallbackPtr(void* ptr) {
	callbackPtr = ptr;
}

void EntityManager::processQueue() {
	entityQueueMap.clear();
	while (!functionQueue.empty()) {
		IFunctionCaller* fun = functionQueue.front();
		fun->call();
		delete fun;
		functionQueue.pop_front();
	}
}

std::size_t EntityManager::getTypeID(IComponentData* component) {
	return component->getTypeID();
}

bool EntityManager::setEntityIDCounter(std::size_t value) {
	if (entityIDCounter > value) {
		return false;
	}
	entityIDCounter = value;
	return true;
}

std::size_t EntityManager::getEntityIDCounter() const {
	return entityIDCounter;
}