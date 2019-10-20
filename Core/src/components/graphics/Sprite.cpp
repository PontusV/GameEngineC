#include "Sprite.h"
#include "entity/handle/EntityHandle.h"
#include <stdexcept>

using namespace Core;


Sprite::Sprite(Color color, std::size_t sortingLayer) : color(color), sortingLayer(sortingLayer) {
} // Constructor

Sprite::~Sprite() {
} // Destructor


const Color& Sprite::getColor() const {
	return color;
}

void Sprite::setColor(Color value) {
	color = value;
}

void Sprite::setSortingLayer(std::size_t sortingLayer) {
	this->sortingLayer = sortingLayer;
}

const std::size_t& Sprite::getSortingLayer() const {
	return sortingLayer;
}

const std::vector<std::array<Vector2, 4>>& Sprite::getMasks() const {
	return masks;
}

std::size_t Sprite::getMaskSenderIndex(ComponentID sender) {
	for (std::size_t i = 0; i < maskSenders.size(); i++) {
		if (maskSenders[i] == sender)
			return i;
	}
	std::cout << "Sprite::getMaskSenderIndex::ERROR the sender has not sent anything to this sprite" << std::endl;
	throw std::invalid_argument("Sprite::getMaskSenderIndex::ERROR the sender has not sent anything to this sprite");
}
bool Sprite::hasMaskFromSender(ComponentID sender) {
	for (const ComponentID& maskSender : maskSenders) {
		if (maskSender == sender) {
			return true;
		}
	}
	return false;
}

void Sprite::clip(ComponentID sender, const std::array<Vector2, 4> & mask) {
	// Check for existing mask
	if (hasMaskFromSender(sender)) {
		EntityHandle ownerHandle = owner;
		std::cout << "Sprite::clip::ERROR the sender(" << sender << ") has already sent a mask to this sprite(" << ownerHandle.getEntityName() << ")" << std::endl;
		throw std::invalid_argument("Sprite::clip::ERROR the sender has already sent a mask to this sprite");
	}
	masks.push_back(mask);
	maskSenders.push_back(sender);
}

void Sprite::reclip(ComponentID sender, const std::array<Vector2, 4> & newMask) {
	std::size_t index = getMaskSenderIndex(sender);
	masks[index] = newMask;
}

void Sprite::removeClip(ComponentID sender) {
	std::size_t index = getMaskSenderIndex(sender);
	masks.erase(masks.begin() + index);
	maskSenders.erase(maskSenders.begin() + index);
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