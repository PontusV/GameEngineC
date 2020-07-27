#include "SpriteMask.h"

using namespace Core;


SpriteMask::SpriteMask() {
}

SpriteMask::~SpriteMask() {
}


void SpriteMask::setTexture(Texture2D value) {
	texture = value;
}

const Texture2D& SpriteMask::getTexture() const {
	return texture;
}