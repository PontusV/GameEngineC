#ifndef CHUNK_ENTITY_HANDLE_H
#define CHUNK_ENTITY_HANDLE_H

#include "Chunk.h"
#include "Entity.h"
#include <memory>
#include <vector>

namespace Core {

	class Component; // Forward declare

	/* A reference to an Entity. A faster kind of Core::EntityHandle. Becomes invalid when the Entity it refers to moves to another archetype/chunk. */
	class ChunkEntityHandle {
	public:
		ChunkEntityHandle(std::weak_ptr<Chunk> chunkPtr, Entity entity);
		ChunkEntityHandle() {}
		~ChunkEntityHandle();

		bool operator==(const Entity& value) {
			return entity == value;
		}

		bool isValid();

	public:
		template <typename T> T*	getComponent() const;
		Component*					getComponent(ComponentTypeID typeID) const;
		std::vector<Component*>		getComponents() const;
		/* Returns a copy of the Entity it refers to. */
		Entity						getEntity() const;
		/* Returns a pointer to the Entity in the chunk buffer. Returns nullptr if the Entity is no longer in the chunk. */
		Entity*						getEntityPtr() const;

	private:
		std::weak_ptr<Chunk> chunkPtr;
		Entity entity;
	};

	template <typename T>
	T* ChunkEntityHandle::getComponent() const {
		return static_cast<T*>(getComponent(T::TYPE_ID));
	}
}
#endif