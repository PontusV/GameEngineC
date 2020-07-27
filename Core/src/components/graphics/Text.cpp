#include "Text.h"
#include "engine/ResourceManager.h"

using namespace Core;


Text::Text(std::wstring text, Font font, Color color)
	: font(font), text(text), Graphic(0, ResourceManager::getInstance().loadShader("resources/shaders/sprite"), color) {
} // Constructor
Text::Text(std::string text, Font font, Color color)
	: font(font), text(text.begin(), text.end()), Graphic(0, ResourceManager::getInstance().loadShader("resources/shaders/sprite"), color) {
} // Constructor

Text::Text() {
} // Constructor

Text::~Text() {
} // Destructor

const std::wstring& Text::getText() const {
	return text;
}

void Text::setText(std::wstring value) {
	text = value;
	setDirty();
}
void Text::setText(std::string text) {
	setText(std::wstring(text.begin(), text.end()));
}

const Font& Text::getFont() const {
	return font;
}

void Text::setDirty(bool value) {

}
bool Text::isDirty() const {
	return dirty;
}

void Text::update(std::vector<TextSprite> sprites) {
	dirty = false;
	this->sprites = sprites;
}

const std::vector<TextSprite>& Text::getSprites() const {
	return sprites;
}