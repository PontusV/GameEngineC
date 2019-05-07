#include "ScriptManager.h"
#include "Engine.h"

using namespace Core;


ScriptManager::ScriptManager(Engine* engine) {
	Script::input = &engine->getInput();
	Script::window = &engine->getGraphics().getWindow();
}


ScriptManager::~ScriptManager() {
}


void ScriptManager::update(float deltaTime) {
	std::size_t scriptAmount = scriptGroup.scripts.size();

	// Run scripts
	for (std::size_t i = 0; i < scriptAmount; i++) {
		if (scriptGroup.scripts[i].enabled) { // Check if enabled
			// Start
			if (!scriptGroup.scripts[i].started) {
				scriptGroup.scripts[i].started = true;
				scriptGroup.scripts[i].start();
			}
			// Update
			scriptGroup.scripts[i].update(deltaTime);
		}
	}
}

ComponentArray<Script>& ScriptManager::getAllScripts() {
	return scriptGroup.scripts;
}