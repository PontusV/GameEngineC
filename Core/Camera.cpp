#include "Camera.h"
#include "Window.h"
#include "Maths/Vector3.h"
#include "Maths/MatrixTransform.h"
#include <GLFW/glfw3.h>
using namespace Core;

Camera::Camera(Window* window) : window(window) {

}

Camera::~Camera() {

}

const Vector2& Camera::getPosition() {
	return position;
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

Matrix4 Camera::getViewMatrix() {
	Matrix4 view;
	Vector2 resolution = window->getResolution();
	Vector2 center = Vector2(resolution.x/2, resolution.y/2);

	view = maths::translate(view, Vector3(center.x, center.y, 0.0f));
	view = maths::rotate(view, -rotation, Vector3(0.0f, 0.0f, 1.0f));
	view = maths::translate(view, Vector3(-center.x - position.x, -center.y - position.y, 0.0f));

	return view;
}