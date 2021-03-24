#include "Handle.h"
#include "scene/Scene.h"
#include "entity/Chunk.h"
#include "entity/component/Component.h"

#include "components/entity/ParentEntity.h"
#include "components/entity/ChildManager.h"
#include "ReflectionPolymorph.generated.h"

#include <algorithm>

using namespace Core;

Handle::Handle(Entity entity, Scene* scene) : entity(entity), scene(scene) {
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

Scene* Handle::getScene() const {
	return scene;
}

IScene* Handle::getIScene() const {
	return scene;
}

void Handle::activate() {
	scene->activateQueued(*this);
}

void Handle::deactivate() {
	scene->deactivateQueued(*this);
}

bool Handle::isActive() {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->isActive(locationData.index);
	}
	return false;
}

void Handle::setSiblingIndex(std::size_t index) {
	scene->setSiblingIndex(*this, index);
}

void Handle::setSiblingIndexQueued(std::size_t index) {
	scene->setSiblingIndexQueued(*this, index);
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
	scene = nullptr;
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
	if (scene) {
		locationData = scene->getEntityManager()->getLocation(entity);
	}
}

void Handle::updateLocation(EntityLocation location) {
	locationData = location;
}

const EntityLocation& Handle::getLocation() const {
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

Component* Handle::getComponent(ComponentTypeID typeID) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponent(locationData.index, typeID);
	}
	return nullptr;
}

Component* Handle::getComponent(ComponentType type) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponent(locationData.index, type);
	}
	return nullptr;
}

std::vector<Component*> Handle::getComponents(ComponentType type) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponents(locationData.index, type);
	}
	return std::vector<Component*>(); // Empty
}

std::vector<Component*> Handle::getComponents(ComponentTypeID typeID) {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponents(locationData.index, typeID);
	}
	return std::vector<Component*>(); // Empty
}

std::vector<Component*> Handle::getComponents() {
	if (refresh()) {
		if (auto chunk = locationData.chunk.lock())
			return chunk->getComponents(locationData.index);
	}
	return std::vector<Component*>(); // Empty
}

std::vector<Component*> Handle::getComponentsInImmediateChildren(bool includeInactive) {
	std::vector<Component*> components;
	std::size_t childCount = getImmediateChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = getChild(i);
		if (child.isActive() || includeInactive) {
			std::vector<Component*> childComponents = child.getComponents();
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}
	}
	return components;
}

std::vector<Component*> Handle::getComponentsInChildren(bool includeInactive) {
	std::vector<Component*> components;
	std::size_t childCount = getChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = getChild(i);
		if (child.isActive() || includeInactive) {
			std::vector<Component*> childComponents = child.getComponents();
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}
	}
	return components;
}

std::vector<Component*> Handle::getComponentsInParents() {
	std::vector<Component*> components;
	Handle parent = getParent();
	if (parent.refresh()) {
		std::vector<Component*> parentComponents = parent.getComponents();
		components.insert(components.end(), parentComponents.begin(), parentComponents.end());
		parentComponents = parent.getComponentsInParents();
		components.insert(components.end(), parentComponents.begin(), parentComponents.end());
	}
	return components;
}

std::vector<Component*> Handle::getComponentsUpwards() {
	std::vector<Component*> components = getComponents();
	std::vector<Component*> parentComponents = getComponentsInParents();
	components.insert(components.end(), parentComponents.begin(), parentComponents.end());
	return components;
}

std::vector<Component*> Handle::getComponentsDownwards(bool includeInactive) {
	std::vector<Component*> components = getComponents();
	std::vector<Component*> childComponents = getComponentsInChildren(includeInactive);
	components.insert(components.end(), childComponents.begin(), childComponents.end());
	return components;
}

const char* Handle::getEntityName() {
	if (refresh()) {
		return scene->getEntityManager()->getEntityName(entity);
	}
	std::cout << "EntityHandle::getEntityName::ERROR Invalid Handle!" << std::endl;
	return "Invalid";
}

bool Handle::renameEntity(const char* name) {
	if (refresh()) {
		return scene->getEntityManager()->renameEntity(entity, name);
	}
	std::cout << "EntityHandle::renameEntity::ERROR Invalid Handle!" << std::endl;
	return false;
}

HideFlags Handle::getEntityHideFlags() {
	HideFlags result = scene->getEntityManager()->getEntityHideFlags(entity);
	Handle parent = getParent();
	while (parent.refresh()) {
		HideFlags hideFlags = parent.getEntityHideFlags();
		result = result | hideFlags;
		parent = parent.getParent();
	}
	return result;
}

void Handle::setEntityHideFlags(HideFlags hideFlags) {
	return scene->getEntityManager()->setEntityHideFlags(entity, hideFlags);
}

void Handle::setParent(const Entity& entity, bool keepPosition) {
	Handle parent = scene->getEntityHandle(entity);
	setParent(parent, keepPosition);
}

void Handle::setParent(Handle entity, bool keepPosition) {
	scene->setParentQueued(*this, entity, keepPosition);
}

void Handle::removeParent(bool keepPosition) {
	scene->setParentQueued(*this, Handle(Entity(0), scene), keepPosition);
}

Handle Handle::getParent() {
	ParentEntity* parentCmp = static_cast<ParentEntity*>(getComponent(typeIDof(ParentEntity))); // Get component with ComponentTypeID is much faster
	if (parentCmp) {
		return parentCmp->getParent();
	}
	return Handle();
}

Handle Handle::getChild(std::size_t index) {
	ChildManager* childManager = static_cast<ChildManager*>(getComponent(typeIDof(ChildManager))); // Get component with ComponentTypeID is much faster
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
	ChildManager* childManager = static_cast<ChildManager*>(getComponent(typeIDof(ChildManager))); // Get component with ComponentTypeID is much faster
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
	scene->addComponentQueued(entity, componentTypeID);
}

void Handle::removeComponent(ComponentTypeID componentTypeID) {
	scene->removeComponentQueued(entity, componentTypeID);
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

IComponent* Handle::getIComponent(ComponentTypeID typeID) {
	return static_cast<IComponent*>(getComponent(typeID));
}

void Handle::getIComponents(IComponent** out, std::size_t count) {
	std::vector<Component*> components = getComponents();
	std::size_t size = std::min(count, components.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = static_cast<IComponent*>(components[i]);
	}
}

void Handle::getIComponents(ComponentTypeID typeID, IComponent** out, std::size_t count) {
	std::vector<Component*> components = getComponents(typeID);
	std::size_t size = std::min(count, components.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = static_cast<IComponent*>(components[i]);
	}
}