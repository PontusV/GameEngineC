#ifndef ARCHETYPE_H
#define ARCHETYPE_H

#include "Entity.h"
#include "Chunk.h"
#include "ChunkEntityHandle.h"
#include "ComponentTypeInfo.h"

#include <vector>
#include <memory>

namespace Core {

	class Component; // Forward declare

	/* Contains entities with the archetype specified by the given types. If the types of components contained by an Entity matches the template it is added to the first chunk with room. */
	class Archetype {
	public:
		Archetype(std::vector<ComponentTypeInfo> types);
		~Archetype();

		template <typename... Ts>
		ChunkEntityHandle					addEntity(Entity entity, Ts*... components);
		void								removeEntity(Entity entity, bool destruct = true);					// Destruct decides if the destructor of its components should be called or not
		void								copyEntity(Entity entity, std::vector<ComponentDataBlock> sources);

		template<typename T> void			setComponent(Entity entity, T* component);
		Component*							getComponent(Entity entity, ComponentTypeID componentTypeID);
		std::vector<Component*>				getComponents(Entity entity);

		bool								isEmpty();
		bool								match(std::vector<ComponentTypeInfo> otherTypes);
		bool								match(std::vector<ComponentTypeID> typeIDs);
		bool								containsType(ComponentTypeID typeID);

		std::vector<ComponentTypeInfo>		getTypes();
		std::vector<ComponentTypeID>		getTypeIDs();
		std::vector<ComponentDataBlock>		getComponentDataBlocks(Entity entity);

		void								clear();
		bool								operator==(Archetype& other);

	private:
		std::shared_ptr<Chunk>				getContainer(Entity entity);
		void								createChunk();
		void								removeChunk(std::size_t index);

	private:
		std::vector<std::shared_ptr<Chunk>> chunks;
		std::vector<ComponentTypeInfo> types;
	};

	// --------------------------- Template Function Definitions --------------------------------

	template <typename... Ts>
	ChunkEntityHandle Archetype::addEntity(Entity entity, Ts*... components) {
		//Checking for space in existing chunks
		for (std::shared_ptr<Chunk> chunk : chunks) {
			if (!chunk->isFull()) {
				chunk->add(entity, components...);
				return ChunkEntityHandle(chunk, entity);
			}
		}

		//No space found, adding new chunk
		createChunk();
		std::shared_ptr<Chunk> chunk = chunks.back();
		chunk->add(entity, components...);
		return ChunkEntityHandle(chunk, entity);
	}

	template<typename T>
	void Archetype::setComponent(Entity entity, T* component) {
		getContainer(entity)->setComponent(entity, component);
	}
}
#endif