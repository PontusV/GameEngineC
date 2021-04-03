#ifndef MATRIX_TRANSFORM_H
#define MATRIX_TRANSFORM_H
#define _USE_MATH_DEFINES
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include <math.h>
namespace Core {
	namespace maths {
		inline Vector2 normalize(const Vector2& vec) {
			float length = vec.length();
			return Vector2(vec.x / length, vec.y / length);
		}

		inline Vector3 normalize(const Vector3& vec) {
			float length = vec.length();
			return Vector3(vec.x / length, vec.y / length, vec.z / length);
		}

		inline Vector4 normalize(const Vector4& vec) {
			float length = vec.length();
			return Vector4(vec.x / length, vec.y / length, vec.z / length, vec.w / length);
		}

		inline float distance(const Vector2& lhs, const Vector2 rhs) {
			return std::abs(std::sqrt(std::pow(rhs.x - lhs.x, 2) + std::pow(rhs.y - lhs.y, 2)));
		}

		inline Matrix4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
			Matrix4 result;
			result[0][0] = 2 / (right - left);
			result[1][1] = 2 / (top - bottom);
			result[2][2] = -2 / (zFar - zNear);
			result[3][0] = -(right + left) / (right - left);
			result[3][1] = -(top + bottom) / (top - bottom);
			result[3][2] = -(zFar + zNear) / (zFar - zNear);
			return result;
		}

		inline Matrix4 translate(const Matrix4& mat, const Vector3& vec) {
			Matrix4 result(mat);
			result[3] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2] + mat[3];
			return result;
		}

		inline Matrix4 rotate(const Matrix4& mat, float angle, const Vector3& vec) {
			float c = cos(angle);
			float s = sin(angle);

			Vector3 axis(normalize(vec));
			Vector3 temp((1.0f - c) * axis);

			Matrix4 rotation;
			rotation[0][0] = c + temp[0] * axis[0];
			rotation[0][1] = temp[0] * axis[1] + s * axis[2];
			rotation[0][2] = temp[0] * axis[2] - s * axis[1];

			rotation[1][0] = temp[1] * axis[0] - s * axis[2];
			rotation[1][1] = c + temp[1] * axis[1];
			rotation[1][2] = temp[1] * axis[2] + s * axis[0];

			rotation[2][0] = temp[2] * axis[0] + s * axis[1];
			rotation[2][1] = temp[2] * axis[1] - s * axis[0];
			rotation[2][2] = c + temp[2] * axis[2];

			Matrix4 result;
			result[0] = mat[0] * rotation[0][0] + mat[1] * rotation[0][1] + mat[2] * rotation[0][2];
			result[1] = mat[0] * rotation[1][0] + mat[1] * rotation[1][1] + mat[2] * rotation[1][2];
			result[2] = mat[0] * rotation[2][0] + mat[1] * rotation[2][1] + mat[2] * rotation[2][2];
			result[3] = mat[3];
			return result;
		}

		inline Matrix4 scale(const Matrix4& mat, const Vector3& vec) {
			Matrix4 result;
			result[0] = mat[0] * vec[0];
			result[1] = mat[1] * vec[1];
			result[2] = mat[2] * vec[2];
			result[3] = mat[3];
			return result;
		}

		namespace {
			inline bool gluInvertMatrix(const float m[16], float invOut[16]) {
				float inv[16], det;
				int i;

				inv[0] = m[5] * m[10] * m[15] -
					m[5] * m[11] * m[14] -
					m[9] * m[6] * m[15] +
					m[9] * m[7] * m[14] +
					m[13] * m[6] * m[11] -
					m[13] * m[7] * m[10];

				inv[4] = -m[4] * m[10] * m[15] +
					m[4] * m[11] * m[14] +
					m[8] * m[6] * m[15] -
					m[8] * m[7] * m[14] -
					m[12] * m[6] * m[11] +
					m[12] * m[7] * m[10];

				inv[8] = m[4] * m[9] * m[15] -
					m[4] * m[11] * m[13] -
					m[8] * m[5] * m[15] +
					m[8] * m[7] * m[13] +
					m[12] * m[5] * m[11] -
					m[12] * m[7] * m[9];

				inv[12] = -m[4] * m[9] * m[14] +
					m[4] * m[10] * m[13] +
					m[8] * m[5] * m[14] -
					m[8] * m[6] * m[13] -
					m[12] * m[5] * m[10] +
					m[12] * m[6] * m[9];

				inv[1] = -m[1] * m[10] * m[15] +
					m[1] * m[11] * m[14] +
					m[9] * m[2] * m[15] -
					m[9] * m[3] * m[14] -
					m[13] * m[2] * m[11] +
					m[13] * m[3] * m[10];

				inv[5] = m[0] * m[10] * m[15] -
					m[0] * m[11] * m[14] -
					m[8] * m[2] * m[15] +
					m[8] * m[3] * m[14] +
					m[12] * m[2] * m[11] -
					m[12] * m[3] * m[10];

				inv[9] = -m[0] * m[9] * m[15] +
					m[0] * m[11] * m[13] +
					m[8] * m[1] * m[15] -
					m[8] * m[3] * m[13] -
					m[12] * m[1] * m[11] +
					m[12] * m[3] * m[9];

				inv[13] = m[0] * m[9] * m[14] -
					m[0] * m[10] * m[13] -
					m[8] * m[1] * m[14] +
					m[8] * m[2] * m[13] +
					m[12] * m[1] * m[10] -
					m[12] * m[2] * m[9];

				inv[2] = m[1] * m[6] * m[15] -
					m[1] * m[7] * m[14] -
					m[5] * m[2] * m[15] +
					m[5] * m[3] * m[14] +
					m[13] * m[2] * m[7] -
					m[13] * m[3] * m[6];

				inv[6] = -m[0] * m[6] * m[15] +
					m[0] * m[7] * m[14] +
					m[4] * m[2] * m[15] -
					m[4] * m[3] * m[14] -
					m[12] * m[2] * m[7] +
					m[12] * m[3] * m[6];

				inv[10] = m[0] * m[5] * m[15] -
					m[0] * m[7] * m[13] -
					m[4] * m[1] * m[15] +
					m[4] * m[3] * m[13] +
					m[12] * m[1] * m[7] -
					m[12] * m[3] * m[5];

				inv[14] = -m[0] * m[5] * m[14] +
					m[0] * m[6] * m[13] +
					m[4] * m[1] * m[14] -
					m[4] * m[2] * m[13] -
					m[12] * m[1] * m[6] +
					m[12] * m[2] * m[5];

				inv[3] = -m[1] * m[6] * m[11] +
					m[1] * m[7] * m[10] +
					m[5] * m[2] * m[11] -
					m[5] * m[3] * m[10] -
					m[9] * m[2] * m[7] +
					m[9] * m[3] * m[6];

				inv[7] = m[0] * m[6] * m[11] -
					m[0] * m[7] * m[10] -
					m[4] * m[2] * m[11] +
					m[4] * m[3] * m[10] +
					m[8] * m[2] * m[7] -
					m[8] * m[3] * m[6];

				inv[11] = -m[0] * m[5] * m[11] +
					m[0] * m[7] * m[9] +
					m[4] * m[1] * m[11] -
					m[4] * m[3] * m[9] -
					m[8] * m[1] * m[7] +
					m[8] * m[3] * m[5];

				inv[15] = m[0] * m[5] * m[10] -
					m[0] * m[6] * m[9] -
					m[4] * m[1] * m[10] +
					m[4] * m[2] * m[9] +
					m[8] * m[1] * m[6] -
					m[8] * m[2] * m[5];

				det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

				if (det == 0)
					return false;

				det = 1.0f / det;

				for (i = 0; i < 16; i++)
					invOut[i] = inv[i] * det;

				return true;
			}
		}

		inline Matrix4 inverse(Matrix4 mat) {
			float out[16];
			float in[16];
			for (std::size_t i = 0; i < 4; i++) {
				for (std::size_t i2 = 0; i2 < 4; i2++) {
					std::size_t index = 4 * i + i2;
					in[index] = mat[i][i2];
				}
			}
			gluInvertMatrix(in, out);
			for (std::size_t i = 0; i < 4; i++) {
				for (std::size_t i2 = 0; i2 < 4; i2++) {
					std::size_t index = 4 * i + i2;
					mat[i][i2] = out[index];
				}
			}
			return mat;
		}

		inline float radians(float degrees) {
			return (degrees * (float)M_PI) / 180;
		}
	}

	inline Vector2 operator*(const Matrix4& mat, const Vector2& vec) {
		return Vector2(
			vec.x * mat[0][0] + vec.y * mat[1][0] + mat[3][0],
			vec.x * mat[0][1] + vec.y * mat[1][1] + mat[3][1]
		);
	}

	inline Vector3 operator*(const Matrix4& mat, const Vector3& vec) {
		return Vector3(
			vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0] + mat[3][0],
			vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1] + mat[3][1],
			vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2] + mat[3][2]
		);
	}

	inline Vector4 operator*(const Matrix4& mat, const Vector4& vec) {
		return Vector4(
			vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0] + vec.w * mat[3][0],
			vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1] + vec.w * mat[3][1],
			vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2] + vec.w * mat[3][2],
			vec.x * mat[0][3] + vec.y * mat[1][3] + vec.z * mat[2][3] + vec.w * mat[3][3]
		);
	}
}
#endif