#include "Text.h"

using namespace Core;


Text::Text(const char* text, const char* fontAddress, int fontSize, glm::vec4 color, unsigned short layerIndex) : GraphicComponent(layerIndex), font(fontAddress, fontSize), color(color) {
	setText(text);
} // Constructor

Text::Text() {
} // Constructor

Text::~Text() {
} // Destructor

const char* Text::getText() const {
	return text.c_str();
}

void Text::setText(std::string nText) {
	text = nText;
}

const Font& Text::getFont() const {
	return font;
}
const glm::vec4& Text::getColor() const {
	return color;
}

// ------------------------------- Serializable ----------------------------------------

void Text::serialize(std::ostream& os) const {
	GraphicComponent::serialize(os);

	os.write(text.c_str(), text.size() + 1);		// Text string
	font.serialize(os);								// Font
	os.write((char*)&color.x, sizeof(color.x));		// Color x
	os.write((char*)&color.y, sizeof(color.y));		// Color y
	os.write((char*)&color.z, sizeof(color.z));		// Color z
	os.write((char*)&color.w, sizeof(color.w));		// Color w
}

void Text::deserialize(std::istream& is) {
	GraphicComponent::deserialize(is);

	std::getline(is, text, '\0');					// Text string
	font.deserialize(is);							// Font
	is.read((char*)&color.x, sizeof(color.x));		// Color x
	is.read((char*)&color.y, sizeof(color.y));		// Color y
	is.read((char*)&color.z, sizeof(color.z));		// Color z
	is.read((char*)&color.w, sizeof(color.w));		// Color w
}