#include "UISystem.h"
#include <cstddef>

using namespace Core;

UISystem::UISystem() {
}


UISystem::~UISystem() {
}

void updateLayout(LayoutGroup* group) {
	// Update self
	group->refresh();
	// Update children layout
	Handle handle = group->getOwner();
	std::size_t childCount = handle.getImmediateChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		for (LayoutGroup* childGroup : handle.getChild(i).getComponents<LayoutGroup>()) {
			updateLayout(childGroup);
		}
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
	std::vector<EntityHandle> dirtyRoots;
	std::size_t layoutGroupSize = layoutGroupComponentGroup.size();
	for (std::size_t i = 0; i < layoutGroupSize; i++) {
		LayoutGroup& group = layoutGroupComponentGroup.get<LayoutGroup>(i);
		updateLayoutSizes(&group);
		if (group.isDirty()) {
			Handle groupEntity = group.getOwner();
			bool add = true;
			for (auto it = dirtyRoots.begin(); it != dirtyRoots.end();) {
				EntityHandle& root = *it;
				if (root.isParent(groupEntity.getEntity())) {
					dirtyRoots.erase(it);
					continue;
				}
				if (groupEntity.isParent(root.getEntity())) {
					add = false;
				}
				it++;
			}
			if (add)
				dirtyRoots.push_back(groupEntity);
		}
	}
	for (EntityHandle& root : dirtyRoots) {
		for (LayoutGroup* rootGroup : root.getComponents<LayoutGroup>())
		updateLayout(rootGroup);
	}
}

void UISystem::onWindowResize() {
	std::size_t behavioursSize = behavioursGroup.size();
	for (std::size_t i = 0; i < behavioursSize; i++) {
		UIBehaviour& behaviour = behavioursGroup.get<UIBehaviour>(i);
		behaviour.onWindowResize();
	}
}