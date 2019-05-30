#include "RectSprite.h"

using namespace Core;


RectSprite::RectSprite(Color color, unsigned char layerIndex) : Sprite(layerIndex, color) {
} // Constructor

RectSprite::RectSprite() {
}

RectSprite::~RectSprite() {
} // Destructor

// ------------------------------- Serializable ----------------------------------------

void RectSprite::serialize(std::ostream &os) const {
	Sprite::serialize(os);
}

void RectSprite::deserialize(std::istream &is) {
	Sprite::deserialize(is);
}