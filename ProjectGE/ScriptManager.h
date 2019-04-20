#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include "Script.h"
#include "ComponentGroup.h"
#include "ComponentArray.h"

namespace Core {
	struct ScriptGroup : public ComponentGroup<Script> {
		ComponentArray<Script>& scripts = getComponentArray<Script>();
	};

	class ScriptManager {
	public:
		ScriptManager();
		~ScriptManager();

		/* Calls Script::update() on all scripts. */
		void update(float deltaTime);

	private:
		ScriptGroup scriptGroup;
	};
}
#endif