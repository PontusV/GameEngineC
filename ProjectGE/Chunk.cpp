#include "Chunk.h"
#include "Component.h"

#include <stdexcept>
#include <iostream>
#include <cstring>

using namespace Core;

int ChunkIDCounter::idCounter = 0;


Chunk::Chunk(std::vector<IComponentTypeInfo> infoVec) : size(0), id(idCounter++) {
	// Number of entries
	std::size_t sizeSum = sizeof(Entity);
	for (IComponentTypeInfo& info : infoVec) {
		sizeSum += info.size;
	}

	MAX_SIZE = CHUNK_SIZE / sizeSum;
	BUFFER_SIZE = MAX_SIZE * sizeSum;
	buffer = new char[BUFFER_SIZE];

	// ComponentDataArrayInfo
	types.reserve(infoVec.size());
	for (IComponentTypeInfo& info : infoVec) {
		types.push_back(ComponentDataArrayInfo(info.type.getTypeID(), info.size));
	}

	// Set ptr to start of ComponenDataArrays
	std::size_t offset = sizeof(Entity) * MAX_SIZE; // Starts after Entity array
	for (ComponentDataArrayInfo& info : types) {
		info.ptr = &buffer[offset];
		offset += info.sizePerEntry * MAX_SIZE;
	}
}

Chunk::~Chunk() {
	// Call destructors
	while (!isEmpty()) {
		remove(size - 1, true);
	}
	delete[] buffer;
}

void Chunk::remove(Entity entity, bool destruct) {
	int index = getIndex(entity);
	remove(index, destruct);
}

void Chunk::remove(std::size_t index, bool destruct) {
	std::vector<Component*> iComponents = getComponents(index);

	if (destruct) {
		// Call component destructors (destroy object without releasing memory from buffer)
		for (Component* component : iComponents) {
			component->~Component();
		}
	}

	if (index < size - 1) { // Only swap if index isnt back of active chunk
		std::vector<Component*> lComponents = getComponents(size - 1);

		for (std::size_t i = 0; i < types.size(); i++) {
			std::memcpy(iComponents[i], lComponents[i], types[i].sizePerEntry);
		}

		// Copy data from last entry to the removed entry
		getEntityArrayPtr()[index] = getEntityArrayPtr()[size - 1];
	}
	size--;
	getEntityArrayPtr()[size].setID(0); // Makes the back entry invalid
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
	if (index > size) throw std::out_of_range("Chunk::getComponent::ERROR!");
	return (Component*)(&info.ptr[info.sizePerEntry * index]);
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
		void* ptr = &info.ptr[info.sizePerEntry * index]; // Wrong
		blocks.push_back({ ptr, info.typeID });
	}

	return blocks;
}

Entity* Chunk::getEntityPtr(Entity entity) {
	std::size_t index = getIndex(entity);
	return &getEntityArrayPtr()[index];
}

/* Returns start of Entity array */
Entity* Chunk::getEntityArrayPtr() {
	return static_cast<Entity*>((void*)&buffer[0]);
}

char* Chunk::getComponentArrayPtr(ComponentTypeID typeID) {
	for (ComponentDataArrayInfo& info : types) {
		if (typeID == info.typeID)
			return info.ptr;
	}
	return nullptr;
}

/* Returns all matches for the type. */
std::vector<char*> Chunk::getComponentArrayPtrs(ComponentType type) {
	std::vector<char*> componentArrayPtrVec;
	for (ComponentDataArrayInfo& info : types) {
		if (type == info.typeID) {
			componentArrayPtrVec.push_back(info.ptr);
		}
	}

	return componentArrayPtrVec;
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
				void* dest = &getComponentArrayPtr(types[i].typeID)[types[i].sizePerEntry * index];
				std::memcpy(dest, src.ptr, types[i].sizePerEntry);
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
	Entity* entityArrayPtr = getEntityArrayPtr();
	for (std::size_t i = 0; i < size; i++) {
		if (entityArrayPtr[i] == entity) return true;
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
	Entity* entityArray = getEntityArrayPtr();
	for (std::size_t i = 0; i < size; i++) {
		if (entityArray[i] == entity)
			return i;
	}
	throw std::invalid_argument("Entity is not contained in this chunk!");
}