#include "Transform.h"
using namespace GameEngine;


Transform::Transform(float x, float y, float z, float rotation) : position(x, y), z(z), rotation(rotation)
{
}

Transform::Transform() : position(0, 0)
{
}

Transform::Transform(std::istream& is) {
	deserialize(is);
}

Transform::~Transform()
{
}

const float& Transform::getRotation() const {
	return rotation;
}

const glm::vec2& Transform::getPosition() const {
	return position;
}

float Transform::getX() const {
	return position.x;
}

float Transform::getY() const {
	return position.y;
}

float Transform::getZ() const {
	return z;
}

void Transform::setX(float x) {
	position.x = x;
}

void Transform::setY(float y) {
	position.y = y;
}

void Transform::setZ(float z) {
	this->z = z;
}

/* Save */
void Transform::serialize(std::ostream& os) const {
	//Save component ID; loaded in GameObject.cpp to identify which kind of Component to load
	os.write((char*)&TYPE_ID, sizeof(decltype(TYPE_ID)));
	//position.serialize(os);
	os.write((char*)&z, sizeof(decltype(z)));
}

/* Load */
void Transform::deserialize(std::istream& is) {
	//position.deserialize(is);
	is.read((char*)&z, sizeof(decltype(z)));
}