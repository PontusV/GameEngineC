#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include "Script.h"
#include "ComponentGroup.h"
#include "ComponentArray.h"

namespace Core {

	class Engine;

	struct ScriptGroup : public ComponentGroup<Script> {
		ComponentArray<Script>& scripts = getComponentArray<Script>();
	};

	class ScriptManager {
	public:
		ScriptManager(Engine* engine);
		~ScriptManager();

		/* Calls Script::update() on all scripts. */
		void update(float deltaTime);

		ComponentArray<Script>& getAllScripts();

	private:
		ScriptGroup scriptGroup;
	};
}
#endif