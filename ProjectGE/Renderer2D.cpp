#include "Renderer2D.h"
#include "ResourceManager.h"
#include "BatchConfig.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
using namespace Core;

Renderer2D::Renderer2D(Window* window, std::size_t layerAmount)
{
	layers.reserve(layerAmount);
	for (std::size_t i = 0; i < layerAmount; i++) {
		layers.emplace_back(i, window);
	}
	push(glm::mat4(1.0f), true); //Push identity matrix
}


Renderer2D::~Renderer2D()
{
}
void Renderer2D::push(glm::mat4 matrix, bool override) {
	if (override)
		transformationStack.push_back(matrix);
	else
		transformationStack.push_back(transformationStack.back() * matrix);
	transformationStackBack = &transformationStack.back();
}

void Renderer2D::pop() {
	if (transformationStack.size() > 1) {
		transformationStack.pop_back();
		transformationStackBack = &transformationStack.back();
	}
}

void Renderer2D::submit(const Renderable2D& renderable, const unsigned short layerIndex) {
	layers[layerIndex].submit(renderable);
}

/* Draws and clears list of batches. */
void Renderer2D::flush() {
	for (Layer& layer : layers) {
		layer.flush();
	}
}

void Renderer2D::renderText(std::string text, Transform transform, Font font, glm::vec4 color, bool clipEnabled, const glm::vec4 drawRect, const unsigned int layerIndex) {
	layers[layerIndex].renderText(text, transform, font, color, clipEnabled, drawRect);
}