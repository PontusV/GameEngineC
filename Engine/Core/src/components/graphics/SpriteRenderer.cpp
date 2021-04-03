#include "SpriteRenderer.h"

using namespace Core;

SpriteRenderer::SpriteRenderer() : Renderer() {
} // Constructor

SpriteRenderer::~SpriteRenderer() {
} // Destructor

void SpriteRenderer::setFlipX(bool value) {
	flipX = value;
}

bool SpriteRenderer::getFlipX() {
	return flipX;
}

void SpriteRenderer::setFlipY(bool value) {
	flipY = value;
}

bool SpriteRenderer::getFlipY() {
	return flipY;
}