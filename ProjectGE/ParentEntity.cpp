#include "ParentEntity.h"

using namespace Core;

#include "ComponentLoader.h"
REGISTER_LOAD_COMPONENT_IGNORE(ParentEntity);


ParentEntity::ParentEntity(Entity parent) : parent(parent) {
} // Constructor

ParentEntity::ParentEntity() {
}

ParentEntity::~ParentEntity() {
} // Destructor


void ParentEntity::setParent(Entity parent) {
	this->parent = parent;
}

Entity ParentEntity::getParent() const {
	return parent;
}