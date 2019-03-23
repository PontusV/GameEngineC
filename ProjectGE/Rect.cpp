#include "Rect.h"
#include "Chunk.h"

using namespace Core;

Rect::Rect(int width, int height, glm::vec4 color, unsigned short layerIndex) : GraphicComponent(layerIndex, width, height), color(color) {
} // Constructor


Rect::~Rect() {
} // Destructor

const glm::vec4& Rect::getColor() const {
	return color;
}

void Rect::serialize(std::ostream &os) const {

}

void Rect::deserialize(std::istream &is) {

}