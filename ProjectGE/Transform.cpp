#include "Transform.h"
using namespace Core;


Transform::Transform(float x, float y, float z, TransformAnchor anchorPoint, float rotation, float scale) : position(x, y), z(z), rotation(rotation), scale(scale)
{
	setAnchor(anchorPoint);
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

const float& Transform::getX() const {
	return position.x;
}

const float& Transform::getY() const {
	return position.y;
}

const float& Transform::getZ() const {
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

/* Calculate anchor values from TransformAnchor */
void Transform::setAnchor(TransformAnchor anchorPoint) {
	anchor = glm::vec2(0, 0);

	float xOffset = -(float)(anchorPoint % 3) / 2;
	float yOffset = -(float)((int)anchorPoint / 3) / 2;
	anchor = glm::vec2(xOffset, yOffset);
}

const glm::vec2& Transform::getAnchor() const {
	return anchor;
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

glm::vec2 Transform::calculateOffset(const glm::vec2& size) const {
	return anchor * size;
}