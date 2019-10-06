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
	enabled = true;
}

void Behaviour::disable() {
	started = false;
	enabled = false;
}

void Behaviour::destroyEntity(const EntityHandle& handle) {
	destroyEntity(handle.getEntity());
}

void Behaviour::destroyEntity(Entity entity) {
	Scene* scene = owner.getComponent<ObjectData>()->getScene();
	scene->destroyEntityQueued(entity);
}