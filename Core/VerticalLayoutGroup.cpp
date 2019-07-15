#include "VerticalLayoutGroup.h"
#include "RectTransform.h"
#include "Anchor.h"
#include <algorithm>

using namespace Core;

VerticalLayoutGroup::VerticalLayoutGroup() {
}


VerticalLayoutGroup::~VerticalLayoutGroup() {
}


void VerticalLayoutGroup::updateLayout() {
	RectTransform* rectTransform = owner.getComponent<RectTransform>();
	if (rectTransform) {
		std::vector<LayoutElementData> elements = getLayoutElementData(shrinkableChildWidth, shrinkableChildHeight);
		glm::vec2 allocatedSpace = getAllocatedSpace(elements);
		float totalMinHeight = getTotalMinHeight(elements);
		float totalPrefHeight = getTotalPrefHeight(elements);
		float totalPrefHeightDif = totalPrefHeight - totalMinHeight;

		// Calculate height scales
		float allocatedMinHeight = std::min(totalMinHeight, allocatedSpace.y);
		float allocatedPrefHeightDif = std::min(totalPrefHeightDif, allocatedSpace.y - allocatedMinHeight);

		float minHeightScale = totalMinHeight > 0 ? allocatedMinHeight / totalMinHeight : 0;
		float prefHeightScale = totalPrefHeightDif > 0 ? allocatedPrefHeightDif / totalPrefHeightDif : 0;

		if (prefHeightScale > 1.0f) {
			prefHeightScale = 1.0f;
		}
		if (minHeightScale > 1.0f) {
			minHeightScale = 1.0f;
		}

		// Calculate size of each element
		for (LayoutElementData& element : elements) {
			// Calculate width
			float width;
			if (childForceExpandWidth) { // Expand width?
				width = allocatedSpace.x * element.flexibleSize.x;
			}
			else {
				width = element.child.getComponent<RectTransform>()->getSize().x;
			}

			// Calculate height
			float height = element.minSize.y * minHeightScale + (element.preferredSize.y - element.minSize.y) * prefHeightScale;

			if (width < element.minSize.x)
				width = element.minSize.x;
			if (height < element.minSize.y)
				height = element.minSize.y;

			element.size = glm::vec2(width, height);
			element.preStretchSize = element.size;
		}

		if (childForceExpandHeight) {
			expandHeight(elements, allocatedSpace.y - totalPrefHeight);
		}

		// Position and resize all elements
		float claimedHeight = 0; // Total width claimed by all the layout elements
		for (LayoutElementData& element : elements) {
			claimedHeight += element.size.y + spacing;
		}

		glm::vec2 anchor = Anchor(childAlignment);
		float x = paddingLeft + allocatedSpace.x * anchor.x;
		float y = paddingTop + allocatedSpace.y * anchor.y - claimedHeight * anchor.y;

		for (std::size_t i = 0; i < elements.size(); i++) {
			LayoutElementData& element = elements[i];
			RectTransform* transform = element.child.getComponent<RectTransform>();

			// Set size
			transform->setSize(element.size);

			// Set position
			transform->setLocalX(x - transform->getRectOffset().x - rectTransform->getRectOffset().x - element.size.x * anchor.x);
			transform->setLocalY(y - transform->getRectOffset().y - rectTransform->getRectOffset().y);

			// Calculate position for next element
			y += element.size.y + spacing;
		}
	}
}

void VerticalLayoutGroup::expandHeight(std::vector<LayoutElementData>& elements, float allocatedExpandHeight) {
	if (allocatedExpandHeight > 0) {
		std::vector<LayoutElementData*> sortedLayoutElements;
		sortedLayoutElements.reserve(elements.size());
		for (LayoutElementData& element : elements) {
			if (element.flexibleSize.y > 0) // Does this element want to be stretched?
				sortedLayoutElements.push_back(&element);
		}
		std::sort(sortedLayoutElements.begin(), sortedLayoutElements.end(), [](LayoutElementData* left, LayoutElementData* right) {
			return left->flexibleSize.y < right->flexibleSize.y;
		});

		bool breakLoop = false;
		float excessHeight = allocatedExpandHeight;
		auto it = sortedLayoutElements.begin();
		while (it != sortedLayoutElements.end()) {
			float height = (*it)->flexibleSize.y * allocatedExpandHeight - ((*it)->size.y - (*it)->preStretchSize.y); // How much height does it want to be added from this expand?
			float maxHeightPerElement = excessHeight / sortedLayoutElements.size();
			if (height >= maxHeightPerElement) { // Makes sure to not exceed expansionHeight
				height = maxHeightPerElement;
				breakLoop = true;
			}
			auto iterator = sortedLayoutElements.begin();
			while (iterator != sortedLayoutElements.end()) {
				LayoutElementData& e = **iterator;
				float heightGoalDif = e.flexibleSize.y * allocatedExpandHeight - (e.size.y - e.preStretchSize.y); // How much more height is required to reach its goal?
				if (height < heightGoalDif) {
					e.size.y += height;
					excessHeight -= height;
					iterator++;
				}
				else if (heightGoalDif > 0) {
					e.size.y += heightGoalDif;
					excessHeight -= heightGoalDif;
					sortedLayoutElements.erase(iterator);
				}
			}
			if (breakLoop)
				break;
		}
	}
}

glm::vec2 VerticalLayoutGroup::getMinSize() {
	std::vector<LayoutElementData> elements = getLayoutElementData();
	float minWidth = 0;
	for (LayoutElementData& element : elements) {
		minWidth = std::max(element.minSize.x, minWidth);
	}
	return glm::vec2(minWidth, getTotalMinHeight(elements));
}

glm::vec2 VerticalLayoutGroup::getPrefSize() {
	std::vector<LayoutElementData> elements = getLayoutElementData();
	float prefWidth = 0;
	for (LayoutElementData& element : elements) {
		prefWidth = std::max(element.preferredSize.x, prefWidth);
	}
	return glm::vec2(paddingLeft + paddingRight + prefWidth, paddingTop + paddingBottom + getTotalPrefHeight(elements));
}

glm::vec2 VerticalLayoutGroup::getFlexibleSize() {
	float flexibleWidth = 1;
	float totalFlexibleHeight = 0;
	std::vector<LayoutElementData> elements = getLayoutElementData();
	for (LayoutElementData& element : elements) {
		flexibleWidth = std::max(element.flexibleSize.x, flexibleWidth);
		totalFlexibleHeight += element.flexibleSize.y;
	}
	if (flexibleWidth > 1) flexibleWidth = 1;
	if (totalFlexibleHeight > 1) totalFlexibleHeight = 1;
	return glm::vec2(flexibleWidth, totalFlexibleHeight);
}