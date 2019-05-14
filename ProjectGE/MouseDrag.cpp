#include "MouseDrag.h"
#include "Transform.h"
#include "Input.h"
#include "TransformMaths.h"

using namespace Core;

MouseDrag::MouseDrag() {
}


MouseDrag::~MouseDrag() {
}


void MouseDrag::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	Transform* transform = owner.getComponent<Transform>();
	if (transform) {
		const glm::vec2& mousePosition = input->getMousePosition();
		offset = transform->getWorldToLocalMatrix() * mousePosition;
	}
}

void MouseDrag::onMouseDrag(float mouseX, float mouseY) {
	Transform* transform = owner.getComponent<Transform>();
	if (transform) {
		transform->setX(mouseX - offset.x);
		transform->setY(mouseY - offset.y);
	}
}