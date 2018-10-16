#include "Transform.h"



Transform::Transform(float x, float y) : position(x, y)
{
}

Transform::Transform() : position(0, 0)
{
}

Transform::~Transform()
{
}

/* Retrieves image adress from file to load texture and passes instream to Graphics2D to load its own values in the Image constructor this function calls */
Transform* Transform::load(std::istream& is) {
	Transform* transform = new Transform();
	transform->deserialize(is);
	return transform;
}

void Transform::serialize(std::ostream& os) const {
	os << position;
}

void Transform::deserialize(std::istream& is) {
	is >> position;
}

float Transform::getX() const {
	return position.getX();
}

float Transform::getY() const {
	return position.getY();
}

void Transform::setX(float x) {
	position.setX(x);
}

void Transform::setY(float y) {
	position.setY(y);
}