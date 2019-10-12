#include "Behaviour.h"

using namespace Core;


Input* Behaviour::input;
Window* Behaviour::window;
Camera* Behaviour::camera;

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
	awake();
	if (enabled)
		onEnable();
	active = true;
}

void Behaviour::destroyEntity(const EntityHandle& handle) {
	destroyEntity(handle.getEntity());
}

void Behaviour::destroyEntity(Entity entity) {
	Scene* scene = owner.getComponent<ObjectData>()->getScene();
	scene->destroyEntityQueued(entity);
}