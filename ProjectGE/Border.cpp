#include "Border.h"
#include "Sprite.h"
#include <stdexcept>

using namespace Core;

Border::Border(std::size_t thickness, Color color, bool inner, unsigned int layerIndex, bool top, bool right, bool bottom, bool left) : Sprite(layerIndex, color), borderThickness(thickness), inner(inner), top(top), right(right), bottom(bottom), left(left) {
} // Constructor

Border::Border() {
} // Constructor


Border::~Border() {
} // Destructor

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
	Sprite::serialize(os);

	os.write((char*)&top, sizeof(top));					// top
	os.write((char*)&right, sizeof(right));				// right
	os.write((char*)&bottom, sizeof(bottom));			// bottom
	os.write((char*)&left, sizeof(left));				// left

	os.write((char*)&inner, sizeof(inner));				// inner
	os.write((char*)&borderThickness, sizeof(borderThickness));	// borderSize
}

void Border::deserialize(std::istream& is) {
	Sprite::deserialize(is);

	is.read((char*)&top, sizeof(top));					// top
	is.read((char*)&right, sizeof(right));				// right
	is.read((char*)&bottom, sizeof(bottom));			// bottom
	is.read((char*)&left, sizeof(left));				// left

	is.read((char*)&inner, sizeof(inner));				// inner
	is.read((char*)&borderThickness, sizeof(borderThickness));	// borderSize
}