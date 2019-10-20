#include "Scene.h"
#include "entity/component/Component.h"
#include "entity/component/ComponentLoader.h"
#include "components/entity/ParentEntity.h"
#include "components/entity/ChildManager.h"
#include "components/Behaviour.h"

#include <iostream>
#include <stdexcept>
using namespace Core;


Scene::Scene(EntityManager* entityManager, ObjectType type) : manager(entityManager), type(type), isAwake(false) {
}

Scene::~Scene() {
	clear();
}


const std::vector<Handle>& Scene::getAllEntities() const {
	return entities;
}

std::vector<Handle> Scene::getRootEntities() {
	std::vector<Handle> roots;
	for (Handle& entity : entities) {
		if (!entity.hasParent())
			roots.push_back(entity);
	}
	return roots;
}

bool Scene::destroyEntity(Entity entity) {
	return destroyEntity(entity, false);
}

bool Scene::destroyEntity(Entity entity, bool chained) {
	auto it = findEntity(entity);
	if (it == entities.end()) return false;

	onEntityDestroyed(entity, chained);

	// Call onDestroy
	for (Behaviour* script : manager->getComponents<Behaviour>(entity)) {
		if (script->isEnabled())
			script->onDisable();
		script->onDestroy();
	}

	// Destroy children first. The children will destroy their own children.
	// Destroy Children
	Handle entityHandle = getEntityHandle(entity);
	if (entityHandle.hasComponent<ChildManager>()) {
		bool updateIt = entityHandle.getImmediateChildCount() > 0;
		while (entityHandle.getImmediateChildCount() > 0) {
			destroyEntity(getChild(entity, 0).getEntity(), true);
		}
		// Update the iterator if children were removed.
		if (updateIt) {
			it = findEntity(entity);
		}
	}

	manager->destroyEntity(entity);
	entities.erase(it);

	return false;
}
std::vector<Handle>::iterator Scene::findEntity(Entity entity) {
	for (auto it = entities.begin(); it != entities.end(); it++) {
		if (it->getEntity() == entity) {
			return it;
		}
	}
	return entities.end();
}

Handle Scene::getEntityHandle(Entity entity) {
	return Handle(entity, this);
}

Handle Scene::getEntityHandle(std::string name) {
	return Handle(manager->getEntity(name), this);
}

void Scene::removeComponent(Entity entity, ComponentTypeID componentTypeID) {
	Handle handle = getEntityHandle(entity);
	if (componentTypeID == typeof(Behaviour)) {
		Behaviour* script = static_cast<Behaviour*>(handle.getComponent(componentTypeID));
		script->onDestroy();
	}
	EntityLocation location = manager->removeComponent(entity, componentTypeID);
	handle.updateLocation(location);
	prepEntity(handle);
	onEntityChanged(handle);
}

void Scene::removeComponentQueued(Entity entity, ComponentTypeID componentTypeID) {
	functionQueue.push_back(new FunctionCaller<void, Scene, Entity, ComponentTypeID>(&Scene::removeComponent, *this, entity, componentTypeID));
}

void Scene::awake() {
	isAwake = true;
	// Loop through all entities
	for (Handle& handle : entities) {
		awakeEntity(handle);
	}
}

void Scene::awakeEntity(Handle entity) {
	std::vector<Behaviour*> scripts = entity.getComponents<Behaviour>();
	for (Behaviour* script : scripts) {
		script->activate();
	}
}

void Scene::awakeComponent(Handle entity, ComponentTypeID componentID) {
	// Awake script
	for (Behaviour* script : entity.getComponents<Behaviour>()) {
		if (script->getComponentID() == componentID) {
			script->activate();
			break;
		}
	}
}

/* Returns invalid handle if entity does not have ParentEntity component or if the parent is nullptr. */
Handle Scene::getParent(Entity entity) const {
	ParentEntity* parentCmp = manager->getComponent<ParentEntity>(entity);
	if (parentCmp) { // If component exists
		if (parentCmp->getParent().refresh()) {
			return parentCmp->getParent();
		}
	}
	return Handle();
}

/* Returns nullptr if entity does not have ChildRefCollection component or if the index is out of bounds. */
Handle Scene::getChild(Entity entity, int index) const {
	ChildManager* childRefs = manager->getComponent<ChildManager>(entity);
	if (childRefs) { // If component exists
		return childRefs->getChild(index);
	}
	return Handle();
}

std::size_t	Scene::getImmediateChildCount(Entity entity) const {
	ChildManager* childRefs = manager->getComponent<ChildManager>(entity);
	if (childRefs) { // If component exists
		return childRefs->getChildCount();
	}
	return 0;
}

/* Prepares recently added entity to be used by the systems. */
void Scene::prepEntity(Handle entity) {
	// Set owner
	std::vector<Component*> components = entity.getComponents<Component>();
	for (Component* component : components) {
		component->setOwner(entity);
	}

	// Update parent ref in Children
	std::size_t childCount = entity.getImmediateChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = entity.getChild(i);
		if (child.refresh()) {
			if (ParentEntity* parentCmp = child.getComponent<ParentEntity>()) {
				parentCmp->setParent(entity); // Update ref
			}
		}
	}
}

void Scene::destroyEntityQueued(Entity entity) {
	manager->removeEntityName(entity); // Unreserves name immediately
	EntityHandle handle = getEntityHandle(entity);
	// Unreserves names of children
	std::size_t childCount = handle.getChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		manager->removeEntityName(handle.getChild(i).getEntity());
	}
	functionQueue.push_back(new FunctionCaller<bool, Scene, Entity>(&Scene::destroyEntity, *this, entity));
}

void Scene::setParentQueued(Handle entity, Handle parent) {
	functionQueue.push_back(new FunctionCaller<void, Scene, Handle, Handle>(&Scene::setParent, *this, entity, parent));
}

void Scene::setParent(Handle entityHandle, Handle parentHandle) {
	if (parentHandle.getEntity().getID() == Entity::INVALID_ID) return; // Cannot set Invalid Entity to parent
	Entity entity = entityHandle.getEntity();
	Entity parent = parentHandle.getEntity();
	// Notify previous parent of removal
	Handle currentParent = getParent(entity);
	if (currentParent.refresh()) {
		for (Behaviour* behaviour : currentParent.getComponentsUpwards<Behaviour>()) {
			behaviour->onChildRemoved(entityHandle);
		}

		ChildManager* childManager = currentParent.getComponent<ChildManager>();
		childManager->onChildRemoved(entityHandle);
		if (childManager->getChildCount() == 0)
			removeComponent<ChildManager>(currentParent.getEntity());
	}

	// Add ChildManager to new parent if none already exists
	if (!manager->hasComponent<ChildManager>(parent)) {
		ChildManager childManager;
		addComponent<ChildManager>(parent, childManager);
	}

	// Modify ParentEntity component
	ParentEntity* parentComponent = manager->getComponent<ParentEntity>(entity);
	if (!parentComponent) {
		ParentEntity component;
		addComponent<ParentEntity>(entity, component);
		parentComponent = manager->getComponent<ParentEntity>(entity);
	}
	parentComponent->setParent(parentHandle);

	// Notify new parent of added child
	parentHandle.getComponent<ChildManager>()->onChildAdded(entityHandle);
	for (Behaviour* behaviour : parentHandle.getComponentsUpwards<Behaviour>()) {
		behaviour->onChildAdded(entityHandle);
	}
}

void Scene::onEntityCreated(Entity entity) {
	onEntityCreated(getEntityHandle(entity));
}

void Scene::onEntityCreated(Handle entity) {
	if (isAwake) awakeEntity(entity);
	// Should not have to handle any ParentEntity component here
}
void Scene::onEntityDestroyed(Entity entity, bool destroyingParent) {
	Handle handle = getEntityHandle(entity);
	// Check for parent & call onChildDestroyed
	Handle parent = handle.getParent();
	if (parent.refresh()) {
		if (!destroyingParent) {
			for (Behaviour* behaviour : parent.getComponentsUpwards<Behaviour>()) {
				behaviour->onChildRemoved(handle);
			}
		}
		ChildManager* childManager = parent.getComponent<ChildManager>();
		childManager->onChildRemoved(handle);
		if (!destroyingParent && childManager->getChildCount() == 0)
			removeComponent<ChildManager>(parent.getEntity());
	}
}

void Scene::onEntityChanged(Handle entity) {
	// Check for parent & call onChildChanged
	Handle parent = entity.getParent();
	for (Behaviour* behaviour : parent.getComponentsUpwards<Behaviour>()) {
		behaviour->onChildChanged(entity);
	}
}

void Scene::processQueue() {
	entityQueueMap.clear();
	while (!functionQueue.empty()) {
		IFunctionCaller* fun = functionQueue.front();
		fun->call();
		delete fun;
		functionQueue.pop_front();
	}
}

void Scene::clear() {
	// Clears and deletes all stored function calls
	while (!functionQueue.empty()) {
		delete functionQueue.front();
		functionQueue.pop_front();
	}
	// Destroys all Entities
	for (Handle& entity : entities) {
		destroyEntity(entity.getEntity());
	}
	entities.clear();
}

EntityManager* Scene::getEntityManager() {
	return manager;
}

ObjectType Scene::getType() {
	return type;
}

/* Save to outstream */
void Scene::serialize(std::ostream& os) const {
	// Entities
	std::size_t entityAmount = entities.size();
	os.write((char*)&entityAmount, sizeof(std::size_t));			// Entity amount

	for (auto it = entities.begin(); it != entities.end(); it++) {	// Loop through Entities
		Entity entity = it->getEntity();
		std::string entityName = manager->getEntityName(entity);
		os.write(entityName.c_str(), entityName.size() + 1);		// Entity name

		std::vector<Component*> components = manager->getComponents(entity);
		std::vector<IComponentTypeInfo> componentTypeInfoVec = manager->getComponentTypes(entity);
		std::size_t componentAmount = components.size();
		os.write((char*)&componentAmount, sizeof(std::size_t));		// Component amount

		for (std::size_t i = 0; i < components.size(); i++) {		// Loop through Components
			std::string typeName = componentTypeInfoVec[i].name;
			os.write(typeName.c_str(), typeName.size() + 1);		// Component Type name
			components[i]->serialize(os);	// Component Data
			//os.write((char*)&components[i], componentTypeInfoVec[i].size);	// Component Data
		}
	}

	// Hierarchy
	std::vector<std::pair<std::string, std::string>> parentChildPairs;
	for (auto it = entities.begin(); it != entities.end(); it++) {
		Entity entity = it->getEntity();
		// Loop through all children from roots (Child order is managed by ChildManager component)
		if (!manager->hasComponent<ParentEntity>(entity)) { // Checks if root
			std::size_t childCount = getImmediateChildCount(entity);
			for (std::size_t i = 0; i < childCount; i++) {
				Handle handle = getChild(entity, i);
				if (handle.refresh()) {
					EntityHandle child = handle;

					ParentEntity* component = child.getComponent<ParentEntity>();
					if (component) {
						std::string childName = child.getEntityName();

						std::size_t parentID = component->getParent().getEntity().getID();
						std::string parentName = manager->getEntityName(Entity(parentID));

						parentChildPairs.push_back(std::make_pair(childName, parentName));
					} else {
						std::cout << "Scene::serialize::ERROR Entity childManager has reference to Entity without any Parent!\n";
						throw std::invalid_argument("Scene::serialize::ERROR Entity childManager has reference to Entity without any Parent!");
					}
				}
			}
		}
		ParentEntity* component = manager->getComponent<ParentEntity>(entity);
		if (component) { // If the entity has a parent
			std::string childName = manager->getEntityName(entity);

			std::size_t parentID = component->getParent().getEntity().getID();
			std::string parentName = manager->getEntityName(Entity(parentID));

			parentChildPairs.push_back(std::make_pair(childName, parentName));
		}
	}

	std::size_t parentAmount = parentChildPairs.size();
	os.write((char*)&parentAmount, sizeof(std::size_t));			// Parent amount
	for (auto pair : parentChildPairs) {							// Loop through ParentChild pairs
		std::string childName = pair.first;
		os.write(childName.c_str(), childName.size() + 1);			// Child name

		std::string parentName = pair.second;
		os.write(parentName.c_str(), parentName.size() + 1);		// Parent name
	}
}

/* Load from instream */
void Scene::deserialize(std::istream& is) {
	// Entities
	std::size_t entityAmount;
	is.read((char*)&entityAmount, sizeof(std::size_t));					// Entity amount

	for (std::size_t i = 0; i < entityAmount; i++) {					// Loop through Entities
		std::string entityName;
		std::getline(is, entityName, '\0');								// Entity name
		EntityHandle object = createEntity(entityName);

		std::size_t componentAmount;
		is.read((char*)&componentAmount, sizeof(std::size_t));			// Component amount

		for (std::size_t ii = 0; ii < componentAmount; ii++) {			// Loop through Components
			std::string typeName;
			std::getline(is, typeName, '\0');							// Component Type name
			ComponentLoader::addComponentFromFile(is, typeName, object);// Component Data
		}
	}

	// Hierarchy
	std::size_t parentAmount;
	is.read((char*)&parentAmount, sizeof(std::size_t));				// Parent amount
	for (std::size_t i = 0; i < parentAmount; i++) {				// Loop through ParentChild pairs
		std::string childName;
		std::getline(is, childName, '\0');							// Child name

		std::string parentName;
		std::getline(is, parentName, '\0');							// Parent name

		// Add ParentEntity component
		EntityHandle parent = getEntityHandle(parentName);
		EntityHandle child = getEntityHandle(childName);
		if (parent.refresh())
			child.setParent(parent);
		else
			destroyEntity(child.getEntity());
	}
}