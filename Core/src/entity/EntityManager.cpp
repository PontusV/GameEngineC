#include "EntityManager.h"
#include "component/Component.h"

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

EntityLocation EntityManager::getLocation(Entity entity) {
	auto it = entityMap.find(entity);
	if (it == entityMap.end()) return EntityLocation(); // Returns invalid location
	return it->second->getLocation(entity);
}

Entity EntityManager::getEntity(std::string entityName) {
	Entity& entity = entityNameMap[entityName];
	return entity;
}

std::string EntityManager::getEntityName(Entity entity) {
	auto it = entityNameMap.begin();
	while (it != entityNameMap.end()) {
		if (it->second == entity) return it->first;
		it++;
	}
	auto it2 = entityMap.find(entity);
	if (it2 != entityMap.end()) {
		return std::string("Queued_For_Destruction");
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
		return;
		//std::cout << "EntityManager::removeEntity::ERROR The entity does not exist in this manager!\n";
		//throw std::invalid_argument("EntityManager::removeEntity::ERROR The entity does not exist in this manager!");
	}
	Archetype* archetype = it->second;
	archetype->removeEntity(entity);

	if (archetype->isEmpty()) {
		removeArchetype(archetype);
	}
	entityMap.erase(it);

	// Remove hideflags entry
	auto iterator = entityHideFlags.find(entity);
	if (iterator == entityHideFlags.end()) {
		std::cout << "EntityManager::removeEntity::ERROR There was an error when trying to remove the hideflags of Entity: " << entity.getID() << std::endl;
		throw std::invalid_argument("EntityManager::removeEntity::ERROR The Entity does not have hideflags!");
	}
	iterator = entityHideFlags.erase(iterator);
	removeEntityName(entity);
}

EntityLocation EntityManager::removeComponent(Entity entity, ComponentTypeID typeID) {
	Archetype* src = entityMap.at(entity);
	std::vector<IComponentTypeInfo> srcTypes = src->getTypes();

	auto it = std::find(srcTypes.begin(), srcTypes.end(), typeID);
	if (it == srcTypes.end()) throw std::invalid_argument("EntityManager::removeComponent::ERROR Cannot remove component");

	srcTypes.erase(it);
	Archetype* dest = getArchetype(srcTypes);
	return moveEntity(entity, src, dest);
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
	//typeIDs.reserve(types.size());

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

EntityLocation EntityManager::moveEntity(Entity entity, Archetype* src, Archetype* dest) {
	std::shared_ptr<Chunk> chunk = src->getContainer(entity);
	EntityLocation location = dest->moveEntity(entity, chunk->getComponentDataBlocks(entity), !chunk->isActive(entity));

	src->removeEntity(entity);
	if (src->isEmpty()) {
		removeArchetype(src);
	}
	/*auto it = entityMap.find(entity);
	if (it != entityMap.end())
		it->second = dest;*/
	entityMap.find(entity)->second = dest;

	return location;
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
	entityNameMap.clear();
	entityHideFlags.clear();
}