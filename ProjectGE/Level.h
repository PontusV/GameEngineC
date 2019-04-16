#ifndef LEVEL_H
#define LEVEL_H

#include "Serializable.h"
#include "EntityManager.h"
#include "EntityHandle.h"
#include "Entity.h"
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace Core {
	/* Contains a collection of entities. */
	class Level : public Serializable {
	public:
		Level();
		~Level();

		template <typename... Ts> EntityHandle	createEntity(std::string name, Ts... components); //Creates an entity and maps the given name to it
		void									destroyEntity(Entity entity);
		EntityHandle							getEntity(std::string entityName);
		EntityHandle							getEntityHandle(Entity entity);
		std::weak_ptr<EntityManager>			getEntityManager();

		/* Destroys all Entities. */
		void clear();

		// Serializable
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

	private:
		std::string						getEntityName(Entity entity) const;

	private:
		std::vector<Entity>				entities;		// A list of entities in order of the Hierarchy	
		std::map<std::string, Entity>	entityNameMap;	// Name of Entity, Entity
		std::shared_ptr<EntityManager>	manager;		// Manages the Entities in this level
	};

	// --------------------------- Template Function Definitions --------------------------------

	template <typename... Ts>
	EntityHandle Level::createEntity(std::string name, Ts... components) {
		if (entityNameMap.find(name) != entityNameMap.end()) {
			std::cout << "Level::createEntity::ERROR The name(" << name << ") is already mapped to another Entity!\n";
			throw std::invalid_argument("Level::createEntity::ERROR The name(" + name + ") is already mapped to another Entity!");
		}
		Handle handle = manager->createEntity(components...);
		if (handle.isValid() && handle.getEntity().getID() != Entity::INVALID_ID) {
			entities.push_back(handle.getEntity());
			entityNameMap.insert(std::pair<std::string, Entity>(name, handle.getEntity()));

			return handle;
		}
		return EntityHandle(); // If returned Entity was invalid, returns an invalid handle
	}
}
#endif