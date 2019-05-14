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

// ------------------------------- Serializable ----------------------------------------

void Text::serialize(std::ostream& os) const {
	Sprite::serialize(os);

	os.write(text.c_str(), text.size() + 1);		// Text string
	font.serialize(os);								// Font
}

void Text::deserialize(std::istream& is) {
	Sprite::deserialize(is);

	std::getline(is, text, '\0');					// Text string
	font.deserialize(is);							// Font
}