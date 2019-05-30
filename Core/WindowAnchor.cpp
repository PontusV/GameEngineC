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


void WindowAnchor::awake() {
	onWindowResize();
}

void WindowAnchor::onWindowResize() {
	Transform* transform = owner.getComponent<Transform>();
	if (transform) {
		glm::vec2 resolution = window->getResolution();
		transform->setLocalX(resolution.x * anchor.x + offset.x);
		transform->setLocalY(resolution.y * anchor.y + offset.y);
	}
}