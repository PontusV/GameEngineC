#include "Mask.h"

using namespace Core;

Mask::Mask() {}
Mask::~Mask() {}

bool Mask::isEnabled() {
	return enabled;
}

void Mask::enable() {
	enabled = true;
}

void Mask::disable() {
	enabled = false;
}