#ifndef USER_INTERFACE_SYSTEM_H
#define USER_INTERFACE_SYSTEM_H
#include "ComponentGroup.h"
#include "LayoutGroup.h"
#include "ContentSizeFitter.h"
#include "UIBehaviour.h"
#include "ParentEntity.h"
namespace Core {
	struct ContentSizeFitterGroup : public ComponentGroup<ContentSizeFitter> {
		ComponentArray<ContentSizeFitter>& contentSizeFitters = getComponentArray<ContentSizeFitter>();
	};
	struct LayoutGroupComponentGroup : public ComponentGroup<LayoutGroup> {
		//LayoutGroupComponentGroup() : ComponentGroup<LayoutGroup>({ typeof(ParentEntity) }) {}
		ComponentArray<LayoutGroup>& layoutGroups = getComponentArray<LayoutGroup>();
	};
	struct UIBehaviourGroup : public ComponentGroup<UIBehaviour> {
		ComponentArray<UIBehaviour>& behaviours = getComponentArray<UIBehaviour>();
	};

	class UISystem {
	public:
		UISystem();
		~UISystem();

		void update();
		/* Notifies all UIBehaviours of the window resize. */
		void onWindowResize();

	private:
		ContentSizeFitterGroup contentSizeFitterGroup;
		LayoutGroupComponentGroup layoutGroupComponentGroup;
		UIBehaviourGroup behavioursGroup;
	};
}
#endif