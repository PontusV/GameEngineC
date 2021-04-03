#include "Graphic.h"
#include "engine/ResourceManager.h"

using namespace Core;

Graphic::Graphic(std::size_t sortingOrder, const char* shaderPath, Color color)
	: sortingOrder(sortingOrder), shaderPath(shaderPath), color(color), shader(ResourceManager::getInstance().loadShader(shaderPath)) {}
Graphic::Graphic(std::size_t sortingOrder, Shader shader, Color color) : sortingOrder(sortingOrder), shader(shader), color(color) {}
Graphic::Graphic() : sortingOrder(0) {}

Graphic::~Graphic() {
}

const Color& Graphic::getColor() const {
	return color;
}

void Graphic::setColor(Color value) {
	color = value;
}

void Graphic::setShader(Shader value) {
	shader = value;
}

const Shader& Graphic::getShader() const {
	return shader;
}

void Graphic::setSortingOrder(std::size_t value) {
	sortingOrder = value;
}

const std::size_t& Graphic::getSortingOrder() const {
	return sortingOrder;
}

void Graphic::reloadShader(bool force) {
	if (!shaderPath.empty() && shader.ID == 0 || force) {
		shader = ResourceManager::getInstance().loadShader(shaderPath.c_str());
	}
}