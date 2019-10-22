#include "LayoutController.h"
#include "ILayoutElement.h"
#include "LayoutElement.h"
#include "LayoutGroup.h"
#include "components/RectTransform.h"
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

void LayoutController::onChildAdded(EntityHandle entity) {
	setDirty();
}

void LayoutController::onChildRemoved(EntityHandle entity) {
	setDirty();
}

void LayoutController::onChildChanged(EntityHandle entity) {
	setDirty();
}

void LayoutController::refresh() {
	updateLayout();
	dirty = false;
}

void LayoutController::setDirty() {
	for (LayoutController* controller : owner.getParent().getComponents<LayoutController>()) {
		controller->setDirty();
	}
	dirty = true;
	dirtySize = true;
}

bool LayoutController::isDirty() {
	return dirty;
}

bool LayoutController::isDirtySize() {
	return dirtySize;
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
	return minSize;
}

Vector2 LayoutController::getPrefSize(Handle entity) {
	// Check element first
	if (LayoutElement* element = entity.getComponent<LayoutElement>())
		if (element->getPrefSizeEnabled())
			return element->getPrefSize();
		else if (element->getMinSizeEnabled())
			return element->getMinSize();

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

Vector2 LayoutController::getMinSize() {
	return minSize;
}

Vector2 LayoutController::getPrefSize() {
	return prefSize;
}

Vector2 LayoutController::getFlexibleSize() {
	return flexibleSize;
}