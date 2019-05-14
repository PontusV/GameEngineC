#include "RectTransform.h"
#include "UIBehaviour.h"

using namespace Core;

RectTransform::RectTransform(float x, float y, int width, int height, float z, Anchor pivot, float rotation, float scale) : Transform(x, y, z, rotation, scale), size(width, height), pivot(pivot) {
}

RectTransform::RectTransform() {
}


RectTransform::~RectTransform() {
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

// ------------------------------- Serializable ----------------------------------------

void RectTransform::serialize(std::ostream& os) const {
	Transform::serialize(os);
	os.write((char*)&pivot.x, sizeof(pivot.x));			// Pivot x
	os.write((char*)&pivot.y, sizeof(pivot.y));			// Pivot y
}

void RectTransform::deserialize(std::istream& is) {
	Transform::deserialize(is);
	is.read((char*)&pivot.x, sizeof(pivot.x));			// Pivot x
	is.read((char*)&pivot.y, sizeof(pivot.y));			// Pivot y

	changed = true;
}