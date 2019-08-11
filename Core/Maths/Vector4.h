#ifndef VECTOR_4_H
#define VECTOR_4_H
#include <cstddef>
namespace Core {
	class Vector4 {
	public:
		Vector4(float x, float y, float z, float w);
		Vector4(float value);
		~Vector4();

		float length() const;

		Vector4 operator+(const Vector4& other) const;
		Vector4 operator-(const Vector4& other) const;
		bool operator==(const Vector4& other) const;
		bool operator==(const Vector4& other) const;
		const float& operator[](std::size_t index) const;
		float& operator[](std::size_t index);
		Vector4 operator*(const float& value) const;
	public:
		float x;
		float y;
		float z;
		float w;
	};
}
#endif