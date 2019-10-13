#include "ScrollRect.h"
#include "RectTransform.h"
#include "Maths/MatrixTransform.h"
#include <iostream>
using namespace Core;

ScrollRect::ScrollRect() {

}

ScrollRect::~ScrollRect() {

}

void ScrollRect::checkScrollArea() {
	if (RectTransform * rect = owner.getComponent<RectTransform>()) {
		if (scrollableY) {
			if (offset.y < -contentBounds.pos.y - contentBounds.size.y + rect->getSize().y - paddingBottom) {
				offset.y = -contentBounds.pos.y - contentBounds.size.y + rect->getSize().y - paddingBottom;
				changed = true;
			}
			if (offset.y > contentBounds.pos.y + paddingTop) {
				offset.y = contentBounds.pos.y + paddingTop;
				changed = true;
			}
		}
	}
}

void ScrollRect::onScroll(float xOffset, float yOffset) {
	// getContentBounds
	if (scrollableX)
		offset.x += xOffset * speed;
	if (scrollableY)
		offset.y += yOffset * speed;
	changed = true;

	checkScrollArea();
}

void ScrollRect::onPreRender() {
	refreshContentBounds();
	checkScrollArea();
}

void ScrollRect::refreshContentBounds() {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		float top = paddingTop;
		float left = paddingLeft;
		float bottom = rect->getSize().y - paddingBottom;
		float right = rect->getSize().x - paddingRight;
		std::vector<RectTransform*> rectTransforms = owner.getComponentsInImmediateChildren<RectTransform>();
		for (const RectTransform* transform : rectTransforms) {
			for (const Vector2& vertex : transform->getLocalVertices()) {
				if (vertex.x < left)
					left = vertex.x;
				else if (vertex.x > right)
					right = vertex.x;
				if (vertex.y < top)
					top = vertex.y;
				else if (vertex.y > bottom)
					bottom = vertex.y;
			}
		}
		contentBounds = Bounds(Vector2(left, top), Vector2(right - left, bottom - top));
	}
}

bool ScrollRect::hasChanged() {
	if (changed) {
		changed = false;
		return true;
	}
	return false;
}

void ScrollRect::onChildAdded(EntityHandle entity) {
	//refreshContentBounds();
	//checkScrollArea();
}

void ScrollRect::onChildRemoved(EntityHandle entity) {
	//refreshContentBounds();
	//checkScrollArea();
}

const Bounds& ScrollRect::getContentBounds() const {
	return contentBounds;
}