#include "ScriptManager.h"

using namespace Core;


ScriptManager::ScriptManager() {
}


ScriptManager::~ScriptManager() {
}


void ScriptManager::update(float deltaTime) {
	std::size_t scriptAmount = scriptGroup.scripts.size();

	// Start
	for (std::size_t i = 0; i < scriptAmount; i++) {
		if (scriptGroup.scripts[i].enabled && !scriptGroup.scripts[i].active) {
			scriptGroup.scripts[i].active = true;
			scriptGroup.scripts[i].start();
		}
	}
	// Update
	for (std::size_t i = 0; i < scriptAmount; i++) {
		scriptGroup.scripts[i].update(deltaTime);
	}
}