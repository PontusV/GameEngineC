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
		EntityHandle							getEntityHandle(std::string entityName);
		EntityHandle							getEntityHandle(Entity entity);
		std::weak_ptr<EntityManager>			getEntityManager();

		/* Calls awake on all Entities in this Level and awakes the EntityManager; new Scripts will be awaken right after creation. Does nothing if the level has already been awaken. */
		void awake();
		/* Destroys all Entities. */
		void clear();

		// Serializable
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

	private:
		std::string						getEntityName(Entity entity) const;

	private:
		std::vector<Entity>				entities;		// A list of entities in order of the Hierarchy	
		std::shared_ptr<EntityManager>	manager;		// Manages the Entities in this level
	};

	// --------------------------- Template Function Definitions --------------------------------

	template <typename... Ts>
	EntityHandle Level::createEntity(std::string name, Ts... components) {
		Handle handle = manager->createEntity(name, components...);
		if (handle.isValid() && handle.getEntity().getID() != Entity::INVALID_ID) {
			entities.push_back(handle.getEntity());
			return handle;
		}
		return EntityHandle(); // If returned Entity was invalid, returns an invalid handle
	}
}
#endif