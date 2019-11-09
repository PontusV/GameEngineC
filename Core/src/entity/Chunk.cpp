#include "Chunk.h"
#include "component/Component.h"
#include "ReflectionPolymorph.generated.h"

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
		remove(size - 1);
	}
	while (inactiveSize != 0) {
		remove(MAX_SIZE - inactiveSize);
	}
	delete[] buffer;
}

void Chunk::clear(std::size_t index) {
	getEntity(index).setID(0); // Makes the back entry invalid
	// Call component destructors (destroy object without releasing memory from buffer)
	for (Component* component : getComponents(index)) {
		component->~Component();
	}
}

void Chunk::remove(Entity entity) {
	int index = getIndex(entity);
	remove(index);
}

void Chunk::remove(std::size_t index) {
	clear(index);
	if (isActive(index)) { // Active
		if (index < size - 1) {
			move(size - 1, index);
		}
		size--;
	}
	else { // Inactive
		std::size_t backIndex = MAX_SIZE - inactiveSize;
		if (backIndex < index) {
			move(backIndex, index);
		}
		inactiveSize--;
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
	move(backIndex, size);
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
	move(size, MAX_SIZE - inactiveSize);
	return true;
}

bool Chunk::isActive(Entity entity) {
	return isActive(getIndex(entity));
}

bool Chunk::isActive(std::size_t index) {
	return index < size;
}

void Chunk::move(std::size_t fromIndex, std::size_t toIndex) {
	if (fromIndex == toIndex) return;
	std::vector<Component*> iComponents = getComponents(fromIndex);
	std::vector<Component*> lComponents = getComponents(toIndex);

	for (std::size_t i = 0; i < types.size(); i++) {
		Mirror::memcpy(lComponents[i], iComponents[i], types[i].typeID);
		iComponents[i]->~Component();
	}

	getEntity(toIndex) = getEntity(fromIndex);
	getEntity(fromIndex).setID(0);
}

void Chunk::swap(std::size_t index, std::size_t otherIndex) {
	if (index == otherIndex) return;
	std::vector<Component*> iComponents = getComponents(index);
	std::vector<Component*> lComponents = getComponents(otherIndex);

	for (std::size_t i = 0; i < types.size(); i++) {
		char* temp = new char[types[i].size];
		Mirror::memcpy(temp, iComponents[i], types[i].typeID);
		iComponents[i]->~Component();

		Mirror::memcpy(iComponents[i], lComponents[i], types[i].typeID);
		lComponents[i]->~Component();

		Mirror::memcpy(lComponents[i], temp, types[i].typeID);
		((Component*)temp)->~Component();

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

std::size_t Chunk::addInactive(Entity entity) {
	if (isFull()) throw std::invalid_argument("Chunk::addInactive::ERROR there is no more room in this chunk!");

	inactiveSize++;
	std::size_t index = MAX_SIZE - inactiveSize;
	getEntity(index) = entity;
	return index;
}

std::size_t Chunk::moveEntity(Entity entity, std::vector<ComponentDataBlock> sources, bool inactive) {
	std::size_t index = inactive ? addInactive(entity) : add(entity);

	for (ComponentDataBlock& src : sources) {
		if (char* dest = getComponentBeginPtr(src.typeID)) {
			Mirror::memcpy(&dest[stride * index], src.ptr, src.typeID);
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
	return (size + inactiveSize) >= MAX_SIZE;
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