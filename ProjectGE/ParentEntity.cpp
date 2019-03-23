#include "ParentEntity.h"

using namespace Core;

ParentEntity::ParentEntity(std::istream& is) {
}
ParentEntity::ParentEntity(Entity parent) : parent(parent)
{
} // Constructor


ParentEntity::~ParentEntity()
{
} // Destructor


void ParentEntity::setParent(Entity parent) {
	this->parent = parent;
}

Entity ParentEntity::getParent() const {
	return parent;
}


// Save & Load operator
void ParentEntity::serialize(std::ostream& os) const {

}

void ParentEntity::deserialize(std::istream& is) {

}