#include "RectButton.h"
#include "RectSprite.h"
#include "InputEvent.h"
#include <iostream>

using namespace Core;

RectButton::RectButton(unsigned char layerIndex) {
	colors[DEFAULT]			= { 255,255,255,255 };
	colors[PRESSED_DOWN]	= { 255,255,255,255 };
	colors[HOVER_OVER]		= { 255,255,255,255 };
	setLayerIndex(layerIndex);
}

RectButton::~RectButton() {
}

void RectButton::setLayerIndex(unsigned char value) {
	layerIndex = value;
	RectSprite* sprite = owner.getComponent<RectSprite>();
	if (sprite) {
		sprite->setLayerIndex(value);
	}
}

void RectButton::awake() {
	//std::cout << owner.hasComponent<Rect>() << "\n";
	//if (!owner.hasComponent<Rect>())
		//owner.addComponent<Rect>(width, height, colors[DEFAULT], layerIndex);
	changeState(DEFAULT);
}

void RectButton::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	if (buttoncode == MOUSE_BUTTON_LEFT) {
		changeState(PRESSED_DOWN);
	}
}
void RectButton::onMouseButtonReleasedAsButton(int buttoncode, int mods) {
	if (buttoncode == MOUSE_BUTTON_LEFT) {
		changeState(HOVER_OVER);
		clickFunction.call();
	}
}

void RectButton::onHoverover() {
	changeState(HOVER_OVER);
}

void RectButton::onHoverout() {
	if (state == PRESSED_DOWN || state == HOVER_OVER)
		changeState(DEFAULT);
}

void RectButton::changeState(ButtonState state) {
	if (this->state == state) return;
	this->state = state;

	// Change image
	RectSprite* rect = owner.getComponent<RectSprite>();
	if (rect) {
		// Change color
		rect->setColor(colors[state]);
	}
	else {
		std::cout << "RectButton: Could not find Rect component!\n";
	}
}