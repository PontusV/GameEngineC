#include "ParentEntity.h"

using namespace Core;


ParentEntity::ParentEntity(Handle parent) : parent(parent) {
} // Constructor

ParentEntity::ParentEntity() {
}

ParentEntity::~ParentEntity() {
} // Destructor


void ParentEntity::setParent(Handle parent) {
	this->parent = parent;
}

Handle* ParentEntity::getParent() {
	return &parent;
}