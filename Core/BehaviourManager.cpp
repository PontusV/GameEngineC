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
	std::size_t scriptAmount = behaviourGroup.behaviours.size();

	// Run scripts
	for (std::size_t i = 0; i < scriptAmount; i++) {
		if (behaviourGroup.behaviours[i].enabled) { // Check if enabled
			// Start
			if (!behaviourGroup.behaviours[i].started) {
				behaviourGroup.behaviours[i].started = true;
				behaviourGroup.behaviours[i].start();
			}
			// Update
			behaviourGroup.behaviours[i].update(deltaTime);
		}
	}
	// LateUpdate
	for (std::size_t i = 0; i < scriptAmount; i++) {
		if (behaviourGroup.behaviours[i].enabled) { // Check if enabled
			behaviourGroup.behaviours[i].lateUpdate(deltaTime);
		}
	}
}

void BehaviourManager::onPreRender() {
	std::size_t scriptAmount = behaviourGroup.behaviours.size();
	// Run scripts
	for (std::size_t i = 0; i < scriptAmount; i++) {
		if (behaviourGroup.behaviours[i].enabled) { // Check if enabled
			behaviourGroup.behaviours[i].onPreRender();
		}
	}
}

void BehaviourManager::onPostRender() {
	std::size_t scriptAmount = behaviourGroup.behaviours.size();
	// Run scripts
	for (std::size_t i = 0; i < scriptAmount; i++) {
		if (behaviourGroup.behaviours[i].enabled) { // Check if enabled
			behaviourGroup.behaviours[i].onPostRender();
		}
	}
}

ComponentArray<Behaviour>& BehaviourManager::getAllScripts() {
	return behaviourGroup.behaviours;
}