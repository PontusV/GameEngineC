#include "ParentEntity.h"
#include "Behaviour.h"
#include <iostream>
#include <stdexcept>

using namespace Core;


ParentEntity::ParentEntity(Handle parent) : parent(parent) {
} // Constructor

ParentEntity::ParentEntity() {
}

ParentEntity::~ParentEntity() {
} // Destructor


void ParentEntity::setParent(Handle parent) {
	if (!parent.refresh()) {
		std::cout << "ParentEntity::setParent::ERROR Cannot add invalid Parent to Entity(" << owner.getEntity().getID() << ")\n";
		throw std::invalid_argument("ParentEntity::setParent::ERROR Cannot add invalid Parent");
	}
	this->parent = parent;
	// Notify scripts
	std::vector<Behaviour*> scripts = owner.getComponents<Behaviour>();
	for (Behaviour* script : scripts) {
		script->onParentChanged();
	}
}

Handle ParentEntity::getParent() {
	parent.refresh(); // Make sure its updated
	return parent;
}