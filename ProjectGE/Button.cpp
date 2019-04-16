#include "Button.h"

using namespace Core;


Button::Button(Image defaultImage, Image pressedImage, Image hoverImage) : images{defaultImage, pressedImage, hoverImage} {
} // Constructor

Button::Button(const char* defaultImage, const char* pressedImage, const char* hoverImage, unsigned int width, unsigned int height) : images{ Image(defaultImage, 0, width, height), Image(pressedImage, 0, width, height), Image(hoverImage, 0, width, height) } {
} // Constructor

Button::~Button() {
} // Destructor


/* Initializes handle to image component with same owner. */
void Button::init() {
	changeState(ButtonState::DEFAULT);
}

void Button::onLeftClickPressed() {
	Interactable::onLeftClickPressed();
	changeState(ButtonState::PRESSED_DOWN);
}

void Button::onLeftClickReleased() {
	if (state == ButtonState::PRESSED_DOWN) {
		Interactable::onLeftClickReleased();
		changeState(ButtonState::HOVER_OVER);
	}
}

void Button::onHoverover() {
	Interactable::onHoverover();
	changeState(ButtonState::HOVER_OVER);
}

void Button::onHoverout() {
	Interactable::onHoverout();
	if (state == ButtonState::PRESSED_DOWN || state == ButtonState::HOVER_OVER)
		changeState(ButtonState::DEFAULT);
}

void Button::changeState(ButtonState state) {
	if (this->state == state) return;
	this->state = state;

	// Change image
	Image* image = owner.getComponent<Image>();
	if (image) {
		images[state].reload();
		image->setTexture(images[state].getFileName(), images[state].getTexture());
		image->setSize(images[state].getSize());
	}
	else {
		std::cout << "Button: Could not find Image component!\n";
	}
}

Image* Button::getImages() {
	return images;
}

void Button::setButtonPressCallback(ui::LeftClickPressFun fun) {
	Interactable::setLeftClickPressedCallback(fun);
}

void Button::setButtonReleaseCallback(ui::LeftClickReleaseFun fun) {
	Interactable::setLeftClickReleasedCallback(fun);
}

// ------------------------------- Serializable ----------------------------------------

void Button::serialize(std::ostream& os) const {
	Component::serialize(os);

	images[0].serialize(os);							// Image 1
	images[1].serialize(os);							// Image 2
	images[2].serialize(os);							// Image 3

	os.write((char*)&state, sizeof(state));				// State
}

void Button::deserialize(std::istream& is) {
	Component::deserialize(is);

	images[0].deserialize(is);							// Image 1
	images[1].deserialize(is);							// Image 2
	images[2].deserialize(is);							// Image 3

	is.read((char*)&state, sizeof(state));				// State
}