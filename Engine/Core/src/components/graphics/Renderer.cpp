#include "Renderer.h"

using namespace Core;


Renderer::Renderer() {
} // Constructor

Renderer::~Renderer() {
} // Destructor


bool Renderer::isEnabled() const {
	return enabled;
}

void Renderer::enable() {
	enabled = true;
}

void Renderer::disable() {
	enabled = false;
}