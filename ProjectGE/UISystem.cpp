#include "UISystem.h"
#include <cstddef>

using namespace Core;

UISystem::UISystem() {
}


UISystem::~UISystem() {
}


void UISystem::update() {
	// Update Content Size Fitters
	std::size_t contentSizeFitterGroupSize = contentSizeFitterGroup.contentSizeFitters.size();
	for (std::size_t i = 0; i < contentSizeFitterGroupSize; i++) {
		ContentSizeFitter& controller = contentSizeFitterGroup.contentSizeFitters[i];
		if (controller.isDirty())
			controller.refresh();
	}

	// Update Layout Controllers
	std::size_t layoutGroupSize = layoutGroupComponentGroup.layoutGroups.size();
	for (std::size_t i = 0; i < layoutGroupSize; i++) {
		LayoutController& controller = layoutGroupComponentGroup.layoutGroups[i];
		if (controller.isDirty())
			controller.refresh();
	}
}

void UISystem::onWindowResize() {
	std::size_t behavioursSize = behavioursGroup.behaviours.size();
	for (std::size_t i = 0; i < behavioursSize; i++) {
		UIBehaviour& behaviour = behavioursGroup.behaviours[i];
		behaviour.onWindowResize();
	}
}