#include "Handle.h"
#include "entity/EntityManager.h"
#include "entity/Chunk.h"
#include "entity/component/IComponentData.h"

#include "components/entity/ParentEntity.h"
#include "components/entity/ChildManager.h"
#include "ReflectionPolymorph.generated.h"

#include <algorithm>

using namespace Core;

Handle::Handle(Entity entity, EntityManager* entityManager) : entity(entity), entityManager(entityManager) {
}

Handle::Handle(Entity entity, EntityManager* entityManager, EntityLocationDetailed locationData) : entity(entity), entityManager(entityManager), locationData(locationData) {
}

Handle::Handle() : entity(0) {
} // Creates invalid handle


Handle::~Handle() {
}

bool Handle::operator==(const Handle& other) const {
	return entity == other.entity;
}

bool Handle::operator!=(const Handle& other) const {
	return !(*this == other);
}

const Entity& Handle::getEntity() const {
	return entity;
}

EntityManager* Handle::getEntityManager() const {
	return entityManager;
}

void Handle::activate() {
	entityManager->activateQueued(entity);
}

void Handle::deactivate() {
	entityManager->deactivateQueued(entity);
}

bool Handle::isActive() {
	if (refresh()) {
		entityManager->isActive(locationData);
	}
	return false;
}

void Handle::setSiblingIndex(std::size_t index) {
	entityManager->setSiblingIndex(locationData, index);
}

void Handle::setSiblingIndexQueued(std::size_t index) {
	entityManager->setSiblingIndexQueued(entity, index);
}

std::size_t Handle::getSiblingIndex() {
	Handle parent = getParent();
	if (parent.isValid()) {
		// Entity
		std::size_t childCount = parent.getImmediateChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			if (parent.getChild(i).getEntity() == entity) {
				return i;
			}
		}
	}
	else {
		std::cout << "Handle::getSiblingIndex::ERROR Cannot get the sibling index of a root object!" << std::endl;
	}
	return 0;
}

std::size_t Handle::getDepth() {
	Handle parent = getParent();
	std::size_t depth = 0;
	while (parent.isValid()) {
		parent = parent.getParent();
		depth++;
	}
	return depth;
}

void Handle::clear() {
	entity = Entity(0);
	entityManager = nullptr;
}

bool Handle::isValid() {
	/*if (auto chunk = locationData.chunk.lock()) {
		return chunk->getEntity(locationData.index) == entity;
	}
	return false;*/
	return entity.getID() != Entity::INVALID_ID;
}

bool Handle::isUpdated() {
	if (auto chunk = locationData.chunk.lock()) {
		return chunk->getEntity(locationData.index) == entity;
	}
	return false;
}

bool Handle::refresh() {
	if (isUpdated()) return true;
	update();
	return isUpdated();
}

void Handle::update() {
	if (entityManager) {
		locationData = entityManager->getLocationDetailed(entity);
	}
}

void Handle::updateLocation(EntityLocationDetailed location) {
	locationData = location;
}

const EntityLocationDetailed& Handle::getLocation() const {
	return locationData;
}

bool Handle::hasParent() {
	return hasComponent<ParentEntity>();
}

bool Handle::isChild(Entity entity) {
	std::size_t childCount = getChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = getChild(i);
		if (child.getEntity() == entity)
			return true;
	}
	return false;
}

bool Handle::isParent(Entity entity) {
	Handle parent = getParent();
	while (parent.refresh()) {
		if (parent.getEntity() == entity) return true;
		parent = parent.getParent();
	}
	return false;
}

bool Handle::isImmediateChild(Entity entity) {
	std::size_t childCount = getImmediateChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = getChild(i);
		if (child.getEntity() == entity)
			return true;
	}
	return false;
}

bool Handle::hasComponent(ComponentType type) {
	return getComponent(type) != nullptr; // Faster than EntityManager::hasComponent
}

bool Handle::hasComponent(ComponentTypeID typeID) {
	return getComponent(typeID) != nullptr; // Faster than EntityManager::hasComponent
}

IComponentData* Handle::getComponent(ComponentTypeID typeID) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponent(locationData.index, typeID);
	}
	return nullptr;
}

IComponentData* Handle::getComponent(ComponentType type) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponent(locationData.index, type);
	}
	return nullptr;
}

std::vector<IComponentData*> Handle::getComponents(ComponentType type) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponents(locationData.index, type);
	}
	return std::vector<IComponentData*>(); // Empty
}

std::vector<IComponentData*> Handle::getComponents(ComponentTypeID typeID) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponents(locationData.index, typeID);
	}
	return std::vector<IComponentData*>(); // Empty
}

std::vector<IComponentData*> Handle::getComponents() {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponents(locationData.index);
	}
	return std::vector<IComponentData*>(); // Empty
}

std::vector<IComponentData*> Handle::getComponentsInImmediateChildren(bool includeInactive) {
	std::vector<IComponentData*> components;
	std::size_t childCount = getImmediateChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = getChild(i);
		if (child.isActive() || includeInactive) {
			std::vector<IComponentData*> childComponents = child.getComponents();
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}
	}
	return components;
}

std::vector<IComponentData*> Handle::getComponentsInChildren(bool includeInactive) {
	std::vector<IComponentData*> components;
	std::size_t childCount = getChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = getChild(i);
		if (child.isActive() || includeInactive) {
			std::vector<IComponentData*> childComponents = child.getComponents();
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}
	}
	return components;
}

std::vector<IComponentData*> Handle::getComponentsInParents() {
	std::vector<IComponentData*> components;
	Handle parent = getParent();
	if (parent.refresh()) {
		std::vector<IComponentData*> parentComponents = parent.getComponents();
		components.insert(components.end(), parentComponents.begin(), parentComponents.end());
		parentComponents = parent.getComponentsInParents();
		components.insert(components.end(), parentComponents.begin(), parentComponents.end());
	}
	return components;
}

std::vector<IComponentData*> Handle::getComponentsUpwards() {
	std::vector<IComponentData*> components = getComponents();
	std::vector<IComponentData*> parentComponents = getComponentsInParents();
	components.insert(components.end(), parentComponents.begin(), parentComponents.end());
	return components;
}

std::vector<IComponentData*> Handle::getComponentsDownwards(bool includeInactive) {
	std::vector<IComponentData*> components = getComponents();
	std::vector<IComponentData*> childComponents = getComponentsInChildren(includeInactive);
	components.insert(components.end(), childComponents.begin(), childComponents.end());
	return components;
}

std::vector<IComponentTypeInfo> Handle::getComponentTypes() {
	std::vector<IComponentData*> components;
	if (refresh()) {
		if (auto archetype = locationData.archetype.lock())
			return archetype->getTypes();
	}
	return std::vector<IComponentTypeInfo>();
}

HideFlags Handle::getEntityHideFlags() {
	HideFlags result = entityManager->getEntityHideFlags(entity);
	Handle parent = getParent();
	while (parent.refresh()) {
		HideFlags hideFlags = parent.getEntityHideFlags();
		result = result | hideFlags;
		parent = parent.getParent();
	}
	return result;
}

void Handle::setEntityHideFlags(HideFlags hideFlags) {
	return entityManager->setEntityHideFlags(entity, hideFlags);
}

void Handle::setParent(const Entity& entity, bool keepPosition) {
	Handle parent = Handle(entity, entityManager);
	setParent(parent, keepPosition);
}

void Handle::setParent(Handle parent, bool keepPosition) {
	entityManager->setParentQueued(entity, parent.getEntity(), keepPosition);
}

void Handle::removeParent(bool keepPosition) {
	entityManager->setParentQueued(entity, Entity(0), keepPosition);
}

Handle Handle::getParent() {
	ParentEntity* parentCmp = static_cast<ParentEntity*>(getComponent(ParentEntity::getClassTypeID())); // Get component with ComponentTypeID is much faster
	if (parentCmp) {
		return parentCmp->getParent();
	}
	return Handle();
}

Handle Handle::getChild(std::size_t index) {
	ChildManager* childManager = static_cast<ChildManager*>(getComponent(ChildManager::getClassTypeID())); // Get component with ComponentTypeID is much faster
	if (childManager) {
		std::size_t immediateChildCount = childManager->getChildCount();
		// Check if child is immediate
		if (index < immediateChildCount) {
			return childManager->getChild(index);
		} // Otherwise check for the child in immediate children
		else {
			index -= immediateChildCount;
			for (std::size_t i = 0; i < immediateChildCount; i++) {
				Handle child = getChild(i);
				std::size_t childChildCount = child.getChildCount();
				if (index < childChildCount) {
					return child.getChild(index);
				}
				else {
					index -= childChildCount;
				}
			}
		}
	}
	return Handle();
}

Entity Handle::getParentEntity() {
	return getParent().getEntity();
}

Entity Handle::getChildEntity(std::size_t index) {
	return getChild(index).getEntity();
}

std::size_t Handle::getImmediateChildCount() {
	ChildManager* childManager = static_cast<ChildManager*>(getComponent(ChildManager::getClassTypeID())); // Get component with ComponentTypeID is much faster
	if (childManager) {
		return childManager->getChildCount();
	}
	return 0;
}

std::size_t Handle::getChildCount() {
	std::size_t immediateChildCount = getImmediateChildCount();
	std::size_t childCount = immediateChildCount;
	for (std::size_t i = 0; i < immediateChildCount; i++) {
		childCount += getChild(i).getChildCount();
	}
	return childCount;
}

void Handle::addComponent(ComponentTypeID componentTypeID) {
	if (refresh()) {
		entityManager->addComponentQueued(entity, componentTypeID);
	}
}

IComponentData* Handle::addComponentImmediate(ComponentTypeID componentTypeID) {
	if (refresh()) {
		EntityLocationDetailed newLocation = entityManager->addComponent(locationData, componentTypeID);
		updateLocation(newLocation);
		return getComponent(componentTypeID);
	}
}

void Handle::removeComponent(ComponentTypeID componentTypeID) {
	if (refresh()) {
		entityManager->removeComponentQueued(entity, componentTypeID);
	}
}

bool Handle::removeComponentImmediate(ComponentTypeID componentTypeID) {
	if (refresh()) {
		EntityLocationDetailed newLocation = entityManager->removeComponent(locationData, componentTypeID);
		updateLocation(newLocation);
		return !hasComponent(componentTypeID);
	}
}

void Handle::destroy() {
	if (refresh()) {
		entityManager->destroyEntityQueued(entity);
	}
}

void Handle::destroyImmediate() {
	if (refresh()) {
		entityManager->destroyEntity(locationData);
	}
}

std::size_t Handle::getComponentCount() {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock()) {
			return chunk->getComponentCount();
		}
	}
	return 0;
}

std::size_t Handle::getComponentCount(ComponentTypeID typeID) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock()) {
			return chunk->getComponentCount(typeID);
		}
	}
	return 0;
}