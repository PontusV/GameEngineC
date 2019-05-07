#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

#define M_PI 3.14159265358979323846
static float RADIANS_MAX = M_PI * 2;

using namespace Core;


Transform::Transform(float x, float y, float z, Anchor anchorPoint, float rotation, float scale) : position(x, y), z(z), rotation(rotation), scale(scale), anchor(anchorPoint) {
	//anchor = glm::vec2(0, 0);
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

	changed = true;
}

// ----------------------- Getters and Setters -----------------------------------------

void Transform::rotate(float degrees) {
	float radians = glm::radians(degrees);
	rotation += radians;
	if (rotation > RADIANS_MAX)
		rotation -= RADIANS_MAX;
	else if (rotation < 0)
		rotation += RADIANS_MAX;
	changed = true;
}

void Transform::moveX(float value) {
	position.x += value;
	changed = true;
}

void Transform::moveY(float value) {
	position.y += value;
	changed = true;
}

void Transform::setX(float value) {
	position.x = value;
	//moveX(value - position.x);
	changed = true;
}

void Transform::setY(float value) {
	position.y = value;
	//moveY(value - position.y);
	changed = true;
}

void Transform::pushModelMatrix(const glm::mat4& model) {
	localToWorldMatrix *= model;
}

void Transform::updateLocalToWorldMatrix(const glm::mat4& model) {
	localToWorldMatrix = getLocalModelMatrix() * model;
	worldToLocalMatrix = glm::inverse(localToWorldMatrix);
	changed = false;
}

void Transform::resetLocalToWorldMatrix() {
	localToWorldMatrix = glm::mat4(1.0f);
}

const glm::mat4& Transform::getWorldToLocalMatrix() const {
	return worldToLocalMatrix;
}

/* Calculates Local Model Matrix from local properties (position, rotation, scale). */
glm::mat4 Transform::getLocalModelMatrix() const {
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, glm::vec3(position.x, position.y, 0.0f)); // Anchor position
	matrix = glm::rotate(matrix, rotation, glm::vec3(0, 0, 1)); // Rotate around Anchor position
	matrix = glm::scale(matrix, glm::vec3(scale, scale, 1));
	return matrix;
}

const glm::mat4& Transform::getLocalToWorldMatrix() const {
	return localToWorldMatrix;
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

void Transform::setZ(float value) {
	z = value;
}
bool Transform::hasChanged() {
	return changed;
}

const glm::vec2& Transform::getAnchor() const {
	return anchor;
}

void Transform::setAnchor(Anchor anchorPoint) {
	anchor = anchorPoint;
}