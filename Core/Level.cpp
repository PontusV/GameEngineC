#include "Level.h"
#include "Component.h"
#include "ComponentLoader.h"
#include "ParentEntity.h"

#include <iostream>
#include <stdexcept>
using namespace Core;


Level::Level() : manager(std::make_shared<EntityManager>()) {
}

Level::~Level() {
	clear();
}

void Level::awake() {
	manager->awake();
}

/* Removes game object with given id. */
void Level::destroyEntity(Entity entity) {
	manager->destroyEntity(entity);
}

EntityHandle Level::getEntityHandle(std::string entityName) {
	return manager->getEntityHandle(entityName);
}

EntityHandle Level::getEntityHandle(Entity entity) {
	return manager->getEntityHandle(entity);
}

void Level::clear() {
	manager->clear();
	entities.clear();
}

std::string Level::getEntityName(Entity entity) const {
	return manager->getEntityName(entity);
}

std::weak_ptr<EntityManager> Level::getEntityManager() {
	return manager;
}

/* Save to outstream */
void Level::serialize(std::ostream& os) const {
	// Entities
	std::size_t entityAmount = entities.size();
	os.write((char*)&entityAmount, sizeof(std::size_t));			// Entity amount

	for (auto it = entities.begin(); it != entities.end(); it++) {	// Loop through Entities
		std::string entityName = getEntityName(*it);
		os.write(entityName.c_str(), entityName.size() + 1);		// Entity name

		std::vector<Component*> components = manager->getComponents(*it);
		std::vector<IComponentTypeInfo> componentTypeInfoVec = manager->getComponentTypes(*it);
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
		// Loop through all children from roots (Child order is managed by ChildManager component)
		if (!manager->hasComponent<ParentEntity>(*it)) { // Checks if root
			std::size_t childCount = manager->getImmediateChildCount(*it);
			for (std::size_t i = 0; i < childCount; i++) {
				Handle handle = manager->getChild(*it, i);
				if (handle.refresh()) {
					Entity child = handle.getEntity();

					ParentEntity* component = manager->getComponent<ParentEntity>(child);
					if (component) {
						std::string childName = getEntityName(child);

						std::size_t parentID = component->getParent().getEntity().getID();
						std::string parentName = getEntityName(Entity(parentID));

						parentChildPairs.push_back(std::make_pair(childName, parentName));
					} else {
						std::cout << "Level::serialize::ERROR Entity childManager has reference to Entity without any Parent!\n";
						throw std::invalid_argument("Level::serialize::ERROR Entity childManager has reference to Entity without any Parent!");
					}
				}
			}
		}
		ParentEntity* component = manager->getComponent<ParentEntity>(*it);
		if (component) { // If the entity has a parent
			std::string childName = getEntityName(*it);

			std::size_t parentID = component->getParent().getEntity().getID();
			std::string parentName = getEntityName(Entity(parentID));

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
void Level::deserialize(std::istream& is) {
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
		Entity parent = getEntityHandle(parentName).getEntity();
		if (parent.getID() == Entity::INVALID_ID) {
			getEntityHandle(childName).destroy();
		} else {
			getEntityHandle(childName).setParent(parent);
		}
	}
}