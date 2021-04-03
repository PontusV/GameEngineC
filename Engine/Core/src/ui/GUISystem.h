#ifndef GRAPHICAL_USER_INTERFACE_SYSTEM_H
#define GRAPHICAL_USER_INTERFACE_SYSTEM_H
#include "entity/component/ComponentGroup.h"
#include "components/ui/layout/LayoutGroup.h"
#include "components/ui/layout/ContentSizeFitter.h"
#include "components/ui/UIBehaviour.h"
namespace Core {
	/* Graphical User Interface System */
	class GUISystem {
	public:
		GUISystem();
		~GUISystem();

		void update();
		/* Notifies all UIBehaviours of the window resize. */
		void onWindowResize();

	private:
		ComponentGroup<ContentSizeFitter> contentSizeFitterGroup;
		ComponentGroup<LayoutGroup> layoutGroupComponentGroup;
		ComponentGroup<UIBehaviour> behavioursGroup;
	};
}
#endif