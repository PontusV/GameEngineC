#pragma once
#include <glm/glm.hpp>
namespace Core {
	glm::vec2& operator*(const glm::mat4& mat, glm::vec2& vec) {
		vec.x = mat[0].x * vec.x + mat[1].x * vec.y + mat[2].x + mat[3].x;
		vec.y = mat[0].y * vec.x + mat[1].y * vec.y + mat[2].y + mat[3].y;
		return vec;
	}
}