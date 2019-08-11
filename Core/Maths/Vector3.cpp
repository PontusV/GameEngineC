#include "Vector3.h"
#include <stdexcept>
using namespace Core;

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {
}
Vector3::Vector3(float value) : Vector3(value, value, value) {
}
Vector3::~Vector3() {
}

float Vector3::length() const {
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::operator+(const Vector3& other) const {
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
	return Vector3(x - other.x, y - other.y, z - other.z);
}

bool Vector3::operator==(const Vector3& other) const {
	return x == other.x && y == other.y && z == other.z;
}

bool Vector3::operator==(const Vector3& other) const {
	return !(*this == other);
};

const float& Vector3::operator[](std::size_t index) const {
	if (index == 0) return x;
	else if (index == 1) return y;
	else if (index == 2) return z;
	throw std::out_of_range("Vector3::operator[]::ERROR Index out of range.");
}

float& Vector3::operator[](std::size_t index) {
	if (index == 0) return x;
	else if (index == 1) return y;
	else if (index == 2) return z;
	throw std::out_of_range("Vector3::operator[]::ERROR Index out of range.");
}

Vector3 Vector3::operator*(const float& value) const {
	return Vector3(x * value, y * value, z * value);
}