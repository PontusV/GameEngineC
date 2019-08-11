#ifndef MATRIX_4_H
#define MATRIX_4_H
#include "Vector4.h"
#include <cstddef>
namespace Core {
	class Matrix4 {
	public:
		Matrix4(); // Identity Matrix
		Matrix4(float x);
		Matrix4(Vector4 vec1, Vector4 vec2, Vector4 vec3, Vector4 vec4);
		~Matrix4();
		const Vector4& operator[](std::size_t index) const;
		Vector4& operator[](std::size_t index);
	private:
		Vector4 data[4];
	};
}
#endif