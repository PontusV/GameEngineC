#include "DragAndResize.h"
#include "Input.h"
#include "Maths/MatrixTransform.h"
#include "RectTransform.h"

using namespace Core;

DragAndResize::DragAndResize()
{
}


DragAndResize::~DragAndResize()
{
}


void DragAndResize::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	if (buttoncode == MOUSE_BUTTON_LEFT) {
		RectTransform* transform = owner.getComponent<RectTransform>();
		if (transform) {
			const Vector2& mousePosition = input->getMousePosition(); // Screen space, TODO: convert to World Space
			Vector2 mousePos = transform->getWorldToLocalMatrix() * maths::inverse(transform->getLocalModelMatrix()) * mousePosition;
			mouseOffset = mousePos - transform->getRectOffset();
			// Left
			if (mouseOffset.x <= edgeSize && left) {
				draggingLeft = true;
			} // Right
			else if (mouseOffset.x >= transform->getSize().x - edgeSize && right) {
				draggingRight = true;
			}
			// Top
			if (mouseOffset.y <= edgeSize && top) {
				draggingTop = true;
			}
			// Bottom
			else if (mouseOffset.y >= transform->getSize().y - edgeSize && bottom) {
				draggingBottom = true;
			}
		}
	}
}

void DragAndResize::onMouseDrag(float mouseX, float mouseY) {
	RectTransform* transform = owner.getComponent<RectTransform>();
	if (transform) {
		const Vector2& mousePosition = input->getMousePosition(); // Screen space, TODO: convert to World Space
		Vector2 mousePos = transform->getWorldToLocalMatrix() * maths::inverse(transform->getLocalModelMatrix()) * mousePosition;
		Vector2 move = mousePos - transform->getRectOffset() - mouseOffset;

		if (draggingLeft) {
			if (move.x > transform->getSize().x - edgeSize * 2)
				move.x = transform->getSize().x - edgeSize * 2;
			float newWidth = transform->getSize().x - move.x;
			transform->setWidth(newWidth);
			transform->moveX(move.x * (1.0f - transform->getPivot().x));
		}
		else if (draggingRight) {
			if (move.x < edgeSize * 2 - transform->getSize().x)
				move.x = edgeSize * 2 - transform->getSize().x;
			float newWidth = transform->getSize().x + move.x;
			transform->setWidth(newWidth);
			transform->moveX(move.x * transform->getPivot().x);

			mouseOffset.x += move.x;
		}
		if (draggingTop) {
			if (move.y > transform->getSize().y - edgeSize * 2)
				move.y = transform->getSize().y - edgeSize * 2;
			float newHeight = transform->getSize().y - move.y;
			transform->setHeight(newHeight);
			transform->moveY(move.y * (1.0f - transform->getPivot().y));
		}
		else if (draggingBottom) {
			if (move.y < edgeSize * 2 - transform->getSize().y)
				move.y = edgeSize * 2 - transform->getSize().y;
			float newHeight = transform->getSize().y + move.y;
			transform->setHeight(newHeight);
			transform->moveY(move.y * transform->getPivot().y);

			mouseOffset.y += move.y;
		}
	}
}

void DragAndResize::onMouseButtonReleased(int buttoncode, int mods) {
	draggingLeft = false;
	draggingTop = false;
	draggingRight = false;
	draggingBottom = false;
}