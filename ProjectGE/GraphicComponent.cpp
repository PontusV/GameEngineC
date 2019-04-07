#include "GraphicComponent.h"

using namespace Core;


GraphicComponent::GraphicComponent(unsigned short layerIndex, unsigned int width, unsigned int height) : BoxComponent(width, height), layerIndex(layerIndex) {
} // Constructor

GraphicComponent::~GraphicComponent() {
} // Destructor

void GraphicComponent::setLayerIndex(unsigned short index) {
	layerIndex = index;
}

unsigned short GraphicComponent::getLayerIndex() const {
	return layerIndex;
}

const std::vector<glm::vec2>& GraphicComponent::getClipMaskVertices() const {
	return clipMaskVertices;
}

void GraphicComponent::clip(glm::vec2 clipVertices[4]) {
	clipEnabled = true;
	// Add mask to list
	for (int i = 0; i < 4; i++) {
		clipMaskVertices.push_back(clipVertices[i]);
	}
}

void GraphicComponent::resetClipping() {
	clipEnabled = false;
	clipMaskVertices.clear();
}

bool GraphicComponent::isClipEnabled() const {
	return clipEnabled;
}

// ------------------------------- Serializable ----------------------------------------

void GraphicComponent::serialize(std::ostream& os) const {
	BoxComponent::serialize(os);


	os.write((char*)&layerIndex, sizeof(decltype(layerIndex)));			// Layer Index
}

void GraphicComponent::deserialize(std::istream& is) {
	BoxComponent::deserialize(is);

	is.read((char*)&layerIndex, sizeof(decltype(layerIndex)));			// Layer Index
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