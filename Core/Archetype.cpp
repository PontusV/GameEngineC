#include "Archetype.h"
#include "ComponentArrayManager.h"
#include <stdexcept>
#include <algorithm>

using namespace Core;

Archetype::Archetype(std::vector<IComponentTypeInfo> types) : types(types) {
	// Make sure types are unique
	if (types.size() > 0) {
		for (std::size_t i = 0; i < types.size() - 1; i++) {
			for (std::size_t ii = i + 1; ii < types.size(); ii++) {
				if (types[i].type == types[ii].type) {
					std::cout << "Archetype::Constructor::ERROR The list of types given to Archetype contains ComponentTypes matching with each other. Types must be unique! Types: " << types[i].type.getTypeID() << " & " << types[ii].type.getTypeID() << "\n";
					throw std::invalid_argument("Archetype::Constructor::ERROR The list of types given to Archetype contains ComponentTypes matching with each other. Types must be unique!");
				}
			}
		}
	}

	// Make sure types are sorted
	/*std::sort(this->types.begin(), this->types.end(), [](ComponentTypeSizePair& l, ComponentTypeSizePair& r) {
		return l.type.getTypeID() < r.type.getTypeID();
	});*/
}

Archetype::~Archetype() {
	clear();
}

/* Removes all chunks */
void Archetype::clear() {
	while (!chunks.empty())
		removeChunk(0);
}


void Archetype::removeEntity(Entity entity, bool destruct) {
	for (std::size_t i = 0; i < chunks.size(); i++) {
		if (chunks[i]->contains(entity)) {
			chunks[i]->remove(entity, destruct);

			if (chunks[i]->isEmpty()) {
				removeChunk(i);
			}
			return;
		}
	}
}

void Archetype::copyEntity(Entity entity, std::vector<ComponentDataBlock> sources) {
	getContainer(entity)->copyEntity(entity, sources);
}


Component* Archetype::getComponent(Entity entity, ComponentType componentType) {
	return getContainer(entity)->getComponent(entity, componentType);
}


std::vector<Component*> Archetype::getComponents(Entity entity) {
	return getContainer(entity)->getComponents(entity);
}

bool Archetype::isEmpty() {
	return chunks.empty();
}

std::vector<IComponentTypeInfo> Archetype::getTypes() {
	return types;
}

std::vector<ComponentTypeID> Archetype::getTypeIDs() {
	std::vector<ComponentTypeID> typeIDs;
	typeIDs.reserve(types.size());
	for (std::size_t i = 0; i < types.size(); i++) {
		typeIDs.push_back(types[i].type.getTypeID());
	}
	return typeIDs;
}

bool Archetype::match(std::vector<ComponentTypeID> typeIDs) {
	if (typeIDs.size() != types.size()) return false;
	for (ComponentTypeID& type : typeIDs) {
		if (!containsType(type)) return false;
	}
	return true;
}

bool Archetype::containsType(ComponentTypeID typeID) {
	for (IComponentTypeInfo& pair : types) {
		if (pair.type.getTypeID() == typeID) return true;
	}
	return false;
}

bool Archetype::operator==(Archetype& other) {
	if (other.types.size() != types.size()) return false;
	return match(other.getTypeIDs());
}

std::shared_ptr<Chunk> Archetype::getContainer(Entity entity) {
	for (std::shared_ptr<Chunk>& chunk : chunks) {
		if (chunk->contains(entity)) {
			return chunk;
		}
	}
	throw std::invalid_argument("Archetype::getContainer::ERROR The entity is not contained by this Archetype!");
}

EntityLocation Archetype::getLocation(Entity entity) {
	try {
		std::shared_ptr<Chunk> chunk = getContainer(entity);
		std::size_t index = chunk->getIndex(entity);
		return EntityLocation(index, chunk);
	} catch (std::exception&) {
		std::cout << "Archetype::getLocation::ERROR could not find location for Entity.\n";
		return EntityLocation(); // Returns invalid location
	}
}

std::vector<ComponentDataBlock> Archetype::getComponentDataBlocks(Entity entity) {
	return getContainer(entity)->getComponentDataBlocks(entity);
}

void Archetype::createChunk() {
	chunks.push_back(std::make_shared<Chunk>(types));
	ComponentArrayManager::getInstance().chunkAdded(chunks.back(), getTypeIDs());
}

void Archetype::removeChunk(std::size_t index) {
	ComponentArrayManager::getInstance().chunkRemoved(chunks[index]->getID());
	chunks.erase(chunks.begin() + index);
}