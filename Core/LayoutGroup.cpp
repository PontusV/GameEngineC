#include "LayoutGroup.h"
#include "RectTransform.h"
#include "LayoutElement.h"

using namespace Core;

LayoutGroup::LayoutGroup() {
}


LayoutGroup::~LayoutGroup() {
}

Vector2 LayoutGroup::getAllocatedSpace(const std::vector<LayoutElementData>& elements) {
	if (RectTransform* rectTransform = owner.getComponent<RectTransform>()) {
		Vector2 allocatedSpace = rectTransform->getSize();
		allocatedSpace.x -= paddingLeft + paddingRight;
		allocatedSpace.y -= paddingTop + paddingBottom;
		return allocatedSpace;
	}
	return Vector2(0, 0);
}

std::vector<LayoutElementData> LayoutGroup::getLayoutElementData(bool shrinkableChildWidth, bool shrinkableChildHeight) {
	std::size_t childCount = owner.getImmediateChildCount();
	std::vector<LayoutElementData> elements;
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = owner.getChild(i);
		Vector2 flexibleSize = LayoutController::getFlexibleSize(child);
		Vector2 prefSize = LayoutController::getPrefSize(child);
		Vector2 minSize;
		if (!shrinkableChildWidth && !shrinkableChildHeight) {
			minSize = prefSize;
		}
		else {
			minSize = LayoutController::getMinSize(child);
			if (!shrinkableChildWidth) minSize.x = prefSize.x;
			if (!shrinkableChildHeight) minSize.y = prefSize.y;
		}
		elements.push_back(LayoutElementData(child, minSize, prefSize, flexibleSize));
	}
	return elements;
}

float LayoutGroup::getTotalMinWidth(const std::vector<LayoutElementData>& elements) {
	float totalMinWidth = elements.size() > 0 ? spacing * (elements.size() - 1) : 0;
	for (const LayoutElementData& element : elements) {
		totalMinWidth += element.minSize.x;
	}
	return totalMinWidth + paddingLeft + paddingRight;
}

float LayoutGroup::getTotalPrefWidth(const std::vector<LayoutElementData>& elements) {
	float totalPrefWidth = elements.size() > 0 ? spacing * (elements.size() - 1) : 0;
	for (const LayoutElementData& element : elements) {
		totalPrefWidth += element.preferredSize.x;
	}
	return totalPrefWidth + paddingLeft + paddingRight;
}

float LayoutGroup::getTotalMinHeight(const std::vector<LayoutElementData>& elements) {
	float totalMinHeight = elements.size() > 0 ? spacing * (elements.size() - 1) : 0;
	for (const LayoutElementData& element : elements) {
		totalMinHeight += element.minSize.y;
	}
	return totalMinHeight + paddingTop + paddingBottom;
}

float LayoutGroup::getTotalPrefHeight(const std::vector<LayoutElementData>& elements) {
	float totalPrefHeight = elements.size() > 0 ? spacing * (elements.size() - 1) : 0;
	for (const LayoutElementData& element : elements) {
		totalPrefHeight += element.preferredSize.y;
	}
	return totalPrefHeight + paddingTop + paddingBottom;
}