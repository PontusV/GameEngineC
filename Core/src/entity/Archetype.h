#ifndef ARCHETYPE_H
#define ARCHETYPE_H

#include "entity/Entity.h"
#include "Chunk.h"
#include "component/ComponentTypeInfo.h"
#include "EntityLocation.h"

#include <vector>
#include <memory>

namespace Core {

	class Component; // Forward declare

	/* Contains entities with the archetype specified by the given types. If the types of components contained by an Entity matches the template it is added to the first chunk with room. */
	class Archetype {
	public:
		Archetype(std::vector<IComponentTypeInfo> types);
		~Archetype();

		template <typename... Ts>
		EntityLocation addEntity(Entity entity, Ts&... components);
		void destroyEntity(Entity entity);
		void removeEntity(Entity entity);
		void copyEntity(Entity entity, std::vector<ComponentDataBlock> sources);

		template<typename T>
		void setComponent(Entity entity, T& component);
		template<typename T>
		T* getComponent(Entity entity);
		/* Returns first match. */
		Component* getComponent(Entity entity, ComponentType componentType);
		std::vector<Component*> getComponents(Entity entity);

		bool isEmpty();
		bool match(std::vector<ComponentTypeID> typeIDs);
		/* Returns true if exact type is contained. Will return false if not exact type */
		bool containsType(ComponentTypeID typeID);

		std::vector<IComponentTypeInfo> getTypes();
		std::vector<ComponentTypeID> getTypeIDs();
		std::vector<ComponentDataBlock> getComponentDataBlocks(Entity entity);
		EntityLocation getLocation(Entity entity);

		void clear();
		bool operator==(Archetype& other);

		std::shared_ptr<Chunk> getContainer(Entity entity);

	private:
		void createChunk();
		void removeChunk(std::size_t index);

	private:
		std::vector<std::shared_ptr<Chunk>> chunks;
		std::vector<IComponentTypeInfo> types;
	};

	// --------------------------- Template Function Definitions --------------------------------

	template <typename... Ts>
	EntityLocation Archetype::addEntity(Entity entity, Ts&... components) {
		//Checking for space in existing chunks
		for (std::shared_ptr<Chunk> chunk : chunks) {
			if (!chunk->isFull()) {
				std::size_t index = chunk->add(entity, components...);
				return EntityLocation(index, chunk);
			}
		}

		//No space found, adding new chunk
		createChunk();
		std::shared_ptr<Chunk> chunk = chunks.back();
		std::size_t index = chunk->add(entity, components...);
		return EntityLocation(index, chunk);
	}

	template<typename T>
	void Archetype::setComponent(Entity entity, T& component) {
		getContainer(entity)->setComponent(entity, component);
	}

	template<typename T>
	T* Archetype::getComponent(Entity entity) {
		return getContainer(entity)->getComponent<T>(entity);
	}
}
#endif