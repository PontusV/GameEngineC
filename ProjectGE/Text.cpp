#include "Text.h"
#include "ResourceManager.h"

using namespace Core;


Text::Text(std::string text, const char* fontAddress, int fontSize, Color color, unsigned char layerIndex) : Sprite(layerIndex, 0, 0, color), font(fontAddress, fontSize) {
	setText(text);
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
	glm::ivec2 size = ResourceManager::getInstance().getTextSize(text.c_str(), font);
	setSize(size);
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