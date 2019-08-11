#ifndef VECTOR_3_H
#define VECTOR_3_H
#include <cstddef>
namespace Core {
	class Vector3 {
	public:
		Vector3(float x, float y, float z);
		Vector3(float value);
		Vector3();
		~Vector3();

		float length() const;

		Vector3 operator-() const;
		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(const Vector3& other) const;
		bool operator==(const Vector3& other) const;
		bool operator!=(const Vector3& other) const;
		const float& operator[](std::size_t index) const;
		float& operator[](std::size_t index);
		Vector3 operator*(const float& scalar) const;
	public:
		float x;
		float y;
		float z;
	};
	inline Vector3 operator*(const float& scalar, const Vector3& vec) {
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}
}
#endif