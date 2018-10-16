#include "Level.h"
#include "Entity.h"
#include <iostream>
#include "HandleManager.h"
#include "EntityHandle.h"
#include "Handle.h"
#include <stdexcept>


Level::Level(HandleManager& handleManager) : handleManager(handleManager) {
}

Level::~Level() {
	//Deletes all game objects contained within the map and removes the data pair from the list.
	for (std::map<std::string, Entity*>::iterator it = entities.begin(); it != entities.end(); it++) {
		delete it->second;
	}
	entities.clear();
}

/* Add game object to the back of the vector storing all objects within the level and gives it an ID number. */
EntityHandle Level::createEntity(std::string name) {
	Entity* entity = new Entity();
	Handle handle = handleManager.add(entity, HandleManager::HANDLE_TYPE_ENTITY);
	entity->setID(handle);
	entities.insert(std::pair<std::string, Entity*>(name, entity));

	return EntityHandle(*entity, handleManager);
}

/* Removes game object with given id. */
void Level::removeEntity(Handle id) {
	for (std::map<std::string, Entity*>::iterator it = entities.begin(); it != entities.end(); it++) {
		if (it->second->getID() == id) {
			//Delete the Entity from memory
			delete it->second;
			//Remove Entity from map
			entities.erase(it);
			//Inactivate the corresponding HandleEntry in HandleManager
			handleManager.remove(id);
			return;
		}
	}
}

EntityHandle Level::getEntity(std::string entityName) {
	std::map<std::string, Entity*>::iterator it = entities.find(entityName);
	if (it != entities.end())
		return EntityHandle(*it->second, handleManager);
	throw std::invalid_argument("Entity with given name does not exist!");
}

/* Save */
void Level::serialize(std::ostream& os) const {
	/*//Store Entities
	int size = entities.size();
	os.write((char*)&size, sizeof(int)); //Store amount of Entities to be loaded at load time
	for (std::map<std::string, Entity*>::const_iterator it = entities.begin(); it != entities.end(); ++it) {
		os.write(it->first.c_str(), it->first.size() + 1); //Write name

		Entity* entity = it->second;
		entity->serialize(os); //Write entity
	}*/
}

/* Load */
void Level::deserialize(std::istream& is) {
	/*//Load Entities
	int entityAmount;
	is.read((char*)&entityAmount, sizeof(int));
	for (int i = 0; i < entityAmount; i++) { //Create entities
		std::string entityName;
		std::getline(is, entityName, '\0'); //Read name

		Entity* entity = new Entity();
		entity->deserialize(is); //Read entity
		entities.insert(std::pair<std::string, Entity*>(entityName, entity));
		//Initiate pointer of Handle Entry
		Handle handleID = entity->getID();
		handleManager->update(handleID, 0);
	}*/
}