#include "Text.h"
#include "ResourceManager.h"
//#include "RectTransform.h"

using namespace Core;


Text::Text(std::wstring text, const char* fontAddress, int fontSize, Color color) : Sprite(color), font(fontAddress, fontSize), text(text) {
} // Constructor
Text::Text(std::string text, const char* fontAddress, int fontSize, Color color) : Sprite(color), font(fontAddress, fontSize), text(text.begin(), text.end()) {
} // Constructor

Text::Text() {
} // Constructor

Text::~Text() {
} // Destructor

const wchar_t* Text::getText() const {
	return text.c_str();
}

void Text::setText(std::wstring value) {
	text = value;
	// set size
	/*RectTransform* transform = owner.getComponent<RectTransform>();
	if (transform) {
		transform->setSize(getSize());
	}*/
}
void Text::setText(std::string text) {
	setText(std::wstring(text.begin(), text.end()));
}

Vector2 Text::getSize() {
	return ResourceManager::getInstance().getTextSize(text, font);
}

const Font& Text::getFont() const {
	return font;
}