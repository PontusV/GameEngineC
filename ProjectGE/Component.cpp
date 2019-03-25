#include "Component.h"
#include <iostream>

using namespace Core;


Component::Component() {
}

Component::~Component() {
}

void Component::setOwner(ChunkEntityHandle owner) {
	this->owner = owner;
}

void Component::destroy() {
	destruct = true;
}

bool Component::isDestroyed() {
	return destruct;
}

// ------------------------------- Serializable ----------------------------------------

void Component::serialize(std::ostream& os) const {
	ComponentTypeID typeID = getTypeID();
	os.write((char*)&typeID, sizeof(decltype(typeID)));	// Component Type ID
}

void Component::deserialize(std::istream& is) {
	// Component Type ID is read by loader
}