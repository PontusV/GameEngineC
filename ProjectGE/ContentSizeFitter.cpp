#include "ContentSizeFitter.h"
#include "RectTransform.h"

using namespace Core;

ContentSizeFitter::ContentSizeFitter()
{
}


ContentSizeFitter::~ContentSizeFitter()
{
}


void ContentSizeFitter::updateLayout() {
	RectTransform* transform = owner.getComponent<RectTransform>();
	if (transform) {
		float width;
		float height;
		if (horizontalFit == Mode::UNCONSTRAINED) {
			width = transform->getSize().x;
		}
		else if (horizontalFit == Mode::MINSIZE) {
			width = LayoutController::getMinSize(owner).x;
		}
		else if (horizontalFit == Mode::PREFERREDSIZE) {
			width = LayoutController::getPrefSize(owner).x;
		}
		if (verticalFit == Mode::UNCONSTRAINED) {
			height = transform->getSize().y;
		}
		else if (verticalFit == Mode::MINSIZE) {
			height = LayoutController::getMinSize(owner).y;
		}
		else if (verticalFit == Mode::PREFERREDSIZE) {
			height = LayoutController::getPrefSize(owner).y;
		}
		transform->setSize(glm::vec2(width, height));
	}
}