#ifndef RECT_TRANSFORM_H
#define RECT_TRANSFORM_H
#include "components/Transform.h"
#include "utils/Alignment.h"
#include "utils/Anchor.h"
namespace Core {

	struct Rect {
		Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
		float x, y, w, h;
		bool operator==(const Rect& other) {
			return (x == other.x &&
				y == other.y &&
				w == other.w &&
				h == other.h);
		}
		bool operator!=(const Rect& other) {
			return (x != other.x ||
				y != other.y ||
				w != other.w ||
				h != other.h);
		}
	};

	#include "RectTransform.generated.h"

	/* A Transform with a size and pivot. */
	CLASS() RectTransform : public Transform {
		GENERATED_BODY()
	public:
		RectTransform(float x, float y, float width, float height, float z = 0.0f, Anchor pivot = Alignment::CENTER, float rotation = 0.0f, float scale = 1.0f);
		RectTransform();
		~RectTransform();


		void setSize(Vector2 size);
		void setWidth(float width);
		void setHeight(float height);
		void setPivot(Vector2 pivot);

		const Vector2& getPivot() const;
		const Vector2& getSize() const;
		/* Returns a rectangle in local space. */
		Rect getRect() const;
		Vector2 getRectOffset() const;
		/* Returns the vetices in local space. */
		std::array<Vector2, 4> getLocalVertices() const;
		/* Returns the vertex in local space. */
		Vector2 getLocalVertex(std::size_t index) const;
		/* Returns the vertices in world space. */
		std::array<Vector2, 4> getVertices() const;
		/* Returns the vertices in world space. Multiplies the vertices with the given matrix. */
		std::array<Vector2, 4> getVertices(const Matrix4& matrix) const;
		/* Returns the vertex in world space. */
		Vector2 getVertex(std::size_t index) const;
		Vector2 getVertex(std::size_t index, Matrix4& matrix) const;

	private:
		/* Notifies UIBehaviours of a change in size */
		void notifyResize();

	private:
		PROPERTY(Update=setChanged())
		Vector2 size;	// Size of the rect
		PROPERTY(Update=setChanged())
		Vector2 pivot;	// The position the Entity scales and rotates around
	};
}
#endif