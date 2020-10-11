#include "CheckBox.h"
#include "components/RectTransform.h"
#include "components/graphics/Image.h"
#include "engine/ResourceManager.h"

using namespace Core;

CheckBox::CheckBox(bool toggle) {
	setToggle(toggle);
}
CheckBox::~CheckBox() {

}


void CheckBox::showTick() {
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (rect) {
		Vector2 position = rect->getRectOffset();
		hideTick(); // Make sure tick is destroyed
		tick = createEntity(owner.getEntityName() + "_Tick",
			Image("resources/images/ui/CheckBox-tick.png", "resources/shaders/figure", Color(0, 0, 0, 255)),
			RectTransform(position.x, position.y, rect->getSize().x, rect->getSize().y, rect->getZ() + 0.0002f, Alignment::TOP_LEFT)
		);
		tick.setParent(owner);
		tick.setEntityHideFlags(HideFlags::HideInInspector | HideFlags::HideInHierarchy);
	}
}

void CheckBox::hideTick() {
	if (tick.refresh()) {
		destroyEntity(tick);
	}
}

void CheckBox::start() {
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (rect) {
		if (toggle) showTick();
		Vector2 position = rect->getRectOffset();
		// Create graphics
		background = createEntity(owner.getEntityName() + "_Background",
			Image("resources/images/ui/CheckBox-background.png", "resources/shaders/figure", Color(255, 255, 255, 255)),
			RectTransform(position.x, position.y, rect->getSize().x, rect->getSize().y, rect->getZ(), Alignment::TOP_LEFT)
		);
		background.setParent(owner);
		background.setEntityHideFlags(HideFlags::HideInInspector | HideFlags::HideInHierarchy);
		border = createEntity(owner.getEntityName() + "_Border",
			Image("resources/images/ui/CheckBox-border.png", "resources/shaders/figure", Color(0, 0, 0, 255)),
			RectTransform(position.x, position.y, rect->getSize().x, rect->getSize().y, rect->getZ()+0.0001f, Alignment::TOP_LEFT)
		);
		border.setParent(owner);
		border.setEntityHideFlags(HideFlags::HideInInspector | HideFlags::HideInHierarchy);
	}
}

void CheckBox::onDestroy() {
	destroyEntity(tick);
	destroyEntity(background);
	destroyEntity(border);
}

void CheckBox::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	setToggle(!toggle);
}

void CheckBox::setToggle(bool value) {
	if (value != toggle) {
		toggle = value;
		onToggle.invoke(value);
		if (value)
			showTick();
		else
			hideTick();
	}
}

bool CheckBox::getToggle() const {
	return toggle;
}