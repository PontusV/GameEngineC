#include "Text.h"
#include "ResourceManager.h"
#include "Maths/MatrixTransform.h"

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
	refresh();
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

void Text::updateTransforms() {
	refresh();
}

void Text::refresh() {
	// Remove current
	textSprites.clear();
	textTransforms.clear();
	// Create new
	createText();
}

void Text::createText() {
	if (text.empty()) return;

	RectTransform* transform = owner.getComponent<RectTransform>();
	if (transform) {
		Matrix4 localToWorldMatrix = transform->getLocalToWorldMatrix() * transform->getLocalModelMatrix();
		// Create text sprites
		TextData2D textData = ResourceManager::getInstance().createText(text, font);
		std::vector<CharTexture2D>& textTextures = textData.textures;
		// Calculate offset
		Vector2 pivot = transform->getPivot();
		float offsetX = textData.size.x * -pivot.x;
		float offsetY = textData.size.y * -pivot.y + textData.size.y;

		for (CharTexture2D& c : textTextures) {
			// Create new Transform for Character Sprite
			RectTransform spriteTransform(offsetX + c.offset.x, offsetY + c.offset.y, c.texture.size.x, c.texture.size.y, transform->getZ() + 0.00001f, Alignment::TOP_LEFT, 0.0f, 1.0f);
			// Set world model matrix of new text sprite
			spriteTransform.updateLocalToWorldMatrix(localToWorldMatrix);

			textSprites.push_back(c.texture);
			textTransforms.push_back(spriteTransform);
		}
	}
}

const std::vector<Texture2D>& Text::getTextSprites() const {
	return textSprites;
}
const std::vector<RectTransform>& Text::getTextTransforms() const {
	return textTransforms;
}