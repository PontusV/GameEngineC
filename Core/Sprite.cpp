#include "Sprite.h"

using namespace Core;


Sprite::Sprite(unsigned char layerIndex, Color color) : layerIndex(layerIndex), color(color), clipEnabled(false) {
} // Constructor

Sprite::~Sprite() {
} // Destructor

void Sprite::setLayerIndex(unsigned char index) {
	layerIndex = index;
}

unsigned char Sprite::getLayerIndex() const {
	return layerIndex;
}

const Color& Sprite::getColor() const {
	return color;
}

void Sprite::setColor(Color value) {
	color = value;
}

const std::vector<glm::vec2>& Sprite::getClipMaskVertices() const {
	return clipMaskVertices;
}

void Sprite::clip(glm::vec2 clipVertices[4]) {
	clipEnabled = true;
	// Add mask to list
	for (int i = 0; i < 4; i++) {
		clipMaskVertices.push_back(clipVertices[i]);
	}
}

void Sprite::resetClipping() {
	clipEnabled = false;
	clipMaskVertices.clear();
}

bool Sprite::isClipEnabled() const {
	return clipEnabled;
}

// ------------------------------- Serializable ----------------------------------------

void Sprite::serialize(std::ostream& os) const {
	Component::serialize(os);


	os.write((char*)&layerIndex, sizeof(decltype(layerIndex)));			// Layer Index
	color.serialize(os);												// Color
}

void Sprite::deserialize(std::istream& is) {
	Component::deserialize(is);

	is.read((char*)&layerIndex, sizeof(decltype(layerIndex)));			// Layer Index
	color.deserialize(is);												// Color
}





/*// For glScissor
void GraphicComponent::clip(glm::vec2 clipVertices[4]) {
	if (clipEnabled) {
		// Recalculate drawRect
		if (drawRect.x < rect.x) {
			drawRect.x = rect.x;
		}
		if (drawRect.y < rect.y) {
			drawRect.y = rect.y;
		}
		if (drawRect.x + drawRect.z > rect.x + rect.z) {
			drawRect.z = rect.x - drawRect.x + rect.z;
		}
		if (drawRect.y + drawRect.w > rect.y + rect.w) {
			drawRect.w = rect.y - drawRect.y + rect.w;
		}
	} else {
		clipEnabled = true;
		drawRect = rect;
	}
}*/