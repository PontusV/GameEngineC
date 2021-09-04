#include "GUISystem.h"
#include <cstddef>

using namespace Core;

GUISystem::GUISystem(EntityManager* entityManager) : entityManager(entityManager) {
}


GUISystem::~GUISystem() {
}

void updateLayout(LayoutGroup* group, Handle& owner) {
	// Update self
	group->refresh();
	// Update children layout
	std::size_t childCount = owner.getImmediateChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = owner.getChild(i);
		for (LayoutGroup* childGroup : child.getComponents<LayoutGroup>()) {
			updateLayout(childGroup, child);
		}
	}
}

void updateLayoutSizes(LayoutGroup* group, Handle& owner) {
	if (group->isDirtySize()) {
		// Update children layout sizes
		std::size_t childCount = owner.getImmediateChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			Handle child = owner.getChild(i);
			for (LayoutGroup* childGroup : child.getComponents<LayoutGroup>()) {
				updateLayoutSizes(childGroup, child);
			}
		}
		// Update self
		group->updateLayoutSizes();
	}
}

void GUISystem::update() {
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
		Handle groupEntity = layoutGroupComponentGroup.createHandle(i, entityManager);
		updateLayoutSizes(&group, groupEntity);
		if (group.isDirty()) {
			bool add = true;
			for (auto it = dirtyRoots.begin(); it != dirtyRoots.end();) {
				EntityHandle& root = *it;
				if (root.isParent(groupEntity.getEntity()) || root.getEntity() == groupEntity.getEntity()) {
					it = dirtyRoots.erase(it);
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
		for (LayoutGroup* rootGroup : root.getComponents<LayoutGroup>()) {
			updateLayout(rootGroup, root);
		}
	}
}

void GUISystem::onWindowResize() {
	std::size_t behavioursSize = behavioursGroup.size();
	for (std::size_t i = 0; i < behavioursSize; i++) {
		UIBehaviour& behaviour = behavioursGroup.get<UIBehaviour>(i);
		behaviour.onWindowResize();
	}
}