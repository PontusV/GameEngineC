#include "Button.h"
#include "InputEvent.h"

using namespace Core;


Button::Button(Image defaultImage, Image pressedImage, Image hoverImage) : images{defaultImage, pressedImage, hoverImage} {
	for (std::size_t i = 0; i < 3; i++) {
		colors[i] = Color(255,255,255,255);
	}
} // Constructor

Button::Button(const char* defaultImage, const char* pressedImage, const char* hoverImage) : images{ Image(defaultImage, 0), Image(pressedImage, 0), Image(hoverImage, 0) } {
	for (std::size_t i = 0; i < 3; i++) {
		colors[i] = Color(255, 255, 255, 255);
	}
} // Constructor

Button::~Button() {
} // Destructor


void Button::awake() {
	changeState(DEFAULT);
}

void Button::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	if (buttoncode == MOUSE_BUTTON_LEFT) {
		changeState(PRESSED_DOWN);
	}
}
void Button::onMouseButtonReleasedAsButton(int buttoncode, int mods) {
	if (buttoncode == MOUSE_BUTTON_LEFT) {
		changeState(HOVER_OVER);
	}
}

void Button::onHoverover() {
	changeState(HOVER_OVER);
}

void Button::onHoverout() {
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
		// Change image
		image->setTexture(images[state].getFileName(), images[state].getTexture());
		// Change color
		image->setColor(colors[state]);
	}
	else {
		std::cout << "Button: Could not find Image component!\n";
	}
}

Image* Button::getImages() {
	return images;
}

// ------------------------------- Serializable ----------------------------------------

void Button::serialize(std::ostream& os) const {
	Behaviour::serialize(os);

	for (std::size_t i = 0; i < 3; i++) {
		images[i].serialize(os);								// Images
		colors[i].serialize(os);								// Colors
	}

	os.write((char*)&state, sizeof(state));						// State
}

void Button::deserialize(std::istream& is) {
	Behaviour::deserialize(is);

	for (std::size_t i = 0; i < 3; i++) {
		images[i].deserialize(is);								// Images
		colors[i].deserialize(is);								// Colors
	}

	is.read((char*)&state, sizeof(state));						// State
}