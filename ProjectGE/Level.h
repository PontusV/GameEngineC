#ifndef LEVEL_H
#define LEVEL_H

#include "Serializable.h"
#include "EntityManager.h"
#include "EntityHandle.h"
#include "Entity.h"
#include "HandleManager.h"
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace Core {
	/* Consists of multiple entities.
	*/
	class Level : public Serializable {
	public:
		Level();
		~Level();

		template <typename... Ts>
		EntityHandle					createEntity(std::string name, Ts*... components); //Creates an entity and maps the given name to it
		void							removeEntity(Entity entity);
		EntityHandle					getEntity(std::string entityName);
		std::weak_ptr<EntityManager>	getEntityManager() { return manager; }

		void clear();

		//Save & Load operator
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);
	private:
		std::map<std::string, Entity> entities; //Name of Entity, Entity
		std::shared_ptr<EntityManager> manager;	// Manages the Entities in this level
		HandleManager handleManager;			// Manages the ComponentHandles of the components in this level
	};

	/* Add game object to the back of the vector storing all objects within the level and gives it an ID number. */
	template <typename... Ts>
	EntityHandle Level::createEntity(std::string name, Ts*... components) {
		Entity entity = manager->createEntity(components...);
		if (entity.getID() != Entity::INVALID_ID) {
			entities.insert(std::pair<std::string, Entity>(name, entity));

			return EntityHandle(entity, manager);
		}
		return EntityHandle(); // If returned Entity was invalid, returns an invalid handle
	}
}
#endif