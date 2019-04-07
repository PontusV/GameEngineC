#include "BoxComponent.h"
#include "Chunk.h"
#include "Transform.h"

using namespace Core;

BoxComponent::BoxComponent() : size(0,0) {
} // Constructor

BoxComponent::BoxComponent(unsigned int width, unsigned int height) : size(width, height) {
} // Constructor

BoxComponent::BoxComponent(glm::ivec2 size) : size(size) {
} // Constructor


BoxComponent::~BoxComponent() {
} // Destructor

void BoxComponent::setSize(unsigned int width, unsigned int height) {
	size = glm::ivec2(width, height);
}

void BoxComponent::setSize(glm::ivec2 size) {
	this->size = size;
}

const glm::ivec2& BoxComponent::getSize() const {
	return size;
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