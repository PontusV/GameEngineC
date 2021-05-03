#include "Border.h"
#include "engine/ResourceManager.h"
#include <stdexcept>

using namespace Core;

Border::Border(std::size_t thickness, Color color, bool inner, bool top, bool right, bool bottom, bool left)
	: Graphic(0, ResourceManager::getInstance().loadShader("Shaders/sprite"), color), borderThickness(thickness), inner(inner), top(top), right(right), bottom(bottom), left(left) {
} // Constructor

Border::Border() : Graphic(0, ResourceManager::getInstance().loadShader("Shaders/sprite"), Color(255,255,255,255)) {
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

void Border::setPadding(std::size_t value) {
	padding = value;
}
const std::size_t& Border::getPadding() const {
	return padding;
}