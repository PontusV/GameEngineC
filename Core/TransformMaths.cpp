#include "TransformMaths.h"


glm::vec2 operator*(const glm::mat4& mat, const glm::vec2& vec) {
	return glm::vec2(
		vec.x * mat[0].x + vec.y * mat[1].x + mat[3].x,
		vec.x * mat[0].y + vec.y * mat[1].y + mat[3].y
	);
}