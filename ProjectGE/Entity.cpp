#include "Entity.h"
#include <iostream>
using namespace Core;

Entity::Entity(std::size_t entityID) : id(entityID) {}

Entity::Entity() {}


Entity::~Entity() {}


void Entity::setID(std::size_t entityID) {
	id = entityID;
}

std::size_t Entity::getID() const {
	return id;
}

//Entity::Entity(Entity&& other) noexcept : id(other.id) {}