#ifndef CHUNK_ENTITY_HANDLE_H
#define CHUNK_ENTITY_HANDLE_H

#include "Chunk.h"
#include "Entity.h"
#include <memory>
#include <vector>

namespace Core {

	class Component; // Forward declare

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
		template <typename T> T* getComponent();
		Component* getComponent(ComponentTypeID typeID);
		std::vector<Component*> getComponents();
		Entity getEntity();
		Entity* getEntityPtr();

	private:
		std::weak_ptr<Chunk> chunkPtr;
		Entity entity;
	};

	template <typename T>
	T* ChunkEntityHandle::getComponent() {
		return static_cast<T*>(getComponent(T::TYPE_ID));
	}
}
#endif