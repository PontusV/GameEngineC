#include "MouseDrag.h"
#include "Transform.h"
#include "Input.h"

using namespace Core;

MouseDrag::MouseDrag() {
}


MouseDrag::~MouseDrag() {
}


void MouseDrag::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	Transform* transform = owner.getComponent<Transform>();
	if (transform) {
		const glm::vec2& mousePosition = input->getMousePosition();
		offsetX = transform->getX() - mousePosition.x;
		offsetY = transform->getY() - mousePosition.y;
	}
}

void MouseDrag::onMouseDrag(float mouseX, float mouseY) {
	Transform* transform = owner.getComponent<Transform>();
	if (transform) {
		transform->setX(mouseX + offsetX);
		transform->setY(mouseY + offsetY);
	}
}