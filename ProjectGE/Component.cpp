#include "Component.h"
#include "ParentEntity.h"
#include "ChildManager.h"
#include <iostream>

using namespace Core;


Component::Component() {
}

Component::~Component() {
}

Handle* Component::getParent() {
	ParentEntity* parentCmp = owner.getComponent<ParentEntity>();
	if (parentCmp) {
		return parentCmp->getParent();
	}
	return nullptr;
}

Handle* Component::getChild(std::size_t index) {
	ChildManager* childManager = owner.getComponent<ChildManager>();
	if (childManager) {
		return childManager->getChild(index);
	}
	return nullptr;
}

std::size_t Component::getChildCount() {
	ChildManager* childManager = owner.getComponent<ChildManager>();
	if (childManager) {
		return childManager->getChildCount();
	}
	return 0;
}

void Component::setOwner(Handle owner) {
	this->owner = owner;
}

Handle Component::getOwner() {
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
	ComponentTypeID typeID = getTypeID();
	os.write((char*)&typeID, sizeof(decltype(typeID)));	// Component Type ID
}

void Component::deserialize(std::istream& is) {
	// Component Type ID is read by loader
}