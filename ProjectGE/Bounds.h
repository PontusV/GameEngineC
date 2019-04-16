#ifndef BOUNDS_H
#define BOUNDS_H
#include "TransformMaths.h"
#include <glm/glm.hpp>
namespace Core {
	/* Axis Aligned Bounding Box */
	class Bounds {
	public:
		Bounds();
		Bounds(glm::vec2 position, glm::ivec2 size);
		~Bounds();

		/* Creates AABB in world space */
		static Bounds create(const glm::mat4& localToWorldMatrix, const glm::vec2& localPosition, const glm::ivec2& size);

	public:
		glm::vec2 pos;
		glm::ivec2 size;
	};
}
#endif