#include "Sprite.h"
#include "ResourceManager.h"

using namespace Core;


Sprite::Sprite(Color color) : color(color) {
} // Constructor

Sprite::~Sprite() {
} // Destructor


const Color& Sprite::getColor() const {
	return color;
}

void Sprite::setColor(Color value) {
	color = value;
}

const std::vector<RectTransform>& Sprite::getMasks() const {
	return masks;
}

void Sprite::clip(RectTransform mask) {
	clipEnabled = true;
	masks.push_back(mask);
}

void Sprite::resetClipping() {
	clipEnabled = false;
	masks.clear();
}

bool Sprite::isClipEnabled() const {
	return clipEnabled;
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