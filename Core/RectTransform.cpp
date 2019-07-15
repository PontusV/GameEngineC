#include "RectTransform.h"
#include "UIBehaviour.h"

using namespace Core;

RectTransform::RectTransform(float x, float y, int width, int height, float z, Anchor pivot, float rotation, float scale) : Transform(x, y, z, rotation, scale), size(width, height), pivot(pivot) {
}

RectTransform::RectTransform() {
}


RectTransform::~RectTransform() {
}

std::array<glm::vec2, 4> RectTransform::getVertices() const {
	return {
		getVertex(0),
		getVertex(1),
		getVertex(2),
		getVertex(3)
	};
}

glm::vec2 RectTransform::getVertex(std::size_t index) const {
	glm::vec2 pos = getLocalPosition() + getRectOffset();
	if (index == 0) {
		return localToWorldMatrix * pos;
	}
	else if (index == 1) {
		return localToWorldMatrix * glm::vec2(pos.x + size.x, pos.y);
	}
	else if (index == 2) {
		return localToWorldMatrix * glm::vec2(pos.x + size.x, pos.y + size.y);
	}
	else if (index == 3) {
		return localToWorldMatrix * glm::vec2(pos.x, pos.y + size.y);
	}
	throw std::invalid_argument("RectTransform::getVertex::ERROR Vertex index out of bounds!");
}

Rect RectTransform::getRect() const {
	glm::vec2 rectPos = getRectOffset();
	return Rect(rectPos.x, rectPos.y, size.x, size.y);
}

glm::vec2 RectTransform::getRectOffset() const {
	return size * -pivot;
}

const glm::vec2& RectTransform::getSize() const {
	return size;
}
void RectTransform::notifyResize() {
	// Notity scripts attached to the same Entity
	std::vector<UIBehaviour*> scripts = owner.getComponents<UIBehaviour>();
	for (UIBehaviour* script : scripts) {
		script->onRectTransformResized();
	}
	// Notify scripts in parent
	scripts = owner.getParent().getComponents<UIBehaviour>();
	for (UIBehaviour* script : scripts) {
		script->onChildRectTransformResized();
	}
}

void RectTransform::setSize(glm::vec2 size) {
	this->size = size;
	notifyResize();
}
void RectTransform::setWidth(float width) {
	size.x = width;
	notifyResize();
}

void RectTransform::setHeight(float height) {
	size.y = height;
	notifyResize();
}

const glm::vec2& RectTransform::getPivot() const {
	return pivot;
}

void RectTransform::setPivot(glm::vec2 pivot) {
	this->pivot = pivot;
}