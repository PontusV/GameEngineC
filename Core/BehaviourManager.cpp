#include "BehaviourManager.h"
#include "Engine.h"

using namespace Core;


BehaviourManager::BehaviourManager(Engine* engine) {
	Behaviour::input = &engine->getInput();
	Behaviour::window = &engine->getGraphics().getWindow();
	Behaviour::camera = &engine->getGraphics().getCamera();
}


BehaviourManager::~BehaviourManager() {
}


void BehaviourManager::update(float deltaTime) {
	std::size_t scriptAmount = behaviourGroup.size();

	// Run scripts
	for (std::size_t i = 0; i < scriptAmount; i++) {
		Behaviour& behaviour = behaviourGroup.get<Behaviour>(i);
		if (behaviour.enabled) { // Check if enabled
			// Start
			if (!behaviour.started) {
				behaviour.started = true;
				behaviour.start();
			}
			// Update
			behaviour.update(deltaTime);
		}
	}
	// LateUpdate
	for (std::size_t i = 0; i < scriptAmount; i++) {
		Behaviour& behaviour = behaviourGroup.get<Behaviour>(i);
		if (behaviour.enabled && behaviour.started) { // Check if enabled
			behaviour.lateUpdate(deltaTime);
		}
	}
}

void BehaviourManager::onPreRender() {
	std::size_t scriptAmount = behaviourGroup.size();
	// Run scripts
	for (std::size_t i = 0; i < scriptAmount; i++) {
		Behaviour& behaviour = behaviourGroup.get<Behaviour>(i);
		if (behaviour.enabled && behaviour.started) { // Check if enabled
			behaviour.onPreRender();
		}
	}
}

void BehaviourManager::onPostRender() {
	std::size_t scriptAmount = behaviourGroup.size();
	// Run scripts
	for (std::size_t i = 0; i < scriptAmount; i++) {
		Behaviour& behaviour = behaviourGroup.get<Behaviour>(i);
		if (behaviour.enabled && behaviour.started) { // Check if enabled
			behaviour.onPostRender();
		}
	}
}

ComponentArray<Behaviour>& BehaviourManager::getAllScripts() {
	return behaviourGroup.getArray<Behaviour>();
}