#include "BehaviourManager.h"
#include "Engine.h"

using namespace Core;


BehaviourManager::BehaviourManager(Engine* engine) {
	Behaviour::input = &engine->getInput();
	Behaviour::window = &engine->getGraphics().getWindow();
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
}

ComponentArray<Behaviour>& BehaviourManager::getAllScripts() {
	return behaviourGroup.behaviours;
}