#include "Behaviour.h"

using namespace Core;


Input* Behaviour::input;
Window* Behaviour::window;
Camera* Behaviour::camera;
PrefabManager* Behaviour::prefabManager;
EntityManager* Behaviour::entityManager;

Behaviour::Behaviour() : started(false), enabled(true) {
}

Behaviour::~Behaviour() {
}

void Behaviour::enable() {
	if (!enabled && active) onEnable();
	enabled = true;
}

void Behaviour::disable() {
	if (enabled && active) onDisable();
	enabled = false;
}

bool Behaviour::isEnabled() {
	return enabled;
}

bool Behaviour::isActive() {
	return active;
}

void Behaviour::activate() {
	initialize();
	if (enabled)
		onEnable();
	active = true;
}