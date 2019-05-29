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
		glm::vec2 allocatedSpace = getAllocatedSpace();
		std::vector<LayoutElementData> elements = getLayoutElementData(shrinkableChildWidth, shrinkableChildHeight);
		float totalMinHeight = getTotalMinHeight(elements);
		float totalPrefHeight = getTotalPrefHeight(elements);
		float totalPrefHeightDif = totalPrefHeight - totalMinHeight;

		// Calculate height scales
		float allocatedMinHeight = std::min(totalMinHeight, allocatedSpace.x);
		float allocatedPrefHeightDif = std::min(totalPrefHeightDif, allocatedSpace.x - allocatedMinHeight);

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
			if (childForceExpandWidth) { // Expand height?
				width = allocatedSpace.y * element.flexibleSize.y;
			}
			else {
				width = element.child.getComponent<RectTransform>()->getSize().y;
			}

			// Calculate height
			float height = element.minSize.x * minHeightScale + (element.preferredSize.x - element.minSize.x) * prefHeightScale;

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