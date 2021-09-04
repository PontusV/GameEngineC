#include "ScrollBar.h"
#include "components/graphics/RectSprite.h"
#include "components/RectTransform.h"
#include "input/Input.h"
#include "components/ui/ScrollRect.h"
using namespace Core;

ScrollBar::ScrollBar(EntityHandle target) : target(target) {
}
ScrollBar::ScrollBar() {
}
ScrollBar::~ScrollBar() {
}


void ScrollBar::start() {
	// Create all parts
	RectTransform* rect = owner.getComponent<RectTransform>();
	EntityHandle ownerHandle = owner;
	if (rect) {
		// Background
		ownerHandle.addComponent(RectSprite(0, backgroundColor));

		// Handle
		float handleWidth = 0;
		float handleHeight = 0;
		if (scrollY) {
			handleWidth = rect->getSize().x - handlePadding * 2;
		}
		else {
			handleHeight = rect->getSize().y - handlePadding * 2;
		}
		handle = createEntity(
			RectSprite(0, handleColorDefault),
			RectTransform(0, 0, handleWidth, handleHeight, rect->getZ() + 0.01f, Alignment::TOP_LEFT)
		);
		handle.setParent(owner);
	}
}

void ScrollBar::onDestroy() {
	//destroyEntity(handle);
}

void ScrollBar::onPreRender(float deltaTime) {
	if (!handle.refresh()) return;
	if (!target.refresh()) {
		RectTransform* handleRect = handle.getComponent<RectTransform>();
		if (handleRect->getSize().y != 0.0f || handleRect->getSize().x != 0.0f)
			handleRect->setSize(Vector2(0.0f, 0.0f));
		return;
	}
	// Handle color
	if (input->getHoverTarget() == handle) {
		hovering = true;
		if (!handleDrag)
			handle.getComponent<RectSprite>()->setColor(handleColorHighlight);
	}
	else {
		if (!handleDrag)
			handle.getComponent<RectSprite>()->setColor(handleColorDefault);
		hovering = false;
	}
	// Update position and size of handle
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (rect) {
		ScrollRect* scrollRect = target.getComponent<ScrollRect>();
		if (scrollY && scrollRect->scrollableY) {
			RectTransform* handleRect = handle.getComponent<RectTransform>();
			RectTransform* scrollArea = target.getComponent<RectTransform>();

			float scrollRectHeight = scrollRect->getContentBounds().size.y + scrollRect->paddingTop + scrollRect->paddingBottom;
			float handleHeight = rect->getSize().y * scrollArea->getSize().y / scrollRectHeight;
			if (handleHeight > rect->getSize().y) handleHeight = rect->getSize().y;
			handleRect->setHeight(handleHeight);
			Vector2 position(handlePadding, rect->getSize().y * (-scrollRect->getContentBounds().pos.y - scrollRect->offset.y) / scrollRectHeight);
			handleRect->setLocalPosition(position + rect->getRectOffset());
		}
		else if (!scrollY && scrollRect->scrollableX) {
			RectTransform* handleRect = handle.getComponent<RectTransform>();
			RectTransform* scrollArea = target.getComponent<RectTransform>();

			float scrollRectWidth = scrollRect->getContentBounds().size.x + scrollRect->paddingTop + scrollRect->paddingBottom;
			float handleWidth = rect->getSize().x * scrollArea->getSize().x / scrollRectWidth;
			if (handleWidth > rect->getSize().x) handleWidth = rect->getSize().x;
			handleRect->setWidth(handleWidth);
			Vector2 position(rect->getSize().x * (-scrollRect->getContentBounds().pos.x - scrollRect->offset.x) / scrollRectWidth, handlePadding);
			handleRect->setLocalPosition(position + rect->getRectOffset());
		}
	}
}

void ScrollBar::onMouseDrag(float mouseX, float mouseY) {
	if (!target.refresh()) return;
	if (handleDrag) {
		ScrollRect* scrollRect = target.getComponent<ScrollRect>();
		if (scrollY && scrollRect->scrollableY) {
			RectTransform* scrollArea = target.getComponent<RectTransform>();
			RectTransform* rect = owner.getComponent<RectTransform>();
			float step = ((scrollRect->getContentBounds().size.y + scrollRect->paddingTop + scrollRect->paddingBottom) / rect->getSize().y) / scrollRect->speed;

			onScroll(0, (handlePrevPos.y- mouseY) * step);
		}
		if (!scrollY && scrollRect->scrollableX) {
			RectTransform* scrollArea = target.getComponent<RectTransform>();
			RectTransform* rect = owner.getComponent<RectTransform>();
			float step = ((scrollRect->getContentBounds().size.x + scrollRect->paddingLeft + scrollRect->paddingRight) / rect->getSize().x) / scrollRect->speed;

			onScroll((handlePrevPos.x - mouseX) * step, 0);
		}
		handlePrevPos = Vector2(mouseX, mouseY);
	}
}

void ScrollBar::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	if (!target.refresh()) return;
	if (input->getLastLeftClicked() == handle) {
		handlePrevPos = input->getMousePosition();
		handleDrag = true;
		handle.getComponent<RectSprite>()->setColor(handleColorDragging);
	}
}

void ScrollBar::onMouseButtonReleased(int buttoncode, int mods) {
	if (hovering)
		handle.getComponent<RectSprite>()->setColor(handleColorHighlight);
	else
		handle.getComponent<RectSprite>()->setColor(handleColorDefault);
	handleDrag = false;
}

void ScrollBar::onScroll(float xOffset, float yOffset) {
	if (!target.refresh()) return;
	ScrollRect* scrollRect = target.getComponent<ScrollRect>();
	scrollRect->onScroll(xOffset, yOffset);
}