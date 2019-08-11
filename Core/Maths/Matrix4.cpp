#include "Matrix4.h"
#include <stdexcept>
using namespace Core;

Matrix4::Matrix4() : Matrix4(1.0f) {
}

Matrix4::Matrix4(float x) :
	Matrix4(
		Vector4(x,0,0,0),
		Vector4(0,x,0,0),
		Vector4(0,0,x,0),
		Vector4(0,0,0,x)
	)
{
}

Matrix4::Matrix4(Vector4 vec1, Vector4 vec2, Vector4 vec3, Vector4 vec4) : data{vec1, vec2, vec3, vec4} {

}

Matrix4::~Matrix4() {

}

const Vector4& Matrix4::operator[](std::size_t index) const {
	if (index > 3 || index < 0) throw std::out_of_range("Matrix4::operator[]::ERROR Index out of range.");
	return data[index];
}

Vector4& Matrix4::operator[](std::size_t index) {
	if (index > 3 || index < 0) throw std::out_of_range("Matrix4::operator[]::ERROR Index out of range.");
	return data[index];
}