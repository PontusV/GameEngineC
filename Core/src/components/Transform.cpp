#include "Transform.h"
#include "maths/MatrixTransform.h"

#define M_PI 3.14159265358979323846
static float RADIANS_MAX = M_PI * 2;

using namespace Core;


Transform::Transform(float x, float y, float z, float rotation, float scale, TransformSpace space) : position(x, y), z(z), rotation(rotation), scale(scale), space(space) {
}

Transform::Transform() : position(0, 0), z(0), rotation(0.0f), scale(1.0f), space(TransformSpace::World) {
}

Transform::~Transform() {
}

// ----------------------- Getters and Setters -----------------------------------------

void Transform::rotate(float radians) {
	setLocalRotation(rotation + radians);
}

void Transform::moveX(float value) {
	position.x += value;
	changed = true;
}

void Transform::moveY(float value) {
	position.y += value;
	changed = true;
}

void Transform::setLocalRotation(float radians) {
	rotation = radians;
	while (rotation > RADIANS_MAX)
		rotation -= RADIANS_MAX;
	while (rotation < 0)
		rotation += RADIANS_MAX;
	changed = true;
}

void Transform::setLocalPosition(Vector2 pos) {
	position = pos;
	changed = true;
}

void Transform::setPosition(Vector2 pos) {
	setLocalPosition(getWorldToLocalMatrix() * pos);
}

void Transform::setX(float value) {
	Vector2 position = getWorldToLocalMatrix() * Vector2(value, 0);
	setLocalX(position.x);
}

void Transform::setY(float value) {
	Vector2 position = getWorldToLocalMatrix() * Vector2(0, value);
	setLocalX(position.y);
}

void Transform::setLocalX(float value) {
	position.x = value;
	changed = true;
}

void Transform::setLocalY(float value) {
	position.y = value;
	changed = true;
}

void Transform::setSpace(TransformSpace space) {
	changed = true;
}

void Transform::updateLocalToWorldMatrix(const Matrix4& model) {
	localToWorldMatrix = model;
	worldToLocalMatrix = maths::inverse(localToWorldMatrix);
	changed = false;
}

void Transform::resetLocalToWorldMatrix() {
	localToWorldMatrix = Matrix4(1.0f);
}

const Matrix4& Transform::getWorldToLocalMatrix() const {
	return worldToLocalMatrix;
}

void Transform::updateLocalModelMatrix() {
	localModelMatrix = Matrix4(1.0f);
	localModelMatrix = maths::translate(localModelMatrix, Vector3(position.x, position.y, 0.0f));
	localModelMatrix = maths::rotate(localModelMatrix, rotation, Vector3(0, 0, 1));
	localModelMatrix = maths::scale(localModelMatrix, Vector3(scale, scale, 1));
}

/* Calculates Local Model Matrix from local properties (position, rotation, scale). */
const Matrix4& Transform::getLocalModelMatrix() const {
	return localModelMatrix;
}

const Matrix4& Transform::getLocalToWorldMatrix() const {
	return localToWorldMatrix;
}

const float& Transform::getLocalRotation() const {
	return rotation;
}

const Vector2& Transform::getLocalPosition() const {
	return position;
}

const Vector2 Transform::getPosition() const {
	return localToWorldMatrix * position;
}

const TransformSpace& Transform::getSpace() const {
	return space;
}

bool Transform::isInWorldSpace() {
	return space == TransformSpace::World;
}

bool Transform::isInScreenSpace() {
	return space == TransformSpace::Screen;
}

const float& Transform::getZ() const {
	return z;
}

void Transform::setZ(float value) {
	z = value;
}

void Transform::setChanged() {
	changed = true;
}

bool Transform::hasChanged() {
	return changed;
}