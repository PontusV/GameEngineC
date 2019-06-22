#include "Text.h"
#include "ResourceManager.h"
//#include "RectTransform.h"

using namespace Core;


Text::Text(std::string text, const char* fontAddress, int fontSize, Color color, unsigned char layerIndex) : Sprite(layerIndex, color), font(fontAddress, fontSize), text(text) {
} // Constructor

Text::Text() {
} // Constructor

Text::~Text() {
} // Destructor

const char* Text::getText() const {
	return text.c_str();
}

void Text::setText(std::string value) {
	text = value;
	// set size
	/*RectTransform* transform = owner.getComponent<RectTransform>();
	if (transform) {
		transform->setSize(getSize());
	}*/
}

glm::vec2 Text::getSize() {
	return ResourceManager::getInstance().getTextSize(text.c_str(), font);
}

const Font& Text::getFont() const {
	return font;
}