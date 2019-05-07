#include "Rect.h"

using namespace Core;


Rect::Rect(int width, int height, Color color, unsigned char layerIndex) : Sprite(layerIndex, width, height, color) {
} // Constructor

Rect::Rect() {
}

Rect::~Rect() {
} // Destructor

// ------------------------------- Serializable ----------------------------------------

void Rect::serialize(std::ostream &os) const {
	Sprite::serialize(os);
}

void Rect::deserialize(std::istream &is) {
	Sprite::deserialize(is);
}