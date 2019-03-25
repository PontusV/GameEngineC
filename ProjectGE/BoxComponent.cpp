#include "BoxComponent.h"
#include "Chunk.h"
#include "Transform.h"

using namespace Core;

BoxComponent::BoxComponent() : size(0,0), initPosition(true) {
} // Constructor

BoxComponent::BoxComponent(unsigned int width, unsigned int height) : size(width, height), initPosition(true) {
} // Constructor

BoxComponent::BoxComponent(glm::ivec2 size) : size(size), initPosition(true) {
} // Constructor


BoxComponent::~BoxComponent() {
} // Destructor

void BoxComponent::setSize(unsigned int width, unsigned int height) {
	this->size = glm::ivec2(width, height);
	initPosition = true;
}

void BoxComponent::setSize(glm::ivec2 size) {
	this->size = size;
	initPosition = true;
}
const glm::ivec2& BoxComponent::getSize() const {
	return size;
}

const glm::vec2& BoxComponent::getOffset() {
	if (initPosition) {
		Transform* transform = static_cast<Transform*>(owner.getComponent(Transform::TYPE_ID));
		if (transform) {
			offset = transform->calculateOffset(size);
		}
		else {
			std::cout << "BoxComponent: Could not find a Transform!\n";
		}
		initPosition = false;
	}
	return offset;
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
	initPosition = true;
}