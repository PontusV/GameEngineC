#include "WindowAlignment.h"
#include "Transform.h"
#include "Window.h"

using namespace Core;

WindowAlignment::WindowAlignment() {
}
WindowAlignment::WindowAlignment(Anchor anchor, float offsetX, float offsetY) : anchor(anchor), offset(offsetX, offsetY) {
}


WindowAlignment::~WindowAlignment() {
}

void WindowAlignment::update(float deltaTime) {
	glm::vec2 currentResolution = window->getResolution();

	if (currentResolution != resolution) { // Update
		resolution = currentResolution;
		Transform* transform = owner.getComponent<Transform>();
		if (transform) {
			transform->setX(resolution.x * -anchor.x + offset.x);
			transform->setY(resolution.y * -anchor.y + offset.y);
		}
	}
}