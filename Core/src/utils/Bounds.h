#ifndef BOUNDS_H
#define BOUNDS_H
#include "entity/handle/EntityHandle.h"
#include "maths/Vector2.h"
#include "maths/Matrix4.h"
namespace Core {
	/* Axis Aligned Bounding Box */
	class Bounds {
	public:
		Bounds();
		Bounds(Vector2 position, Vector2 size);
		~Bounds();

		/* Creates AABB in world space */
		static Bounds create(const Matrix4& localToWorldMatrix, const Vector2& localPosition, const Vector2& size);

		/* Creates a new Axis Aligned Bounding Box in World Space that surrounds the whole Entity. */
		Bounds getBounds(EntityHandle entity);

	public:
		Vector2 pos;
		Vector2 size;
	};
}
#endif