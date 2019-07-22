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

const std::vector<std::array<glm::vec2, 4>>& Sprite::getMasks() const {
	return masks;
}

std::size_t Sprite::clip(const std::array<glm::vec2, 4> & mask) {
	masks.push_back(mask);
	return masks.size() - 1;
}

void Sprite::reclip(std::size_t index, const std::array<glm::vec2, 4> & newMask) {
	masks[index] = newMask;
}

void Sprite::removeClip(std::size_t index) {
	masks.erase(masks.begin() + index);
}

void Sprite::resetClipping() {
	masks.clear();
}

bool Sprite::isClipEnabled() const {
	return masks.size() > 0;
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