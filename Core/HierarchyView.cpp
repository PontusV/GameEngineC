#include "HierarchyView.h"
#include "RectTransform.h"
#include "VerticalLayoutGroup.h"
#include "HorizontalLayoutGroup.h"
#include "LayoutElement.h"
#include "RectMask.h"
#include "ScrollRect.h"
#include "ScrollBar.h"
#include "Input.h"
using namespace Core;


HierarchyView::HierarchyView() {}
HierarchyView::~HierarchyView() {}


void HierarchyView::onMouseButtonPressed(int buttoncode, int mods) {
	EntityHandle target = input->getLastClicked();
	if (target.getEntity() != currentTarget.getEntity()) {
		currentTarget = target;
	}
}

void HierarchyView::onEnable() {
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (rect) {
		HorizontalLayoutGroup* parentGroup = owner.getComponent<HorizontalLayoutGroup>();
		parentGroup->childForceExpandHeight = true;
		parentGroup->childForceExpandWidth = true;
		parentGroup->shrinkableChildHeight = true;
		parentGroup->shrinkableChildWidth = true;
		parentGroup->spacing = 10;

		scrollPanel = createEntity("Hierarchy_Scroll_Panel",
			RectMask(),
			RectTransform(0, 0, 0, 0, rect->getZ() + 0.05f, Alignment::TOP_LEFT)
		);
		ScrollRect* scrollRect = scrollPanel.addComponent<ScrollRect>();
		scrollRect->paddingBottom = 10;
		LayoutElement* element = scrollPanel.addComponent<LayoutElement>();
		element->setFlexibleSize(Vector2(1, 1));
		element->setFlexibleSizeEnabled(true);
		element->setMinSize(Vector2(0, 0));
		element->setMinSizeEnabled(true);
		VerticalLayoutGroup* group = scrollPanel.addComponent<VerticalLayoutGroup>();
		group->childForceExpandHeight = false;
		group->childForceExpandWidth = true;
		group->shrinkableChildHeight = false;
		group->shrinkableChildWidth = true;
		group->spacing = 5;
		group->paddingTop = 10;
		group->paddingLeft = 10;
		group->paddingRight = 0;
		scrollPanel.setParent(owner);

		scrollBar = createEntity("Hierarchy_Scroll_Bar",
			ScrollBar(scrollPanel),
			RectTransform(0, 0, 20, 500, rect->getZ() + 10.0f)
		);
		LayoutElement* scrollBarElement = scrollBar.addComponent<LayoutElement>();
		scrollBarElement->setMinSize(Vector2(20, 0));
		scrollBarElement->setMinSizeEnabled(true);
		scrollBarElement->setFlexibleSize(Vector2(0, 1));
		scrollBarElement->setFlexibleSizeEnabled(true);
		scrollBar.setParent(owner);
	}
	// TODO: Create list

}

void HierarchyView::onDisable() {
	destroyEntity(scrollPanel);
	destroyEntity(scrollBar);
}

void HierarchyView::lateUpdate(float deltaTime) {

}