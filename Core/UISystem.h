#ifndef USER_INTERFACE_SYSTEM_H
#define USER_INTERFACE_SYSTEM_H
#include "ComponentGroup.h"
#include "LayoutGroup.h"
#include "ContentSizeFitter.h"
#include "UIBehaviour.h"
#include "ParentEntity.h"
namespace Core {
	class UISystem {
	public:
		UISystem();
		~UISystem();

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