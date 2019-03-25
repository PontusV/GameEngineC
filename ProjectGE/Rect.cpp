#include "Rect.h"

using namespace Core;

#include "ComponentLoader.h"
REGISTER_LOADABLE_COMPONENT(Rect);


Rect::Rect(int width, int height, glm::vec4 color, unsigned short layerIndex) : GraphicComponent(layerIndex, width, height), color(color) {
} // Constructor

Rect::Rect() {
}

Rect::~Rect() {
} // Destructor

const glm::vec4& Rect::getColor() const {
	return color;
}

// ------------------------------- Serializable ----------------------------------------

void Rect::serialize(std::ostream &os) const {
	GraphicComponent::serialize(os);

	os.write((char*)&color.x, sizeof(color.x));			// Color x
	os.write((char*)&color.y, sizeof(color.y));			// Color y
	os.write((char*)&color.z, sizeof(color.z));			// Color z
	os.write((char*)&color.w, sizeof(color.w));			// Color w
}

void Rect::deserialize(std::istream &is) {
	GraphicComponent::deserialize(is);

	is.read((char*)&color.x, sizeof(color.x));			// Color x
	is.read((char*)&color.y, sizeof(color.y));			// Color y
	is.read((char*)&color.z, sizeof(color.z));			// Color z
	is.read((char*)&color.w, sizeof(color.w));			// Color w
}