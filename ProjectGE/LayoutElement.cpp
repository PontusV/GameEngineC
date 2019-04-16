#include "LayoutElement.h"
#include "BoxComponent.h"

using namespace Core;

LayoutElement::LayoutElement() {
}


LayoutElement::~LayoutElement() {
}

Bounds LayoutElement::getBounds() {
	std::vector<BoxComponent*> boxComponents = owner.getComponents<BoxComponent>();

	if (boxComponents.size() > 0) {
		Bounds& elementBounds = boxComponents[0]->getBounds();

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
	return Bounds(glm::vec2(0,0), glm::ivec2(0,0));
	//return AxisAlignedBoundingBox::createAABB();
}