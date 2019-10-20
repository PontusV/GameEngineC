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
Matrix4::Matrix4(float data[16]) : data{
	Vector4(data[0],  data[1],  data[2],  data[3]),
	Vector4(data[4],  data[5],  data[6],  data[7]),
	Vector4(data[8],  data[9],  data[10], data[11]),
	Vector4(data[12], data[13], data[14], data[15])
}{
}

Matrix4::~Matrix4() {

}

const float* Matrix4::getDataPtr() const {
	return &data[0][0];
}

Vector4& Matrix4::operator[](std::size_t index) {
	if (index > 3 || index < 0) throw std::out_of_range("Matrix4::operator[]::ERROR Index out of range.");
	return data[index];
}
const Vector4& Matrix4::operator[](std::size_t index) const {
	if (index > 3 || index < 0) throw std::out_of_range("Matrix4::operator[]::ERROR Index out of range.");
	return data[index];
}

Matrix4 Matrix4::operator*(const Matrix4& m2) const {
	/*Matrix4 result(*this);
	result[0][0] = result[0][0] * other[0][0] + result[1][0] * other[0][1] + result[2][0] * other[0][2] + result[3][0] * other[0][3];
	result[1][0] = result[0][0] * other[1][0] + result[1][0] * other[1][1] + result[2][0] * other[1][2] + result[3][0] * other[1][3];
	result[2][0] = result[0][0] * other[2][0] + result[1][0] * other[2][1] + result[2][0] * other[2][2] + result[3][0] * other[2][3];

	result[0][1] = result[0][1] * other[0][0] + result[1][1] * other[0][1] + result[2][1] * other[0][2] + result[3][1] * other[0][3];
	result[1][1] = result[0][1] * other[1][0] + result[1][1] * other[1][1] + result[2][1] * other[1][2] + result[3][1] * other[1][3];
	result[2][1] = result[0][1] * other[2][0] + result[1][1] * other[2][1] + result[2][1] * other[2][2] + result[3][1] * other[2][3];

	result[0][2] = result[0][2] * other[0][0] + result[1][2] * other[0][1] + result[2][2] * other[0][2] + result[3][2] * other[0][3];
	result[1][2] = result[0][2] * other[1][0] + result[1][2] * other[1][1] + result[2][2] * other[1][2] + result[3][2] * other[1][3];
	result[2][2] = result[0][2] * other[2][0] + result[1][2] * other[2][1] + result[2][2] * other[2][2] + result[3][2] * other[2][3];

	result[0][3] = result[0][3] * other[0][0] + result[1][3] * other[0][1] + result[2][3] * other[0][2] + result[3][3] * other[0][3];
	result[1][3] = result[0][3] * other[1][0] + result[1][3] * other[1][1] + result[2][3] * other[1][2] + result[3][3] * other[1][3];
	result[2][3] = result[0][3] * other[2][0] + result[1][3] * other[2][1] + result[2][3] * other[2][2] + result[3][3] * other[2][3];
	return result;*/
	/*Matrix4 result;
	const Matrix4& mat = *this;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float sumElements = 0.0f;
			for (int k = 0; k < 4; k++) {
				sumElements += mat[i][k] * other[k][j];
			}
			result[i][j] = sumElements;
		}
	}
	return result;*/
	Matrix4 result;
	const Matrix4& m1 = *this;
	result[0] = m1[0] * m2[0][0] + m1[1] * m2[0][1] + m1[2] * m2[0][2] + m1[3] * m2[0][3];
	result[1] = m1[0] * m2[1][0] + m1[1] * m2[1][1] + m1[2] * m2[1][2] + m1[3] * m2[1][3];
	result[2] = m1[0] * m2[2][0] + m1[1] * m2[2][1] + m1[2] * m2[2][2] + m1[3] * m2[2][3];
	result[3] = m1[0] * m2[3][0] + m1[1] * m2[3][1] + m1[2] * m2[3][2] + m1[3] * m2[3][3];
	return result;
}