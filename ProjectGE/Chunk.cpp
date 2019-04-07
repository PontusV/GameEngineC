#include "Chunk.h"
#include "Component.h"

#include <stdexcept>
#include <iostream>

using namespace Core;

int ChunkIDCounter::idCounter = 0;


Chunk::Chunk(std::vector<ComponentTypeInfo> types) : size(0), id(idCounter++), types(types) {
	std::size_t sizeSum = sizeof(Entity);
	for (ComponentTypeInfo type : types) {
		sizeSum += type.size;
	}

	MAX_SIZE = CHUNK_SIZE / sizeSum;
	BUFFER_SIZE = MAX_SIZE * sizeSum;
	buffer = new char[BUFFER_SIZE];
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
			std::memcpy(iComponents[i], lComponents[i], types[i].size);
		}

		// Copy data from last entry to the removed entry
		getEntityArrayPtr()[index] = getEntityArrayPtr()[size - 1];
	}
	getEntityArrayPtr()[size - 1].setID(0); // Makes the back entry invalid
	size--;
}

Component* Chunk::getComponent(Entity entity, ComponentTypeID componentTypeID) {
	int index = getIndex(entity);
	return getComponent(index, componentTypeID);
}

Component* Chunk::getComponent(std::size_t index, ComponentTypeID componentTypeID) {
	for (ComponentTypeInfo& type : types) {
		if (componentTypeID == type.id)
			return getComponent(index, type);
	}
	return nullptr;
}

Component* Chunk::getComponent(std::size_t index, ComponentTypeInfo& type) {
	char* componentArrayPtr = getComponentArrayPtr(type.id);
	if (!componentArrayPtr) return nullptr;
	return (Component*)(&componentArrayPtr[type.size * index]);
}

std::vector<Component*> Chunk::getComponents(Entity entity) {
	std::size_t index = getIndex(entity);
	return getComponents(index);
}

std::vector<Component*> Chunk::getComponents(std::size_t index) {
	std::vector<Component*> components;
	components.reserve(types.size());

	for (ComponentTypeInfo& type : types) {
		components.push_back(getComponent(index, type));
	}

	return components;
}

std::vector<ComponentDataBlock> Chunk::getComponentDataBlocks(Entity entity) {
	int index = getIndex(entity);
	std::vector<ComponentDataBlock> blocks;
	for (ComponentTypeInfo& type : types) {
		void* ptr = &getComponentArrayPtr(type.id)[type.size * index];
		blocks.push_back({ ptr, type.id });
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
	std::size_t offset = sizeof(Entity) * MAX_SIZE; // Starts after Entity array
	for (ComponentTypeInfo& type : types) {
		if (typeID == type.id) break;
		offset += type.size * MAX_SIZE;
	}
	if (offset == BUFFER_SIZE) return nullptr;

	return &buffer[offset];
}

void Chunk::copyEntity(Entity entity, std::vector<ComponentDataBlock> sources) {
	std::size_t index = getIndex(entity);

	for (ComponentDataBlock& src : sources) {
		bool copied = false;
		for (std::size_t i = 0; i < types.size(); i++) {
			if (types[i].id == src.typeID) {
				void* dest = &getComponentArrayPtr(types[i].id)[types[i].size * index];
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