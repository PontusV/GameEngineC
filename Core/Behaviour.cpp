#include "Behaviour.h"

using namespace Core;


Input* Behaviour::input;
Window* Behaviour::window;

Behaviour::Behaviour() : started(false), enabled(true) {
}

Behaviour::~Behaviour() {
}

void Behaviour::enable() {
	enabled = true;
}

void Behaviour::disable() {
	started = false;
	enabled = false;
}