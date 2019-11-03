#include "Chunk.h"
#include "component/Component.h"

#include <stdexcept>
#include <iostream>
#include <cstring>

using namespace Core;

int ChunkIDCounter::idCounter = 0;


Chunk::Chunk(std::vector<IComponentTypeInfo> infoVec) : size(0), inactiveSize(0), id(idCounter++) {
	// Calculate max number of entries
	stride = sizeof(Entity);
	for (IComponentTypeInfo& info : infoVec) {
		stride += info.size;
	}

	MAX_SIZE = CHUNK_SIZE / stride;
	BUFFER_SIZE = MAX_SIZE * stride;
	buffer = new char[BUFFER_SIZE];

	// ComponentDataArrayInfo
	std::size_t offset = sizeof(Entity);
	for (IComponentTypeInfo& info : infoVec) {
		types.push_back(ComponentDataArrayInfo(&buffer[offset], info.type.getTypeID(), info.size));
		offset += info.size;
	}
}

Chunk::~Chunk() {
	// Call destructors
	while (size != 0) {
		destroy(size - 1);
	}
	while (inactiveSize != 0) {
		destroy(MAX_SIZE - inactiveSize);
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
	if (isActive(index)) { // Active
		if (index < size - 1) { // Only swap if index isnt back of active chunk
			swap(index, size - 1);
		}
		size--;
		getEntity(size).setID(0); // Makes the back entry invalid
	}
	else { // Inactive
		std::size_t backIndex = MAX_SIZE - inactiveSize;
		if (backIndex < index) { // Only swap if index isnt back of inactive chunk
			swap(index, backIndex);
		}
		inactiveSize--;
		getEntity(backIndex).setID(0); // Makes the back entry invalid
	}
}

bool Chunk::activate(Entity entity) {
	std::size_t index = getIndex(entity);
	return activate(index);
}

bool Chunk::activate(std::size_t index) {
	if (isActive(index)) return true; // Already active
	std::size_t backIndex = MAX_SIZE - inactiveSize;
	if (backIndex < index) { // Swaps to back of the inactives
		swap(index, backIndex);
	}
	swap(size, backIndex);
	size++;
	inactiveSize--;
	return true;
}

bool Chunk::deactivate(Entity entity) {
	std::size_t index = getIndex(entity);
	return deactivate(index);
}

bool Chunk::deactivate(std::size_t index) {
	if (!isActive(index)) return true; // Already inactive
	if (index < size - 1) { // Swaps to back of the actives
		swap(index, size - 1);
	}
	size--;
	inactiveSize++;
	swap(size, MAX_SIZE - inactiveSize);
	return true;
}

bool Chunk::isActive(Entity entity) {
	return isActive(getIndex(entity));
}

bool Chunk::isActive(std::size_t index) {
	return index < size;
}

void Chunk::swap(std::size_t index, std::size_t otherIndex) {
	std::vector<Component*> iComponents = getComponents(index);
	std::vector<Component*> lComponents = getComponents(otherIndex);

	for (std::size_t i = 0; i < types.size(); i++) {
		char* temp = new char[types[i].size];
		std::memcpy(temp, iComponents[i], types[i].size);
		std::memcpy(iComponents[i], lComponents[i], types[i].size);
		std::memcpy(lComponents[i], temp, types[i].size);
		delete[] temp;
	}

	// Copy data from last entry to the removed entry
	Entity temp = getEntity(index);
	getEntity(index) = getEntity(otherIndex);
	getEntity(otherIndex) = temp;
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
	if (index >= MAX_SIZE) throw std::out_of_range("Chunk::getComponent::ERROR out of range!");
	return (Component*)(&info.beginPtr[stride * index]);
}

std::vector<Component*> Chunk::getComponents(Entity entity) {
	std::size_t index = getIndex(entity);
	return getComponents(index);
}

std::vector<Component*> Chunk::getComponents(std::size_t index) {
	std::vector<Component*> components;

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
		blocks.push_back({ ptr, info.typeID, info.size });
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

std::size_t Chunk::addInactive(Entity entity) {
	if (isFull()) throw std::invalid_argument("Chunk::addInactive::ERROR there is no more room in this chunk!");

	inactiveSize++;
	std::size_t index = MAX_SIZE - inactiveSize;
	getEntity(index) = entity;
	return index;
}

std::size_t Chunk::moveEntity(Entity entity, std::vector<ComponentDataBlock> sources, bool inactive) {
	std::size_t index = inactive ? addInactive(entity) : add(entity);

	std::size_t i = 0;
	for (ComponentDataBlock& src : sources) {
		if (char* dest = getComponentBeginPtr(src.typeID)) {
			std::memcpy(&dest[stride * index], src.ptr, src.size);
		} else {
			Component* component = (Component*)src.ptr;
			component->destroy();
		}
	}
	return index;
}

/* Looks to see if the given entity is contained in this chunk. */
bool Chunk::contains(Entity entity) {
	for (std::size_t i = 0; i < size; i++) {
		if (getEntity(i) == entity) return true;
	}
	for (std::size_t i = MAX_SIZE - inactiveSize; i < MAX_SIZE; i++) {
		if (getEntity(i) == entity) return true;
	}

	return false;
}

bool Chunk::isFull() {
	return size >= (MAX_SIZE - inactiveSize);
}

bool Chunk::isEmpty() {
	return size == 0 && inactiveSize == 0;
}

std::size_t Chunk::getID() {
	return id;
}

std::size_t Chunk::getSize() {
	return size;
}

std::size_t Chunk::getIndex(Entity entity) {
	// Check active
	for (std::size_t i = 0; i < size; i++) {
		if (getEntity(i) == entity)
			return i;
	}
	// Check inactive
	for (std::size_t i = MAX_SIZE - inactiveSize; i < MAX_SIZE; i++) {
		if (getEntity(i) == entity)
			return i;
	}
	throw std::invalid_argument("Entity is not contained in this chunk!");
}

std::size_t Chunk::getStride() {
	return stride;
}