#include "MouseDrag.h"
#include "RectTransform.h"
#include "Input.h"
#include "TransformMaths.h"

using namespace Core;

MouseDrag::MouseDrag() {
}


MouseDrag::~MouseDrag() {
}


void MouseDrag::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	if (buttoncode == MOUSE_BUTTON_LEFT) {
		RectTransform* transform = owner.getComponent<RectTransform>();
		if (transform) {
			const glm::vec2& mousePosition = input->getMousePosition(); // Screen space, TODO: convert to World Space
			// Offset from top left vertex of the RectTransform
			glm::vec2 mouseOffset = transform->getWorldToLocalMatrix() * glm::inverse(transform->getLocalModelMatrix()) * mousePosition - transform->getRectOffset();
			if (mouseOffset.x > padding && mouseOffset.x < transform->getSize().x - padding &&
				mouseOffset.y > padding && mouseOffset.y < transform->getSize().y - padding) {
				dragging = true;
				offset = transform->getPosition() - mousePosition;
			}
		}
	}
}

void MouseDrag::onMouseButtonReleased(int buttoncode, int mods) {
	dragging = false;
}

void MouseDrag::onMouseDrag(float mouseX, float mouseY) {
	if (dragging) {
		RectTransform* transform = owner.getComponent<RectTransform>();
		if (transform) {
			const glm::vec2& mousePosition = input->getMousePosition(); // Screen space, TODO: convert to World Space
			transform->setPosition(mousePosition + offset);
		}
	}
}