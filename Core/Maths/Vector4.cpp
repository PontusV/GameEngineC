#include "Vector4.h"
#include <stdexcept>
using namespace Core;

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
}
Vector4::Vector4(float value) : Vector4(value, value, value, value) {
}
Vector4::Vector4() : Vector4(0, 0, 0, 0) {
}
Vector4::~Vector4() {
}

float Vector4::length() const {
	return sqrt(x * x + y * y + z * z + w * w);
}

Vector4 Vector4::operator-() const {
	return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator+(const Vector4& other) const {
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator-(const Vector4& other) const {
	return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator*(const Vector4& other) const {
	return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
}

bool Vector4::operator==(const Vector4& other) const {
	return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool Vector4::operator!=(const Vector4& other) const {
	return !(*this == other);
};

const float& Vector4::operator[](std::size_t index) const {
	if (index == 0) return x;
	else if (index == 1) return y;
	else if (index == 2) return z;
	else if (index == 3) return w;
	throw std::out_of_range("Vector4::operator[]::ERROR Index out of range.");
}

float& Vector4::operator[](std::size_t index) {
	if (index == 0) return x;
	else if (index == 1) return y;
	else if (index == 2) return z;
	else if (index == 3) return w;
	throw std::out_of_range("Vector4::operator[]::ERROR Index out of range.");
}

Vector4 Vector4::operator*(const float& value) const {
	return Vector4(x * value, y * value, z * value, w * value);
}