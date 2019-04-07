#include "Border.h"
#include <stdexcept>

using namespace Core;

Border::Border(unsigned int width, unsigned int height, std::size_t thickness, glm::vec4 color, bool inner, unsigned int layerIndex, bool top, bool right, bool bottom, bool left) : GraphicComponent(layerIndex, width, height), borderThickness(thickness), color(color), inner(inner), top(top), right(right), bottom(bottom), left(left) {
} // Constructor

Border::Border() {
} // Constructor


Border::~Border() {
} // Destructor

const glm::vec4& Border::getColor() const {
	return color;
}

bool Border::sideEnabled(std::size_t side) const {
	if (side == 0) {
		return top;
	}
	else if (side == 1) {
		return right;
	}
	else if (side == 2) {
		return bottom;
	}
	else if (side == 3) {
		return left;
	}
	std::cout << "Border::sideEnabled::ERROR You can only get side between 0 and 3!\n";
	throw std::invalid_argument("Border::sideEnabled::ERROR You can only get side between 0 and 3!");
}

const std::size_t& Border::getBorderThickness() const {
	return borderThickness;
}

void Border::setInner(bool value) {
	inner = value;
}

bool Border::isInner() const {
	return inner;
}
// ------------------------------- Serializable ----------------------------------------

void Border::serialize(std::ostream& os) const {
	GraphicComponent::serialize(os);

	os.write((char*)&top, sizeof(top));					// top
	os.write((char*)&right, sizeof(right));				// right
	os.write((char*)&bottom, sizeof(bottom));			// bottom
	os.write((char*)&left, sizeof(left));				// left

	os.write((char*)&inner, sizeof(inner));				// inner
	os.write((char*)&borderThickness, sizeof(borderThickness));	// borderSize

	os.write((char*)&color.x, sizeof(color.x));			// color.x	r
	os.write((char*)&color.y, sizeof(color.y));			// color.y	g
	os.write((char*)&color.z, sizeof(color.z));			// color.z	b
	os.write((char*)&color.w, sizeof(color.w));			// color.w	a
}

void Border::deserialize(std::istream& is) {
	GraphicComponent::deserialize(is);

	is.read((char*)&top, sizeof(top));					// top
	is.read((char*)&right, sizeof(right));				// right
	is.read((char*)&bottom, sizeof(bottom));			// bottom
	is.read((char*)&left, sizeof(left));				// left

	is.read((char*)&inner, sizeof(inner));				// inner
	is.read((char*)&borderThickness, sizeof(borderThickness));	// borderSize

	is.read((char*)&color.x, sizeof(color.x));			// color.x	r
	is.read((char*)&color.y, sizeof(color.y));			// color.y	g
	is.read((char*)&color.z, sizeof(color.z));			// color.z	b
	is.read((char*)&color.w, sizeof(color.w));			// color.w	a
}