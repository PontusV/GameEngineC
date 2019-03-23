#include "ChunkEntityHandle.h"
using namespace Core;


ChunkEntityHandle::ChunkEntityHandle(std::weak_ptr<Chunk> chunkPtr, Entity entity) : chunkPtr(chunkPtr), entity(entity)
{
}

ChunkEntityHandle::~ChunkEntityHandle()
{
}

Component* ChunkEntityHandle::getComponent(ComponentTypeID typeID) {
	if (auto ptr = chunkPtr.lock()) {
		return ptr->getComponent(entity, typeID);
	}
	return nullptr; // Chunkptr was invalid
}

std::vector<Component*> ChunkEntityHandle::getComponents() {
	if (auto ptr = chunkPtr.lock()) {
		return ptr->getComponents(entity);
	}
	throw std::invalid_argument("ChunkEntityHandle::getComponents()::error Chunkptr was invalid");
	//return nullptr; // Chunkptr was invalid
}

Entity ChunkEntityHandle::getEntity() {
	return entity;
}
Entity* ChunkEntityHandle::getEntityPtr() {
	if (auto ptr = chunkPtr.lock()) {
		return ptr->getEntityPtr(entity);
	}
	return nullptr; // Chunkptr was invalid
}
bool ChunkEntityHandle::isValid() {
	//true if chunkPtr is initialized
	using wt = std::weak_ptr<Chunk>;
	return chunkPtr.owner_before(wt{}) || wt{}.owner_before(chunkPtr);
}