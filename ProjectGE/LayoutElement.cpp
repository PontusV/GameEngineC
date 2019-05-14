#include "LayoutElement.h"
#include "BoxComponent.h"
#include <algorithm>

using namespace Core;


LayoutElement::LayoutElement() {
}


LayoutElement::~LayoutElement() {
}


glm::vec2 LayoutElement::getMinSize() {
	if (minSizeEnabled)
		return minSize;
	return glm::vec2(0, 0);
}

glm::vec2 LayoutElement::getPrefSize() {
	return glm::vec2(std::max(minSize.x, prefSize.x), std::max(minSize.y, prefSize.y));
}

glm::vec2 LayoutElement::getFlexibleSize() {
	if (flexibleSizeEnabled)
		return flexibleSize;
	return glm::vec2(1, 1);
}

void LayoutElement::setMinSize(glm::vec2 size) {
	minSize = size;
}
void LayoutElement::setPrefSize(glm::vec2 size) {
	prefSize = size;
}
void LayoutElement::setFlexibleSize(glm::vec2 size) {
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