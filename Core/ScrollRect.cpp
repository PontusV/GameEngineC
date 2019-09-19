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
	offset.x += xOffset * speed;
	offset.y += yOffset * speed;
	changed = true;

	checkScrollArea();
}

void ScrollRect::lateUpdate(float deltaTime) {
	refreshContentBounds();
	checkScrollArea();
	changed = true;
	/*for (RectTransform* rect : owner.getComponentsInImmediateChildren<RectTransform>()) {
		if (rect->hasChanged()) {
			refreshContentBounds();
			checkScrollArea();
		}
	}*/
}

void ScrollRect::refreshContentBounds() {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		float top = 0.0f;
		float left = 0.0f;
		float bottom = rect->getSize().y;
		float right = rect->getSize().x;
		std::vector<RectTransform*> rectTransforms = owner.getComponentsInImmediateChildren<RectTransform>();
		for (const RectTransform* transform : rectTransforms) {
			for (Vector2 vertex : transform->getLocalVertices()) {
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

void ScrollRect::onChildAdded(Handle entity) {
	refreshContentBounds();
	checkScrollArea();
}

void ScrollRect::onChildRemoved(Handle entity) {
	refreshContentBounds();
	checkScrollArea();
}

const Bounds& ScrollRect::getContentBounds() const {
	return contentBounds;
}