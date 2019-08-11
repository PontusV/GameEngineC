#include "LayoutElement.h"
#include "BoxComponent.h"
#include <algorithm>

using namespace Core;


LayoutElement::LayoutElement() {
}


LayoutElement::~LayoutElement() {
}


Vector2 LayoutElement::getMinSize() {
	if (minSizeEnabled)
		return minSize;
	return Vector2(0, 0);
}

Vector2 LayoutElement::getPrefSize() {
	return Vector2(std::max(minSize.x, prefSize.x), std::max(minSize.y, prefSize.y));
}

Vector2 LayoutElement::getFlexibleSize() {
	if (flexibleSizeEnabled)
		return flexibleSize;
	return Vector2(1, 1);
}

void LayoutElement::setMinSize(Vector2 size) {
	minSize = size;
}
void LayoutElement::setPrefSize(Vector2 size) {
	prefSize = size;
}
void LayoutElement::setFlexibleSize(Vector2 size) {
	flexibleSize = size;
}

void LayoutElement::setMinSizeEnabled(bool value) {
	minSizeEnabled = value;
}
bool LayoutElement::getMinSizeEnabled() {
	return minSizeEnabled;
}
void LayoutElement::setPrefSizeEnabled(bool value) {
	prefSizeEnabled = value;
}
bool LayoutElement::getPrefSizeEnabled() {
	return prefSizeEnabled;
}
void LayoutElement::setFlexibleSizeEnabled(bool value) {
	flexibleSizeEnabled = value;
}
bool LayoutElement::getFlexibleSizeEnabled() {
	return flexibleSizeEnabled;
}