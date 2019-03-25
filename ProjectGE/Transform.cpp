#include "Transform.h"

using namespace Core;

#include "ComponentLoader.h"
REGISTER_LOADABLE_COMPONENT(Transform);


Transform::Transform(float x, float y, float z, TransformAnchor anchorPoint, float rotation, float scale) : position(x, y), z(z), rotation(rotation), scale(scale) {
	setAnchor(anchorPoint);
}

Transform::Transform() : position(0, 0) {
}

Transform::~Transform() {
}

glm::vec2 Transform::calculateOffset(const glm::vec2& size) const {
	return anchor * size;
}


// ------------------------------- Serializable ----------------------------------------

void Transform::serialize(std::ostream& os) const {
	Component::serialize(os);

	os.write((char*)&position.x, sizeof(position.x));		// Position x
	os.write((char*)&position.y, sizeof(position.y));		// Position y
	os.write((char*)&z, sizeof(z));							// Position z
	os.write((char*)&rotation, sizeof(rotation));			// Rotation
	os.write((char*)&scale, sizeof(scale));					// Scale
	os.write((char*)&anchor.x, sizeof(anchor.x));			// Anchor x
	os.write((char*)&anchor.y, sizeof(anchor.y));			// Anchor y
}

void Transform::deserialize(std::istream& is) {
	Component::deserialize(is);

	is.read((char*)&position.x, sizeof(position.x));		// Position x
	is.read((char*)&position.y, sizeof(position.y));		// Position y
	is.read((char*)&z, sizeof(z));							// Position z
	is.read((char*)&rotation, sizeof(rotation));			// Rotation
	is.read((char*)&scale, sizeof(scale));					// Scale
	is.read((char*)&anchor.x, sizeof(anchor.x));			// Anchor x
	is.read((char*)&anchor.y, sizeof(anchor.y));			// Anchor y
}

// ----------------------- Getters and Setters -----------------------------------------

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

const glm::vec2& Transform::getAnchor() const {
	return anchor;
}

void Transform::setAnchor(TransformAnchor anchorPoint) {
	anchor = glm::vec2(0, 0);

	float xOffset = -(float)(anchorPoint % 3) / 2;
	float yOffset = -(float)((int)anchorPoint / 3) / 2;
	anchor = glm::vec2(xOffset, yOffset);
}