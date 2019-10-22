#include "RectTransform.h"
#include "components/graphics/ui/UIBehaviour.h"
#include "maths/MatrixTransform.h"

using namespace Core;

RectTransform::RectTransform(float x, float y, float width, float height, float z, Anchor pivot, float rotation, float scale) : Transform(x, y, z, rotation, scale), size(width, height), pivot(pivot) {
}

RectTransform::RectTransform() {
}


RectTransform::~RectTransform() {
}


std::array<Vector2, 4> RectTransform::getLocalVertices() const {
	return {
		getLocalVertex(0),
		getLocalVertex(1),
		getLocalVertex(2),
		getLocalVertex(3)
	};
}

Vector2 RectTransform::getLocalVertex(std::size_t index) const {
	Vector2 pos = getRectOffset();
	if (index == 0) {
		return position + pos;
	}
	else if (index == 1) {
		return position + Vector2(pos.x, pos.y + size.y);
	}
	else if (index == 2) {
		return position + pos + size;
	}
	else if (index == 3) {
		return position + Vector2(pos.x + size.x, pos.y);
	}
	throw std::invalid_argument("RectTransform::getVertex::ERROR Vertex index out of bounds!");
}

std::array<Vector2, 4> RectTransform::getVertices() const {
	Matrix4 matrix = localToWorldMatrix * localModelMatrix;
	return {
		getVertex(0, matrix),
		getVertex(1, matrix),
		getVertex(2, matrix),
		getVertex(3, matrix)
	};
}

std::array<Vector2, 4> RectTransform::getVertices(const Matrix4& multMatrix) const {
	Matrix4 matrix = localToWorldMatrix * localModelMatrix * multMatrix;
	return {
		getVertex(0, matrix),
		getVertex(1, matrix),
		getVertex(2, matrix),
		getVertex(3, matrix)
	};
}

Vector2 RectTransform::getVertex(std::size_t index) const {
	Matrix4 matrix = localToWorldMatrix * localModelMatrix;
	return getVertex(index, matrix);
}

Vector2 RectTransform::getVertex(std::size_t index, Matrix4& matrix) const {
	Vector2 pos = getRectOffset();
	if (index == 0) {
		return matrix * pos;
	}
	else if (index == 1) {
		return matrix * Vector2(pos.x, pos.y + size.y);
	}
	else if (index == 2) {
		return matrix * (pos + size);
	}
	else if (index == 3) {
		return matrix * Vector2(pos.x + size.x, pos.y);
	}
	throw std::invalid_argument("RectTransform::getVertex::ERROR Vertex index out of bounds!");
}

Rect RectTransform::getRect() const {
	Vector2 rectPos = getRectOffset();
	return Rect(rectPos.x, rectPos.y, size.x, size.y);
}

Vector2 RectTransform::getRectOffset() const {
	return size * -pivot;
}

const Vector2& RectTransform::getSize() const {
	return size;
}
void RectTransform::notifyResize() {
	changed = true;
	// TODO: Move this to a system
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

void RectTransform::setSize(Vector2 size) {
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

const Vector2& RectTransform::getPivot() const {
	return pivot;
}

void RectTransform::setPivot(Vector2 pivot) {
	this->pivot = pivot;
}