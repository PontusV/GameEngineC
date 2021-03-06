#include "Component.h"
#include <iostream>

using namespace Core;


Component::Component() {
}

Component::~Component() {
}

void Component::setOwner(Handle owner) {
	this->owner = owner;
}

EntityHandle& Component::getOwner() {
	return owner;
}

void Component::destroy() {
	destruct = true;
}

bool Component::isDestroyed() {
	return destruct;
}

// ------------------------------- Serializable ----------------------------------------

void Component::serialize(std::ostream& os) const {
	// Component Type is saved by loader
}

void Component::deserialize(std::istream& is) {
	// Component Type is read by loader
}