#include "WindowAnchor.h"
#include "Transform.h"
#include "Window.h"

using namespace Core;

WindowAnchor::WindowAnchor() {
}
WindowAnchor::WindowAnchor(Anchor anchor, float offsetX, float offsetY) : anchor(anchor), offset(offsetX, offsetY) {
}


WindowAnchor::~WindowAnchor() {
}


void WindowAnchor::start() {
	onWindowResize();
}

void WindowAnchor::onWindowResize() {
	glm::vec2 resolution = window->getResolution();
	Transform* transform = owner.getComponent<Transform>();
	if (transform) {
		transform->setX(resolution.x * anchor.x + offset.x);
		transform->setY(resolution.y * anchor.y + offset.y);
	}
}