#include "WindowAnchor.h"
#include "components/Transform.h"
#include "graphics/Window.h"

using namespace Core;

WindowAnchor::WindowAnchor() {
}
WindowAnchor::WindowAnchor(Anchor anchor, float offsetX, float offsetY) : anchor(anchor), offset(offsetX, offsetY) {
}


WindowAnchor::~WindowAnchor() {
}


void WindowAnchor::initialize() {
	onWindowResize();
}

void WindowAnchor::onWindowResize() {
	Transform* transform = owner.getComponent<Transform>();
	if (transform) {
		Vector2 resolution = window->getResolution();
		transform->setLocalX(resolution.x * anchor.x + offset.x);
		transform->setLocalY(resolution.y * anchor.y + offset.y);
	}
}