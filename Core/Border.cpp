#include "Border.h"
#include "Sprite.h"
#include <stdexcept>

using namespace Core;

Border::Border(std::size_t thickness, Color color, std::size_t sortingLayer, bool inner, bool top, bool right, bool bottom, bool left) : Sprite(color, sortingLayer), borderThickness(thickness), inner(inner), top(top), right(right), bottom(bottom), left(left) {
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