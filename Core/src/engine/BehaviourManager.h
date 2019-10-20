#ifndef BEHAVIOUR_MANAGER_H
#define BEHAVIOUR_MANAGER_H

#include "components/Behaviour.h"
#include "entity/component/ComponentGroup.h"
#include "entity/component/ComponentArray.h"

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