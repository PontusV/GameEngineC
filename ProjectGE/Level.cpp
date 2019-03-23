#include "Level.h"
#include "Component.h"
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
	throw std::invalid_argument("Entity with given name does not exist!");
}

void Level::clear() {
	manager->clear();
}

/* Save */
void Level::serialize(std::ostream& os) const {
	//Store Entities
	std::size_t entityAmount = entities.size();
	os.write((char*)&entityAmount, sizeof(std::size_t)); //Store amount of Entities to be loaded at load time
	for (std::map<std::string, Entity>::const_iterator it = entities.begin(); it != entities.end(); it++) {
		os.write(it->first.c_str(), it->first.size() + 1); //Write name
		//Write Entity ID--------------------------------------------------------------

		std::vector<Component*> components = manager->getComponents(it->second);

		std::size_t componentAmount = components.size();
		os.write((char*)&componentAmount, sizeof(std::size_t)); //Store amount of Components to be loaded at load time
		for (Component* component : components) {
			component->serialize(os);
		}
	}
}

/* Load */
void Level::deserialize(std::istream& is) {
	//Load Entities
	std::size_t entityAmount;
	is.read((char*)&entityAmount, sizeof(std::size_t));
	for (std::size_t i = 0; i < entityAmount; i++) { //Load entities
		std::string entityName;
		std::getline(is, entityName, '\0'); //Read name
		//Load Entity ID--------------------------------------------------------------

		EntityHandle object = createEntity(entityName); //Create empty Entity

		std::size_t componentAmount;
		is.read((char*)&componentAmount, sizeof(std::size_t));
		for (std::size_t ii = 0; ii < componentAmount; ii++) {
			ComponentTypeID typeID;
			is.read((char*)&typeID, sizeof(ComponentTypeID));
			util::loadComponent(object, is, typeID);
		}
	}
}