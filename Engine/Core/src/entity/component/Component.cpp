#include "Component.h"
#include <iostream>

using namespace Core;

UIDContainer<1000000> Component::uniqueIDContainer = UIDContainer<1000000>();

Component::Component() {
	//componentID = uniqueIDContainer.getUID();
	componentID = uniqueIDContainer.getUID();
}

Component::~Component() {
	uniqueIDContainer.recycle(componentID);
}

void Component::setOwner(Handle owner) {
	this->owner = owner;
}

Handle Component::getOwner() {
	owner.refresh();
	return owner;
}

ComponentID Component::getComponentID() const {
	return componentID;
}

Component::Component(const Component& other) {
	owner = other.owner;
	componentID = other.componentID;
	uniqueIDContainer.useID(componentID);
}

Component& Component::operator=(const Component& other) {
	owner = other.owner;
	componentID = other.componentID;
	uniqueIDContainer.useID(componentID);
	return *this;
}