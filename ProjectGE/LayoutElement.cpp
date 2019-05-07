#include "LayoutElement.h"
#include "BoxComponent.h"

using namespace Core;

LayoutElement::LayoutElement() {
}


LayoutElement::~LayoutElement() {
}


void LayoutElement::setSize(int width, int height) {
	std::vector<BoxComponent*> boxComponents = owner.getComponents<BoxComponent>();
	for (std::size_t i = 1; i < boxComponents.size(); i++) {
		boxComponents[i]->setSize(glm::ivec2(width, height));
	}
}

glm::ivec2 LayoutElement::getSize() {
	glm::ivec2 size(0,0);
	std::vector<BoxComponent*> boxComponents = owner.getComponents<BoxComponent>();
	for (BoxComponent* component : boxComponents) {
		const glm::ivec2& componentSize = component->getSize();
		if (componentSize.x > size.x)
			size.x = componentSize.x;
		if (componentSize.y > size.y)
			size.y = componentSize.y;

	}
	return size;
}