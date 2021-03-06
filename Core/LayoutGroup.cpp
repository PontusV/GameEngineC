#include "LayoutGroup.h"
#include "RectTransform.h"

using namespace Core;

LayoutGroup::LayoutGroup() {
}


LayoutGroup::~LayoutGroup() {
}

glm::vec2 LayoutGroup::getAllocatedSpace() {
	RectTransform* rectTransform = owner.getComponent<RectTransform>();
	if (rectTransform) {
		glm::vec2 allocatedSpace = rectTransform->getSize(); // Allocated space for all LayoutElements
		allocatedSpace.x -= paddingLeft + paddingRight;
		allocatedSpace.y -= paddingTop + paddingBottom;
		return allocatedSpace;
	}
	return glm::vec2(0, 0);
}

std::vector<LayoutElementData> LayoutGroup::getLayoutElementData(bool shrinkableChildWidth, bool shrinkableChildHeight) {
	std::size_t childCount = owner.getImmediateChildCount();
	std::vector<LayoutElementData> elements;
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = owner.getChild(i);
		RectTransform* childRectTransform = child.getComponent<RectTransform>();
		if (childRectTransform) {
			glm::vec2 flexibleSize = LayoutController::getFlexibleSize(child);
			glm::vec2 prefSize = LayoutController::getPrefSize(child);
			glm::vec2 minSize;
			if (!shrinkableChildWidth && !shrinkableChildHeight) {
				minSize = prefSize;
			}
			else {
				minSize = LayoutController::getMinSize(child);
				if (!shrinkableChildWidth) minSize.x = prefSize.x;
				if (!shrinkableChildWidth) minSize.y = prefSize.y;
			}
			elements.push_back(LayoutElementData(child, minSize, prefSize, flexibleSize));
		}
	}
	return elements;
}

float LayoutGroup::getTotalMinWidth(const std::vector<LayoutElementData>& elements) {
	float totalMinWidth = 0;
	for (const LayoutElementData& element : elements) {
		totalMinWidth += element.minSize.x + spacing;
	}
	return totalMinWidth;
}

float LayoutGroup::getTotalPrefWidth(const std::vector<LayoutElementData>& elements) {
	float totalPrefWidth = 0;
	for (const LayoutElementData& element : elements) {
		totalPrefWidth += element.preferredSize.x + spacing;
	}
	return totalPrefWidth;
}

float LayoutGroup::getTotalMinHeight(const std::vector<LayoutElementData>& elements) {
	float totalMinHeight = 0;
	for (const LayoutElementData& element : elements) {
		totalMinHeight += element.minSize.y + spacing;
	}
	return totalMinHeight;
}

float LayoutGroup::getTotalPrefHeight(const std::vector<LayoutElementData>& elements) {
	float totalPrefHeight = 0;
	for (const LayoutElementData& element : elements) {
		totalPrefHeight += element.preferredSize.y + spacing;
	}
	return totalPrefHeight;
}

glm::vec2 LayoutGroup::getMinSize() {
	std::vector<LayoutElementData> elements = getLayoutElementData();
	return glm::vec2(getTotalMinWidth(elements), getTotalMinHeight(elements));
}

glm::vec2 LayoutGroup::getPrefSize() {
	std::vector<LayoutElementData> elements = getLayoutElementData();
	return glm::vec2(paddingLeft + paddingRight + getTotalPrefWidth(elements), paddingTop + paddingBottom + getTotalPrefHeight(elements));
}

glm::vec2 LayoutGroup::getFlexibleSize() {
	float totalFlexibleWidth = 0;
	float totalFlexibleHeight = 0;
	std::vector<LayoutElementData> elements = getLayoutElementData();
	for (LayoutElementData& element : elements) {
		totalFlexibleWidth += element.flexibleSize.x;
		totalFlexibleHeight += element.flexibleSize.y;
	}
	if (totalFlexibleWidth > 1) totalFlexibleWidth = 1;
	if (totalFlexibleHeight > 1) totalFlexibleHeight = 1;
	return glm::vec2(totalFlexibleWidth, totalFlexibleHeight);
}