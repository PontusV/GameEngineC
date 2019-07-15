#ifndef RECT_TRANSFORM_H
#define RECT_TRANSFORM_H
#include "Transform.h"
#include "BoxComponent.h"
#include "Alignment.h"
#include "Anchor.h"
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

	#include "RectTransform.RectTransform.generated.h"

	/* A Transform with a size and pivot. */
	CLASS() RectTransform : public Transform {
		GENERATED_BODY()
	public:
		RectTransform(float x, float y, int width, int height, float z = 0.0f, Anchor pivot = Alignment::CENTER, float rotation = 0.0f, float scale = 1.0f);
		RectTransform();
		~RectTransform();


		void setSize(glm::vec2 size);
		void setWidth(float width);
		void setHeight(float height);
		void setPivot(glm::vec2 pivot);

		const glm::vec2& getPivot() const;
		const glm::vec2& getSize() const;
		/* Returns a rectangle in local space. */
		Rect getRect() const;
		glm::vec2 getRectOffset() const;
		std::array<glm::vec2, 4> getVertices() const;
		glm::vec2 getVertex(std::size_t index) const;

	private:
		/* Notifies UIBehaviours of a change in size */
		void notifyResize();

	private:
		PROPERTY()
		glm::vec2 size;	// Size of the rect
		PROPERTY()
		glm::vec2 pivot;	// The position the Entity scales and rotates around
	};
}
#endif