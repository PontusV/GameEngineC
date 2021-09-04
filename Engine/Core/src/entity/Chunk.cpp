#include "Chunk.h"
#include "component/IComponentData.h"
#include "ReflectionPolymorph.generated.h"

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
	delete[] buffer;
}

std::vector<Entity> Chunk::getAllEntities() {
	std::vector<Entity> result;
	result.reserve(size);
	for (std::size_t i = 0; i < size; i++) {
		result.push_back(getEntity(i));
	}
	return result;
}

void Chunk::clear(std::size_t index) {
	getEntity(index).setID(0); // Makes the back entry invalid
	// Call component destructors (destroy object without releasing memory from buffer)
	for (IComponentData* component : getComponents(index)) {
		component->~IComponentData();
	}
}

void Chunk::remove(Entity entity) {
	int index = getIndex(entity);
	remove(index);
}

void Chunk::remove(std::size_t index) {
	clear(index);
	if (index < size - 1) {
		move(size - 1, index);
	}
	size--;
}

void Chunk::move(std::size_t fromIndex, std::size_t toIndex) {
	if (fromIndex == toIndex) return;
	std::vector<IComponentData*> iComponents = getComponents(fromIndex);
	std::vector<IComponentData*> lComponents = getComponents(toIndex);

	for (std::size_t i = 0; i < types.size(); i++) {
		Mirror::memcpy(lComponents[i], iComponents[i], types[i].typeID);
		iComponents[i]->~IComponentData();
	}

	getEntity(toIndex) = getEntity(fromIndex);
	getEntity(fromIndex).setID(0);
}

void Chunk::swap(std::size_t index, std::size_t otherIndex) {
	if (index == otherIndex) return;
	std::vector<IComponentData*> iComponents = getComponents(index);
	std::vector<IComponentData*> lComponents = getComponents(otherIndex);

	for (std::size_t i = 0; i < types.size(); i++) {
		char* temp = new char[types[i].size];
		Mirror::memcpy(temp, iComponents[i], types[i].typeID);
		iComponents[i]->~IComponentData();

		Mirror::memcpy(iComponents[i], lComponents[i], types[i].typeID);
		lComponents[i]->~IComponentData();

		Mirror::memcpy(lComponents[i], temp, types[i].typeID);
		((IComponentData*)temp)->~IComponentData();

		delete[] temp;
	}

	// Copy data from last entry to the removed entry
	Entity temp = getEntity(index);
	getEntity(index) = getEntity(otherIndex);
	getEntity(otherIndex) = temp;
}

IComponentData* Chunk::getComponent(std::size_t index, ComponentTypeID componentTypeID) {
	for (ComponentDataArrayInfo& info : types) {
		if (componentTypeID == info.typeID)
			return getComponent(index, info);
	}
	return nullptr;
}

IComponentData* Chunk::getComponent(Entity entity, ComponentType componentType) {
	int index = getIndex(entity);
	return getComponent(index, componentType);
}

IComponentData* Chunk::getComponent(Entity entity, ComponentTypeID componentTypeID) {
	int index = getIndex(entity);
	return getComponent(index, componentTypeID);
}

IComponentData* Chunk::getComponent(std::size_t index, ComponentType componentType) {
	for (ComponentDataArrayInfo& info : types) {
		if (componentType == info.typeID)
			return getComponent(index, info);
	}
	return nullptr;
}

IComponentData* Chunk::getComponent(std::size_t index, ComponentDataArrayInfo& info) {
	if (index >= MAX_SIZE) throw std::out_of_range("Chunk::getComponent::ERROR out of range!");
	return (IComponentData*)(&info.beginPtr[stride * index]);
}

std::vector<IComponentData*> Chunk::getComponents(Entity entity) {
	std::size_t index = getIndex(entity);
	return getComponents(index);
}

std::vector<IComponentData*> Chunk::getComponents(std::size_t index) {
	std::vector<IComponentData*> components;

	for (ComponentDataArrayInfo& info : types) {
		components.push_back(getComponent(index, info));
	}

	return components;
}

std::vector<IComponentData*> Chunk::getComponents(Entity entity, ComponentType componentType) {
	std::size_t index = getIndex(entity);
	return getComponents(index, componentType);
}

std::vector<IComponentData*> Chunk::getComponents(std::size_t index, ComponentType componentType) {
	std::vector<IComponentData*> components;

	for (ComponentDataArrayInfo& info : types) {
		if (componentType == info.typeID)
			components.push_back(getComponent(index, info));
	}

	return components;
}

std::vector<IComponentData*> Chunk::getComponents(Entity entity, ComponentTypeID componentTypeID) {
	std::size_t index = getIndex(entity);
	return getComponents(index, componentTypeID);
}

std::vector<IComponentData*> Chunk::getComponents(std::size_t index, ComponentTypeID componentTypeID) {
	std::vector<IComponentData*> components;

	for (ComponentDataArrayInfo& info : types) {
		if (componentTypeID == info.typeID)
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

std::size_t Chunk::moveEntity(Entity entity, std::vector<ComponentDataBlock> sources) {
	std::size_t index = add(entity);

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

std::size_t Chunk::getComponentCount() {
	return types.size();
}

std::size_t Chunk::getComponentCount(ComponentTypeID componentTypeID) {
	std::size_t count = 0;
	for (ComponentDataArrayInfo& info : types) {
		if (componentTypeID == info.typeID)
			count++;
	}
	return count;
}

void Chunk::setComponent(Entity entity, ComponentTypeID componentTypeID) {
	std::size_t index = getIndex(entity);
	setComponent(index, componentTypeID);
}

void Chunk::setComponent(std::size_t index, ComponentTypeID componentTypeID) {
	char* componentBeginPtr = getComponentBeginPtr(componentTypeID);
	if (componentBeginPtr) {
		void* dest = &componentBeginPtr[index * stride];
		// Creates the component at the specified location
		IComponentData* newComponent = Mirror::createInstance<IComponentData>(componentTypeID, dest);
	}
	else {
		throw std::invalid_argument("Chunk::setComponent invalid component type! It does not exist in this chunk.");
	}
}