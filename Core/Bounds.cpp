#include "Bounds.h"
#include "BoxComponent.h"

using namespace Core;

Bounds::Bounds() {
}
Bounds::Bounds(glm::vec2 position, glm::ivec2 size) : pos(position), size(size) {
}
Bounds::~Bounds() {
}
/* Creates an AABB in world space. The AABB will surround a rectangle with the specified size and position of top left vertex.
 * @param localPosition defines the local position of the top left vertex of a Box/Rectangle
*/
Bounds Bounds::create(const glm::mat4& localToWorldMatrix, const glm::vec2& localPosition, const glm::ivec2& size) {
	glm::vec2 vertices[4] = {
		localToWorldMatrix * localPosition,
		localToWorldMatrix * glm::vec2(localPosition.x + size.x, localPosition.y),
		localToWorldMatrix * glm::vec2(localPosition.x + size.x, localPosition.y + size.y),
		localToWorldMatrix * glm::vec2(localPosition.x, localPosition.y + size.y)
	};
	float top = vertices[0].y;
	float bottom = vertices[0].y;
	float left = vertices[0].x;
	float right = vertices[0].x;

	for (std::size_t i = 1; i < 4; i++) {
		glm::vec2& vertex = vertices[i];
		if (top > vertex.y)
			top = vertex.y;
		else if (bottom < vertex.y)
			bottom = vertex.y;

		if (left > vertex.x)
			left = vertex.x;
		else if (right < vertex.x)
			right = vertex.x;
	}
	return Bounds(glm::vec2(left, top), glm::ivec2(right - left, bottom - top));
}

Bounds Bounds::getBounds(EntityHandle entity) {
	std::vector<BoxComponent*> boxComponents = entity.getComponents<BoxComponent>();

	if (boxComponents.size() > 0) {
		Bounds elementBounds = boxComponents[0]->getBounds();

		// Create bounds surrounding bounds from all of the box components
		for (std::size_t i = 1; i < boxComponents.size(); i++) {
			Bounds& bounds = boxComponents[i]->getBounds();
			if (elementBounds.pos.x > bounds.pos.x)
				elementBounds.pos.x = bounds.pos.x;
			if (elementBounds.pos.y > bounds.pos.y)
				elementBounds.pos.y = bounds.pos.y;

			if (elementBounds.size.x > bounds.size.x)
				elementBounds.size.x = bounds.size.x;
			if (elementBounds.size.y > bounds.size.y)
				elementBounds.size.y = bounds.size.y;
		}
		return elementBounds;
	}
	return Bounds(glm::vec2(0, 0), glm::ivec2(0, 0));
}