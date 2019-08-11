#include "LayoutController.h"
#include "ILayoutElement.h"
#include "LayoutElement.h"
#include "RectTransform.h"
#include "LayoutGroup.h"
#include <algorithm>

using namespace Core;

LayoutController::LayoutController() {
}


LayoutController::~LayoutController() {
}


void LayoutController::onRectTransformResized() {
	setDirty();
}

void LayoutController::onChildRectTransformResized() {
	setDirty();
}

void LayoutController::onChildAdded(Handle entity) {
	setDirty();
}

void LayoutController::onChildRemoved(Handle entity) {
	setDirty();
}

void LayoutController::refresh() {
	updateLayout();
	dirty = false;
}

void LayoutController::setDirty() {
	dirty = true;
}

bool LayoutController::isDirty() {
	return dirty;
}

Vector2 LayoutController::getMinSize(Handle entity) {
	// Check element first
	if (LayoutElement* element = entity.getComponent<LayoutElement>())
		if (element->getMinSizeEnabled())
			return element->getMinSize();

	std::vector<ILayoutElement*> layoutElements = getLayoutGroups(entity);
	Vector2 minSize(0,0); // Default value if no ILayoutElement components are found
	if (!layoutElements.empty()) {
		// Init
		minSize = layoutElements[0]->getMinSize();
		// Find max values from the rest of the ILayoutElements
		for (std::size_t i = 1; i < layoutElements.size(); i++) {
			Vector2 currentMinSize = layoutElements[i]->getMinSize();
			minSize.x = std::max(minSize.x, currentMinSize.x);
			minSize.y = std::max(minSize.y, currentMinSize.y);
		}
	}
	else {
		// Default value for children without any ILayoutElement components
		if (RectTransform* childRectTransform = entity.getComponent<RectTransform>())
			minSize = childRectTransform->getSize();
	}
	return minSize;
}

Vector2 LayoutController::getPrefSize(Handle entity) {
	// Check element first
	if (LayoutElement* element = entity.getComponent<LayoutElement>())
		if (element->getPrefSizeEnabled())
			return element->getPrefSize();

	std::vector<ILayoutElement*> layoutElements = getLayoutGroups(entity);
	Vector2 prefSize(0,0);
	if (!layoutElements.empty()) {
		// Init
		prefSize = layoutElements[0]->getPrefSize();
		// Find max values from the rest of the ILayoutElements
		for (std::size_t i = 1; i < layoutElements.size(); i++) {
			Vector2 currentPrefSize = layoutElements[i]->getPrefSize();
			prefSize.x = std::max(prefSize.x, currentPrefSize.x);
			prefSize.y = std::max(prefSize.y, currentPrefSize.y);
		}
	}
	else {
		// Default value for children without any ILayoutElement components
		if (RectTransform* childRectTransform = entity.getComponent<RectTransform>())
			prefSize = childRectTransform->getSize();
	}
	return prefSize;
}

Vector2 LayoutController::getFlexibleSize(Handle entity) {
	// Check element first
	if (LayoutElement* element = entity.getComponent<LayoutElement>())
		if (element->getFlexibleSizeEnabled())
			return element->getFlexibleSize();

	std::vector<ILayoutElement*> layoutElements = getLayoutGroups(entity);
	Vector2 flexibleSize(1,1); // Default value if no ILayoutElement components are found
	if (!layoutElements.empty()) {
		// Init
		flexibleSize = layoutElements[0]->getFlexibleSize();
		// Find max values from the rest of the ILayoutElements
		for (std::size_t i = 1; i < layoutElements.size(); i++) {
			Vector2 currentFlexibleSize = layoutElements[i]->getFlexibleSize();
			flexibleSize.x = std::max(flexibleSize.x, currentFlexibleSize.x);
			flexibleSize.y = std::max(flexibleSize.y, currentFlexibleSize.y);
		}
	}
	return flexibleSize;
}

std::vector<ILayoutElement*> LayoutController::getLayoutGroups(Handle entity) {
	std::vector<ILayoutElement*> layoutElements;
	std::vector<LayoutGroup*> groups = entity.getComponents<LayoutGroup>();
	layoutElements.reserve(groups.size());
	for (LayoutGroup* group : groups) {
		layoutElements.push_back(static_cast<ILayoutElement*>(group));
	}
	return layoutElements;
}