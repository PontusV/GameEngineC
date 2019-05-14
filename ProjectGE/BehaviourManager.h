#ifndef BEHAVIOUR_MANAGER_H
#define BEHAVIOUR_MANAGER_H

#include "Behaviour.h"
#include "ComponentGroup.h"
#include "ComponentArray.h"

namespace Core {

	class Engine;

	struct BehaviourGroup : public ComponentGroup<Behaviour> {
		ComponentArray<Behaviour>& behaviours = getComponentArray<Behaviour>();
	};

	class BehaviourManager {
	public:
		BehaviourManager(Engine* engine);
		~BehaviourManager();

		/* Calls Behaviour::update() on all behaviours. */
		void update(float deltaTime);

		ComponentArray<Behaviour>& getAllScripts();

	private:
		BehaviourGroup behaviourGroup;
	};
}
#endif