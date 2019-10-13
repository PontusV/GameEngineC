#include "RectButton.h"
#include "RectSprite.h"
#include "InputEvent.h"
#include <iostream>

using namespace Core;

RectButton::RectButton() {
	colors[DEFAULT]			= { 255,255,255,255 };
	colors[PRESSED_DOWN]	= { 255,255,255,255 };
	colors[HOVER_OVER]		= { 255,255,255,255 };
	colors[DISABLED]		= { 255,255,255,255 };
}

RectButton::~RectButton() {
}

void RectButton::awake() {
	changeState(DISABLED);
}

void RectButton::onEnable() {
	changeState(DEFAULT);
}

void RectButton::onDisable() {
	changeState(DISABLED);
}

void RectButton::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	if (state == DISABLED) return;
	changeState(PRESSED_DOWN);
	if (buttoncode == MOUSE_BUTTON_LEFT)
		leftPressed = true;
	else if (buttoncode == MOUSE_BUTTON_MIDDLE)
		middlePressed = true;
	else if (buttoncode == MOUSE_BUTTON_RIGHT)
		rightPressed = true;
}

void RectButton::onMouseButtonReleasedAsButton(int buttoncode, int mods) {
	if (state == DISABLED) return;
	if (buttoncode == MOUSE_BUTTON_LEFT && leftPressed) {
		leftPressed = false;
		clickFunction.call();
	}
	else if (buttoncode == MOUSE_BUTTON_MIDDLE && middlePressed) {
		middlePressed = false;
		clickFunction.call();
	}
	else if (buttoncode == MOUSE_BUTTON_RIGHT && rightPressed) {
		rightPressed = false;
		clickFunction.call();
	}
	if (state != DISABLED && !leftPressed && !middlePressed && !rightPressed)
		changeState(HOVER_OVER);
}

void RectButton::onHoverover() {
	if (state == DISABLED) return;
	changeState(HOVER_OVER);
}

void RectButton::onHoverout() {
	if (state == DISABLED) return;
	if (state == PRESSED_DOWN || state == HOVER_OVER)
		changeState(DEFAULT);
}

void RectButton::changeState(ButtonState state) {
	if (this->state == state) return;
	this->state = state;

	// Change color
	RectSprite* rect = owner.getComponent<RectSprite>();
	if (rect) {
		rect->setColor(colors[state]);
	}
	else {
		std::cout << "RectButton: Could not find Rect component!\n";
	}
}