#include "BoxComponent.h"
#include "Transform.h"

using namespace Core;

BoxComponent::BoxComponent() : size(0,0) {
} // Constructor

BoxComponent::BoxComponent(int width, int height) : size(width, height) {
} // Constructor

BoxComponent::BoxComponent(Vector2 size) : size(size) {
} // Constructor


BoxComponent::~BoxComponent() {
} // Destructor

void BoxComponent::setSize(int width, int height) {
	size = Vector2(width, height);
}

void BoxComponent::setSize(Vector2 size) {
	setSize(size.x, size.y);
}

void BoxComponent::setWidth(int width) {
	size.x = width;
}

void BoxComponent::setHeight(int height) {
	size.y = height;
}

const Vector2& BoxComponent::getSize() const {
	return size;
}

void BoxComponent::updateBounds() {
	Transform* transform = owner.getComponent<Transform>();
	if (transform) {
		const Matrix4& model = transform->getLocalModelMatrix();
		const Vector2& position = transform->getLocalPosition();
		bounds = Bounds::create(model, position, size);
	}
}

Bounds& BoxComponent::getBounds() {
	return bounds;
}