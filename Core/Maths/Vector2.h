#ifndef VECTOR_2_H
#define VECTOR_2_H
#include <cstddef>
namespace Core {
	class Vector2 {
	public:
		Vector2(float x, float y);
		Vector2(float value);
		~Vector2();

		float length() const;

		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		bool operator==(const Vector2& other) const;
		bool operator==(const Vector2& other) const;
		const float& operator[](std::size_t index) const;
		float& operator[](std::size_t index);
		Vector2 operator*(const float& value) const;
	public:
		float x;
		float y;
	};
}
#endif