#include "HorizontalLayoutGroup.h"
#include "RectTransform.h"
#include "Anchor.h"
#include <algorithm>

using namespace Core;

HorizontalLayoutGroup::HorizontalLayoutGroup() {
}


HorizontalLayoutGroup::~HorizontalLayoutGroup() {
}



void HorizontalLayoutGroup::updateLayout() {
	RectTransform* rectTransform = owner.getComponent<RectTransform>();
	if (rectTransform) {
		std::vector<LayoutElementData> elements = getLayoutElementData(shrinkableChildWidth, shrinkableChildHeight);
		Vector2 allocatedSpace = getAllocatedSpace(elements);
		float totalMinWidth = minSize.x - paddingLeft - paddingRight;
		float totalPrefWidth = prefSize.x - paddingLeft - paddingRight;
		float totalPrefWidthDif = totalPrefWidth - totalMinWidth;

		// Calculate width scales
		float allocatedMinWidth = std::min(totalMinWidth, allocatedSpace.x);
		float allocatedPrefWidthDif = std::min(totalPrefWidthDif, allocatedSpace.x - allocatedMinWidth);

		float minWidthScale = totalMinWidth > 0 ? allocatedMinWidth / totalMinWidth : 0;
		float prefWidthScale = totalPrefWidthDif > 0 ? allocatedPrefWidthDif / totalPrefWidthDif : 0;

		if (prefWidthScale > 1.0f) {
			prefWidthScale = 1.0f;
		}
		if (minWidthScale > 1.0f) {
			minWidthScale = 1.0f;
		}

		// Calculate size of each element
		for (LayoutElementData& element : elements) {
			// Calculate height
			float height;
			if (childForceExpandHeight) { // Expand height?
				height = allocatedSpace.y * element.flexibleSize.y;
			}
			else {
				height = element.child.getComponent<RectTransform>()->getSize().y;
			}

			// Calculate width
			float width = element.minSize.x * minWidthScale + (element.preferredSize.x - element.minSize.x) * prefWidthScale;

			if (width < element.minSize.x)
				width = element.minSize.x;
			if (height < element.minSize.y)
				height = element.minSize.y;

			element.size = Vector2(width, height);
			element.preStretchSize = element.size;
		}

		if (childForceExpandWidth) {
			expandWidth(elements, allocatedSpace.x - totalPrefWidth);
		}

		// Position and resize all elements
		float claimedWidth = elements.size() > 0 ? spacing * (elements.size() - 1) : 0; // Total width claimed by all the layout elements
		for (LayoutElementData& element : elements) {
			claimedWidth += element.size.x;
		}

		Vector2 anchor = Anchor(childAlignment);
		float x = paddingLeft + allocatedSpace.x * anchor.x - claimedWidth * anchor.x;
		float y = paddingTop + allocatedSpace.y * anchor.y;

		for (std::size_t i = 0; i < elements.size(); i++) {
			LayoutElementData& element = elements[i];
			RectTransform* transform = element.child.getComponent<RectTransform>();

			// Set size
			if (transform->getSize() != element.size)
				transform->setSize(element.size);

			// Set position
			transform->setLocalX(x - transform->getRectOffset().x + rectTransform->getRectOffset().x);
			transform->setLocalY(y - transform->getRectOffset().y + rectTransform->getRectOffset().y - element.size.y * anchor.y);

			// Calculate position for next element
			x += element.size.x + spacing;
		}
	}
}

void HorizontalLayoutGroup::expandWidth(std::vector<LayoutElementData>& elements, float allocatedExpandWidth) {
	if (allocatedExpandWidth > 0) {
		std::vector<LayoutElementData*> sortedLayoutElements;
		sortedLayoutElements.reserve(elements.size());
		for (LayoutElementData& element : elements) {
			if (element.flexibleSize.x > 0) // Does this element want to be stretched?
				sortedLayoutElements.push_back(&element);
		}
		std::sort(sortedLayoutElements.begin(), sortedLayoutElements.end(), [](LayoutElementData* left, LayoutElementData* right) {
			return left->flexibleSize.x < right->flexibleSize.x;
		});

		bool breakLoop = false;
		float excessWidth = allocatedExpandWidth;
		auto it = sortedLayoutElements.begin();
		while (it != sortedLayoutElements.end()) {
			float width = (*it)->flexibleSize.x * allocatedExpandWidth - ((*it)->size.x - (*it)->preStretchSize.x); // How much width does it want to be added from this expand?
			float maxWidthPerElement = excessWidth / sortedLayoutElements.size();
			if (width >= maxWidthPerElement) { // Makes sure to not exceed expansionWidth
				width = maxWidthPerElement;
				breakLoop = true;
			}
			auto iterator = sortedLayoutElements.begin();
			while (iterator != sortedLayoutElements.end()) {
				LayoutElementData& e = **iterator;
				float widthGoalDif = e.flexibleSize.x * allocatedExpandWidth - (e.size.x - e.preStretchSize.x); // How much more width is required to reach its goal?
				if (width < widthGoalDif) {
					e.size.x += width;
					excessWidth -= width;
					iterator++;
				}
				else if (widthGoalDif > 0) {
					e.size.x += widthGoalDif;
					excessWidth -= widthGoalDif;
					sortedLayoutElements.erase(iterator);
				}
			}
			if (breakLoop)
				break;
		}
	}
}

void HorizontalLayoutGroup::updateLayoutSizes() {
	dirtySize = false;
	std::vector<LayoutElementData> elements = getLayoutElementData();
	// Min size
	float minHeight = paddingTop + paddingBottom;
	for (LayoutElementData& element : elements) {
		minHeight = std::max(element.minSize.y, minHeight);
	}
	minSize = Vector2(getTotalMinWidth(elements), minHeight);
	// Pref size
	float prefHeight = paddingTop + paddingBottom;
	for (LayoutElementData& element : elements) {
		prefHeight = std::max(element.preferredSize.y, prefHeight);
	}
	prefSize = Vector2(getTotalPrefWidth(elements), prefHeight);
	// Flexible size
	float totalFlexibleWidth = 0;
	float flexibleHeight = 0;
	for (LayoutElementData& element : elements) {
		totalFlexibleWidth += element.flexibleSize.x;
		flexibleHeight = std::max(element.flexibleSize.y, flexibleHeight);
	}
	if (totalFlexibleWidth > 1) totalFlexibleWidth = 1;
	if (flexibleHeight > 1) flexibleHeight = 1;
	flexibleSize = Vector2(totalFlexibleWidth, flexibleHeight);
}