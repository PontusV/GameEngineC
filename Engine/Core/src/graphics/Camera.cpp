#include "Camera.h"
#include "Window.h"
#include "Maths/Vector3.h"
#include "maths/MatrixTransform.h"
#include <GLFW/glfw3.h>
using namespace Core;

Camera::Camera(Window* window) : window(window) {

}

Camera::~Camera() {

}

const Vector2& Camera::getPosition() {
	return position;
}

float Camera::getX() {
	return position.x;
}

float Camera::getY() {
	return position.y;
}

void Camera::setPosition(float x, float y) {
	position = Vector2(x, y);
}

void Camera::setPosition(Vector2 vec) {
	position = vec;
}

void Camera::moveX(float value) {
	position.x += value;
}

void Camera::moveY(float value) {
	position.y += value;
}

void Camera::rotate(float value) {
	rotation += value;
}

void Camera::setRotation(float value) {
	rotation = value;
}

float Camera::getRotation() {
	return rotation;
}

float Camera::getScale() {
	return scale;
}

void Camera::setScale(float value) {
	scale = value;
}

const Matrix4& Camera::getWorldToScreenMatrix() const {
	return worldToScreenMatrix;
}

const Matrix4& Camera::getViewMatrix() const {
	return viewMatrix;
}

void Camera::updateViewMatrix() {
	viewMatrix = Matrix4(1.0f);
	Vector2 resolution = window->getResolution();
	Vector2 center = Vector2(resolution.x / 2, resolution.y / 2);

	viewMatrix = maths::translate(viewMatrix, Vector3(center.x, center.y, 0.0f));
	viewMatrix = maths::rotate(viewMatrix, -rotation, Vector3(0.0f, 0.0f, 1.0f));
	viewMatrix = maths::scale(viewMatrix, Vector3(scale, scale, 1.0f));
	viewMatrix = maths::translate(viewMatrix, Vector3(-position.x,-position.y, 0.0f));

	worldToScreenMatrix = maths::inverse(viewMatrix);
}

void Camera::setProjectionMatrix(Matrix4& matrix) {
	projection = matrix;
}

const Matrix4& Camera::getProjectionMatrix() const {
	return projection;
}