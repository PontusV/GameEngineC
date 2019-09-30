#include "UISystem.h"
#include <cstddef>

using namespace Core;

UISystem::UISystem() {
}


UISystem::~UISystem() {
}

void updateLayout(LayoutGroup* group) {
	if (group->isDirty()) {
		// Update parent layout
		for (LayoutGroup* parentGroup : group->getOwner().getParent().getComponents<LayoutGroup>()) {
			updateLayout(parentGroup);
		}
		// Update self
		group->refresh();
	}
}

void updateLayoutSizes(LayoutGroup* group) {
	if (group->isDirtySize()) {
		// Update children layout sizes
		Handle handle = group->getOwner();
		std::size_t childCount = handle.getImmediateChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			for (LayoutGroup* childGroup : handle.getChild(i).getComponents<LayoutGroup>()) {
				updateLayoutSizes(childGroup);
			}
		}
		// Update self
		group->updateLayoutSizes();
	}
}

void UISystem::update() {
	// Update Content Size Fitters
	std::size_t contentSizeFitterGroupSize = contentSizeFitterGroup.size();
	for (std::size_t i = 0; i < contentSizeFitterGroupSize; i++) {
		ContentSizeFitter& controller = contentSizeFitterGroup.get<ContentSizeFitter>(i);
		if (controller.isDirty())
			controller.refresh();
	}

	// Update Layout Groups
	std::size_t layoutGroupSize = layoutGroupComponentGroup.size();
	for (std::size_t i = 0; i < layoutGroupSize; i++) {
		LayoutGroup& group = layoutGroupComponentGroup.get<LayoutGroup>(i);
		updateLayoutSizes(&group);
		updateLayout(&group);
	}

	// Notify UIBehaviour of PostLayoutUpdate
	std::size_t behavioursSize = behavioursGroup.size();
	for (std::size_t i = 0; i < behavioursSize; i++) {
		UIBehaviour& behaviour = behavioursGroup.get<UIBehaviour>(i);
		behaviour.onPostLayoutUpdate();
	}
}

void UISystem::onWindowResize() {
	std::size_t behavioursSize = behavioursGroup.size();
	for (std::size_t i = 0; i < behavioursSize; i++) {
		UIBehaviour& behaviour = behavioursGroup.get<UIBehaviour>(i);
		behaviour.onWindowResize();
	}
}