#include "Button.h"
#include "input/InputEvent.h"

using namespace Core;


Button::Button(Image defaultImage, Image pressedImage, Image hoverImage) : images{defaultImage, pressedImage, hoverImage} {
}

Button::Button(const char* defaultImage, const char* pressedImage, const char* hoverImage) : images{ Image(defaultImage), Image(pressedImage), Image(hoverImage) } {
}

Button::~Button() {
}


void Button::initialize() {
	changeState(DISABLED);
}

void Button::onEnable() {
	changeState(DEFAULT);
}

void Button::onDisable() {
	changeState(DISABLED);
}

void Button::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	if (state == DISABLED) return;
	changeState(PRESSED_DOWN);
	if (buttoncode == MOUSE_BUTTON_LEFT)
		leftPressed = true;
	else if (buttoncode == MOUSE_BUTTON_MIDDLE)
		middlePressed = true;
	else if (buttoncode == MOUSE_BUTTON_RIGHT)
		rightPressed = true;
}

void Button::onMouseButtonReleasedAsButton(int buttoncode, int mods) {
	if (state == DISABLED) return;
	if (buttoncode == MOUSE_BUTTON_LEFT && leftPressed) {
		leftPressed = false;
		onLeftClick.invoke();
	}
	else if (buttoncode == MOUSE_BUTTON_MIDDLE && middlePressed) {
		middlePressed = false;
		onMiddleClick.invoke();
	}
	else if (buttoncode == MOUSE_BUTTON_RIGHT && rightPressed) {
		rightPressed = false;
		onRightClick.invoke();
	}
	if (state != DISABLED && !leftPressed && !middlePressed && !rightPressed)
		changeState(HOVER_OVER);
}

void Button::onHoverOver() {
	if (state == DISABLED) return;
	changeState(HOVER_OVER);
}

void Button::onHoverOut() {
	if (state == DISABLED) return;
	if (state == PRESSED_DOWN || state == HOVER_OVER)
		changeState(DEFAULT);
}

void Button::changeState(ButtonState state) {
	if (this->state == state) return;
	this->state = state;

	// Change image
	Image* image = owner.getComponent<Image>();
	if (image) {
		images[state].reload();
		// Change texture
		image->set(images[state].getFileName());
		// Change color
		image->setColor(images[state].getColor());
		// Change shader
		image->setShader(images[state].getShader());
	}
	else {
		std::cout << "Button: Could not find Image component!\n";
	}
}

Image* Button::getImages() {
	return images;
}