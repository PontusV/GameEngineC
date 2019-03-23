#include "GraphicComponent.h"

using namespace Core;


GraphicComponent::GraphicComponent(unsigned short layerIndex, unsigned int width, unsigned int height) : BoxComponent(width, height), layerIndex(layerIndex) {
} // Constructor

GraphicComponent::~GraphicComponent() {
} // Destructor

void GraphicComponent::setLayerIndex(unsigned short index) {
	layerIndex = index;
}

unsigned short GraphicComponent::getLayerIndex() {
	return layerIndex;
}

const glm::vec4& GraphicComponent::getDrawRect() {
	return drawRect;
}

void GraphicComponent::setDrawRect(glm::vec4 rect) {
	drawRect = rect;
}

void GraphicComponent::enableClipping() {
	clipEnabled = true;
}

void GraphicComponent::disableClipping() {
	clipEnabled = false;
}

bool GraphicComponent::isClipEnabled() {
	return clipEnabled;
}