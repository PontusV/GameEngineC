#include "BoxComponent.h"
#include "Transform.h"

using namespace Core;

BoxComponent::BoxComponent() : size(0,0) {
} // Constructor

BoxComponent::BoxComponent(int width, int height) : size(width, height) {
} // Constructor

BoxComponent::BoxComponent(glm::ivec2 size) : size(size) {
} // Constructor


BoxComponent::~BoxComponent() {
} // Destructor

void BoxComponent::setSize(int width, int height) {
	size = glm::ivec2(width, height);
}

void BoxComponent::setSize(glm::ivec2 size) {
	setSize(size.x, size.y);
}

void BoxComponent::setWidth(int width) {
	size.x = width;
}

void BoxComponent::setHeight(int height) {
	size.y = height;
}

const glm::ivec2& BoxComponent::getSize() const {
	return size;
}

void BoxComponent::updateBounds() {
	Transform* transform = owner.getComponent<Transform>();
	if (transform) {
		const glm::mat4& model = transform->getLocalModelMatrix();
		const glm::vec2& position = transform->getPosition();
		bounds = Bounds::create(model, position, size);
	}
}

Bounds& BoxComponent::getBounds() {
	return bounds;
}

// ------------------------------- Serializable ----------------------------------------

void BoxComponent::serialize(std::ostream& os) const {
	Component::serialize(os);

	os.write((char*)&size.x, sizeof(size.x));			// Size x
	os.write((char*)&size.y, sizeof(size.y));			// Size y
}

void BoxComponent::deserialize(std::istream& is) {
	Component::deserialize(is);

	is.read((char*)&size.x, sizeof(size.x));			// Size x
	is.read((char*)&size.y, sizeof(size.y));			// Size y
}