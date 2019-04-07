#include "Renderer2D.h"
#include "ResourceManager.h"
#include "BatchConfig.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
using namespace Core;

Renderer2D::Renderer2D(Window* window) : window(window) {
}

Renderer2D::~Renderer2D() {
	for (Layer* layer : layers) {
		delete layer;
	}
}

unsigned short Renderer2D::createLayer() {
	unsigned short layerIndex = (unsigned short)layers.size();
	layers.push_back(new Layer(layerIndex, window));
	return layerIndex;
}

void Renderer2D::submit(const Renderable2D& renderable, const unsigned short layerIndex) {
	layers[layerIndex]->submit(renderable);
}

/* Draws and clears list of batches. */
void Renderer2D::flush() {
	for (Layer* layer : layers) {
		layer->flush();
	}
}

void Renderer2D::renderText(std::string text, Transform transform, Font font, glm::vec4 color, bool clipEnabled, const std::vector<glm::vec2>& clipMaskVertices, const unsigned int layerIndex) {
	layers[layerIndex]->renderText(text, transform, font, color, clipEnabled, clipMaskVertices);
}