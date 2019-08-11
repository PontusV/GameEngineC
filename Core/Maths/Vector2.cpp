#include "Vector2.h"
#include <cmath>
#include <stdexcept>
using namespace Core;


Vector2::Vector2(float x, float y) : x(x), y(y) {
}
Vector2::Vector2(float scalar) : Vector2(scalar, scalar) {
}
Vector2::Vector2() : Vector2(0,0) {
}

Vector2::~Vector2() {
}


float Vector2::length() const {
	return sqrt(x * x + y * y);
}

Vector2 Vector2::floor() const {
	return Vector2(std::floor(x), std::floor(y));
}

Vector2 Vector2::operator-() const {
	return Vector2(-x, -y);
}

Vector2 Vector2::operator+(const Vector2& other) const {
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const {
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(const Vector2& other) const {
	return Vector2(x * other.x, y * other.y);
}

bool Vector2::operator==(const Vector2& other) const {
	return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other) const {
	return !(*this == other);
}

const float& Vector2::operator[](std::size_t index) const {
	if (index == 0) return x;
	else if (index == 1) return y;
	throw std::out_of_range("Vector2::operator[]::ERROR Index out of range.");
}

float& Vector2::operator[](std::size_t index) {
	if (index == 0) return x;
	else if (index == 1) return y;
	throw std::out_of_range("Vector2::operator[]::ERROR Index out of range.");
}

Vector2 Vector2::operator*(const float& value) const {
	return Vector2(x * value, y * value);
}