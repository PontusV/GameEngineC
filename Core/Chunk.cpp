#include "Chunk.h"
#include "Component.h"

#include <stdexcept>
#include <iostream>
#include <cstring>

using namespace Core;

int ChunkIDCounter::idCounter = 0;


Chunk::Chunk(std::vector<IComponentTypeInfo> infoVec) : size(0), id(idCounter++) {
	// Calculate max number of entries
	stride = sizeof(Entity);
	for (IComponentTypeInfo& info : infoVec) {
		stride += info.size;
	}

	MAX_SIZE = CHUNK_SIZE / stride;
	BUFFER_SIZE = MAX_SIZE * stride;
	buffer = new char[BUFFER_SIZE];

	// ComponentDataArrayInfo
	types.reserve(infoVec.size());
	std::size_t offset = sizeof(Entity);
	for (IComponentTypeInfo& info : infoVec) {
		types.push_back(ComponentDataArrayInfo(&buffer[offset], info.type.getTypeID(), info.size));
		offset += info.size;
	}
}

Chunk::~Chunk() {
	// Call destructors
	while (!isEmpty()) {
		destroy(size - 1);
	}
	delete[] buffer;
}

void Chunk::destroy(Entity entity) {
	int index = getIndex(entity);
	destroy(index);
}

void Chunk::destroy(std::size_t index) {
	// Call component destructors (destroy object without releasing memory from buffer)
	for (Component* component : getComponents(index)) {
		component->~Component();
	}
	remove(index);
}

void Chunk::remove(Entity entity) {
	int index = getIndex(entity);
	remove(index);
}

void Chunk::remove(std::size_t index) {
	std::vector<Component*> iComponents = getComponents(index);

	if (index < size - 1) { // Only swap if index isnt back of active chunk
		std::vector<Component*> lComponents = getComponents(size - 1);

		for (std::size_t i = 0; i < types.size(); i++) {
			std::memcpy(iComponents[i], lComponents[i], types[i].size);
		}

		// Copy data from last entry to the removed entry
		getEntity(index) = getEntity(size - 1);
	}
	size--;
	getEntity(size).setID(0); // Makes the back entry invalid
}

Component* Chunk::getComponent(std::size_t index, ComponentTypeID componentTypeID) {
	for (ComponentDataArrayInfo& info : types) {
		if (componentTypeID == info.typeID)
			return getComponent(index, info);
	}
	return nullptr;
}

Component* Chunk::getComponent(Entity entity, ComponentType componentType) {
	int index = getIndex(entity);
	return getComponent(index, componentType);
}

Component* Chunk::getComponent(std::size_t index, ComponentType componentType) {
	for (ComponentDataArrayInfo& info : types) {
		if (componentType == info.typeID)
			return getComponent(index, info);
	}
	return nullptr;
}

Component* Chunk::getComponent(std::size_t index, ComponentDataArrayInfo& info) {
	if (index > size) throw std::out_of_range("Chunk::getComponent::ERROR out of range!");
	return (Component*)(&info.beginPtr[stride * index]);
}

std::vector<Component*> Chunk::getComponents(Entity entity) {
	std::size_t index = getIndex(entity);
	return getComponents(index);
}

std::vector<Component*> Chunk::getComponents(std::size_t index) {
	std::vector<Component*> components;
	components.reserve(types.size());

	for (ComponentDataArrayInfo& info : types) {
		components.push_back(getComponent(index, info));
	}

	return components;
}

std::vector<Component*> Chunk::getComponents(Entity entity, ComponentType componentType) {
	std::size_t index = getIndex(entity);
	return getComponents(index, componentType);
}

std::vector<Component*> Chunk::getComponents(std::size_t index, ComponentType componentType) {
	std::vector<Component*> components;

	for (ComponentDataArrayInfo& info : types) {
		if (componentType == info.typeID)
			components.push_back(getComponent(index, info));
	}

	return components;
}

std::vector<ComponentDataBlock> Chunk::getComponentDataBlocks(Entity entity) {
	int index = getIndex(entity);
	std::vector<ComponentDataBlock> blocks;
	for (ComponentDataArrayInfo& info : types) {
		void* ptr = &info.beginPtr[stride * index];
		blocks.push_back({ ptr, info.typeID });
	}

	return blocks;
}

Entity* Chunk::getEntityPtr(Entity entity) {
	std::size_t index = getIndex(entity);
	return getEntityPtr(index);
}

Entity* Chunk::getEntityPtr(std::size_t index) {
	return (Entity*)&getEntityBeginPtr()[index * stride];
}

Entity& Chunk::getEntity(std::size_t index) {
	return *getEntityPtr(index);
}

/* Returns start of Entity array */
char* Chunk::getEntityBeginPtr() {
	return buffer;
}

char* Chunk::getComponentBeginPtr(ComponentTypeID typeID) {
	for (ComponentDataArrayInfo& info : types) {
		if (typeID == info.typeID)
			return info.beginPtr;
	}
	return nullptr;
}

std::vector<ComponentDataArrayInfo> Chunk::getComponentArrayInfo(ComponentType type) {
	std::vector<ComponentDataArrayInfo> infoVec;
	for (ComponentDataArrayInfo& info : types) {
		if (type == info.typeID) {
			infoVec.push_back(info);
		}
	}

	return infoVec;
}

void Chunk::copyEntity(Entity entity, std::vector<ComponentDataBlock> sources) {
	std::size_t index = getIndex(entity);

	for (ComponentDataBlock& src : sources) {
		bool copied = false;
		for (std::size_t i = 0; i < types.size(); i++) {
			if (types[i].typeID == src.typeID) {
				void* dest = &getComponentBeginPtr(types[i].typeID)[stride * index];
				std::memcpy(dest, src.ptr, types[i].size);
				copied = true;
				break;
			}
		}
		if (!copied) {
			Component* component = (Component*)src.ptr;
			component->destroy();
		}
	}
}

/* Looks to see if the given entity is contained in this chunk. */
bool Chunk::contains(Entity entity) {
	for (std::size_t i = 0; i < size; i++) {
		if (getEntity(i) == entity) return true;
	}

	return false;
}

bool Chunk::isFull() {
	return size >= MAX_SIZE;
}

bool Chunk::isEmpty() {
	return size == 0;
}

std::size_t Chunk::getID() {
	return id;
}

std::size_t Chunk::getSize() {
	return size;
}

std::size_t Chunk::getIndex(Entity entity) {
	for (std::size_t i = 0; i < size; i++) {
		if (getEntity(i) == entity)
			return i;
	}
	throw std::invalid_argument("Entity is not contained in this chunk!");
}

std::size_t Chunk::getStride() {
	return stride;
}