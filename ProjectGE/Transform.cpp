#include "Transform.h"
#include "TransformMaths.h"
#include <glm/gtc/matrix_transform.hpp>

#define M_PI 3.14159265358979323846
static float RADIANS_MAX = M_PI * 2;

using namespace Core;


Transform::Transform(float x, float y, float z, float rotation, float scale) : position(x, y), z(z), rotation(rotation), scale(scale) {
}

Transform::Transform() : position(0, 0) {
}

Transform::~Transform() {
}


// ------------------------------- Serializable ----------------------------------------

void Transform::serialize(std::ostream& os) const {
	Component::serialize(os);

	os.write((char*)&position.x, sizeof(position.x));		// Position x
	os.write((char*)&position.y, sizeof(position.y));		// Position y
	os.write((char*)&z, sizeof(z));							// Position z
	os.write((char*)&rotation, sizeof(rotation));			// Rotation
	os.write((char*)&scale, sizeof(scale));					// Scale
}

void Transform::deserialize(std::istream& is) {
	Component::deserialize(is);

	is.read((char*)&position.x, sizeof(position.x));		// Position x
	is.read((char*)&position.y, sizeof(position.y));		// Position y
	is.read((char*)&z, sizeof(z));							// Position z
	is.read((char*)&rotation, sizeof(rotation));			// Rotation
	is.read((char*)&scale, sizeof(scale));					// Scale

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
	changed = true;
}

void Transform::setY(float value) {
	position.y = value;
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
	matrix = glm::translate(matrix, glm::vec3(position.x, position.y, 0.0f));
	matrix = glm::rotate(matrix, rotation, glm::vec3(0, 0, 1));
	matrix = glm::scale(matrix, glm::vec3(scale, scale, 1));
	return matrix;
}

const glm::mat4& Transform::getLocalToWorldMatrix() const {
	return localToWorldMatrix;
}

const float& Transform::getLocalRotation() const {
	return rotation;
}

const glm::vec2& Transform::getLocalPosition() const {
	return position;
}

const glm::vec2 Transform::getPosition() const {
	return localToWorldMatrix * position;
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