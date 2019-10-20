#ifndef BEHAVIOUR_MANAGER_H
#define BEHAVIOUR_MANAGER_H

#include "Behaviour.h"
#include "ComponentGroup.h"
#include "ComponentArray.h"

namespace Core {

	class Engine;

	class BehaviourManager {
	public:
		BehaviourManager(Engine* engine);
		~BehaviourManager();

		/* Calls Behaviour::update() on all behaviours. */
		void update(float deltaTime);
		/* Calls Behavior::onPreRender() on all behaviours */
		void onPreRender();
		/* Calls Behavior::onPostRender() on all behaviours */
		void onPostRender();

		ComponentArray<Behaviour>& getAllScripts();

	private:
		ComponentGroup<Behaviour> behaviourGroup;
	};
}
#endif