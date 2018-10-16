#include "Entity.h"
#include "Component.h"
#include <iostream>
#include "Image.h"
#include "Text.h"
#include "HandleManager.h"
#include "ComponentManager.h"


Entity::Entity() {}


Entity::~Entity() {}


void Entity::setID(Handle handleID) {
	id = handleID;
}

Handle Entity::getID() const {
	return id;
}

/* Save */
void Entity::serialize(std::ostream& os) const {
	uint32_t handleValues = id; //Converts Handle to unint32_t
	os.write((char*)&handleValues, sizeof(uint32_t));
}

/* Load */
void Entity::deserialize(std::istream& is) {
	uint32_t handleValues;
	is.read((char*)&handleValues, sizeof(uint32_t));
	Handle component = Handle(handleValues);
}

//Entity::Entity(Entity&& other) noexcept : id(other.id) {}