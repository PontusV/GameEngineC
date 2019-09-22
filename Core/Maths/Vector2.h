#ifndef VECTOR_2_H
#define VECTOR_2_H
#include <cstddef>
#include <ReflectionParser/ReflectionMacros.h>
#include "../Vector2.generated.h"
namespace Core {
	CLASS() Vector2 {
		GENERATED_BODY()
	public:
		template<typename T, typename T2>
		Vector2(T x, T2 y) : x(float(x)), y(float(y)){}
		Vector2(float scalar);
		Vector2();
		~Vector2();

		float length() const;
		Vector2 floor() const;

		Vector2 operator-() const;
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator*(const Vector2& other) const;
		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;
		const float& operator[](std::size_t index) const;
		float& operator[](std::size_t index);
		Vector2 operator*(const float& scalar) const;
	public:
		PROPERTY()
		float x;
		PROPERTY()
		float y;
	};

	template<typename T>
	struct TVector2 {
		TVector2(T x, T y) : x(x), y(y) {}
		TVector2(T value) : x(value), y(value) {}
		TVector2() : x(0), y(0) {}
		~TVector2() {}

		T length() const {
			return x + y;
		}

		TVector2<T> operator-() const {
			return TVector2<T>(-x, -y);
		}

		TVector2<T> operator+(const TVector2<T>& other) const {
			return Vector2(x + other.x, y + other.y);
		}

		TVector2<T> operator-(const TVector2<T>& other) const {
			return TVector2<T>(x - other.x, y - other.y);
		}

		TVector2<T> operator*(const TVector2<T>& other) const {
			return TVector2<T>(x * other.x, y * other.y);
		}

		bool operator==(const TVector2<T>& other) const {
			return x == other.x && y == other.y;
		}

		bool operator!=(const TVector2<T>& other) const {
			return !(*this == other);
		}

		const float& operator[](std::size_t index) const {
			if (index == 0) return x;
			else if (index == 1) return y;
			throw std::out_of_range("Vector2::operator[]::ERROR Index out of range.");
		}

		float& operator[](std::size_t index) {
			if (index == 0) return x;
			else if (index == 1) return y;
			throw std::out_of_range("Vector2::operator[]::ERROR Index out of range.");
		}

		TVector2<T> operator*(const float& value) const {
			return Vector2(x * value, y * value);
		}

		T x;
		T y;
	};
}
#endif