#include "Level.h"
#include "Component.h"
#include "ComponentLoader.h"
#include "ParentEntity.h"

#include <iostream>
#include <stdexcept>
using namespace Core;

namespace Core {
	namespace util {
		//Load helpers
		/*void loadComponent(EntityHandle entity, std::istream& is, const ComponentTypeID typeID, TypeList<>) {
			throw std::invalid_argument("Could not find ComponentType in TypeList when trying to load component.");
		}

		template <typename T, typename... Types>
		void loadComponent(EntityHandle entity, std::istream& is, const ComponentTypeID typeID, TypeList<T, Types...>) {
			if (typeID == T::TYPE_ID)
				return entity.addComponent<T>(is);

			return loadComponent(entity, is, typeID, TypeList<Types...>());
		}*/

		void loadComponent(EntityHandle entity, std::istream& is, const ComponentTypeID typeID) {
			//TYPE_LIST typeList;
			//return loadComponent(entity, is, typeID, typeList);
		}
	}
}

Level::Level() : manager(std::make_shared<EntityManager>()) {
}

Level::~Level() {
	entities.clear();
}

/* Removes game object with given id. */
void Level::removeEntity(Entity entity) {
	manager->removeEntity(entity);
}

EntityHandle Level::getEntity(std::string entityName) {
	std::map<std::string, Entity>::iterator it = entities.find(entityName);
	if (it != entities.end())
		return EntityHandle(it->second, manager);

	// Exception
	std::cout << "Entity with the name " << entityName << " does not exist!\n";
	throw std::invalid_argument("Entity with given name does not exist!");
}

void Level::clear() {
	entities.clear();
	manager->clear();
}

std::string Level::getEntityNameByEntity(Entity entity) const {
	auto it = entities.begin();
	while (it != entities.end()) {
		if (it->second == entity) return it->first;
		it++;
	}
	std::cout << "Level::getEntityNameByEntity::ERROR There is no such entity stored in this Level!\n";
	throw std::invalid_argument("Level::getEntityNameByEntity::ERROR There is no such entity stored in this Level!");
}

/* Save to outstream */
void Level::serialize(std::ostream& os) const {
	// Entities
	std::size_t entityAmount = entities.size();
	os.write((char*)&entityAmount, sizeof(std::size_t));			// Entity amount

	for (auto it = entities.begin(); it != entities.end(); it++) {	// Loop through Entities
		os.write(it->first.c_str(), it->first.size() + 1);			// Entity name

		std::vector<Component*> components = manager->getComponents(it->second);
		std::size_t componentAmount = components.size();
		os.write((char*)&componentAmount, sizeof(std::size_t));		// Component amount

		for (Component* component : components) {					// Loop through Components
			component->serialize(os);								// Component Data
		}
	}

	// Hierarchy
	std::vector<std::pair<std::string, std::string>> parentChildPairs;
	for (auto it = entities.begin(); it != entities.end(); it++) {
		ParentEntity* component = manager->getComponent<ParentEntity>(it->second);
		if (component) { // If the entity has a parent
			std::string childName = it->first;

			std::size_t parentID = component->getParent().getID();
			std::string parentName = getEntityNameByEntity(parentID);

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
	is.read((char*)&entityAmount, sizeof(std::size_t));				// Entity amount

	for (std::size_t i = 0; i < entityAmount; i++) {				// Loop through Entities
		std::string entityName;
		std::getline(is, entityName, '\0');							// Entity name
		EntityHandle object = createEntity(entityName);

		std::size_t componentAmount;
		is.read((char*)&componentAmount, sizeof(std::size_t));		// Component amount

		for (std::size_t ii = 0; ii < componentAmount; ii++) {		// Loop through Components
			ComponentLoader::addComponentFromFile(is, object);		// Component Data
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
		std::size_t parentID = getEntity(parentName).getEntity().getID();
		if (parentID == Entity::INVALID_ID) {
			getEntity(childName).destroy();
		} else {
			getEntity(childName).addComponent<ParentEntity>(parentID);
		}
	}
}