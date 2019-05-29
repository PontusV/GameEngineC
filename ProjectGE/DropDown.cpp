#include "DropDown.h"
#include "Image.h"
#include "RectSprite.h"
#include "Sprite.h"
#include "Color.h"
#include <glm/glm.hpp>

#include "RectTransform.h"
#include "RectButton.h"
#include "Text.h"
#include "TransformMaths.h"
#include "Input.h"

using namespace Core;

DropDown::DropDown(Text text) : text(text) {
}

DropDown::DropDown() {
}

DropDown::~DropDown() {
}


void DropDown::awake() {
	// Create text
	if (!owner.hasComponent<Text>())
		owner.addComponent(text);

	if (!owner.hasComponent<RectSprite>())
		owner.addComponent<RectSprite>(Color(80, 80, 80, 255), text.getLayerIndex());
}

void DropDown::onMouseButtonPressedAsButton(int buttoncode, int mods) {
	// Toggle Open/Close DropDown
	if (!isOpen) {
		open();
	}
	else {
		close();
	}
}
void DropDown::onMouseButtonPressed(int buttoncode, int mods) {
	if (isOpen) {
		Entity mouseTarget = input->getHoverTarget().getEntity();
		if (mouseTarget != owner.getEntity() && mouseTarget != menuBox.getEntity() && !menuBox.isChild(mouseTarget)) {
			close();
		}
	}
}
void DropDown::onMouseButtonReleased(int buttoncode, int mods) {
	if (isOpen) {
		Entity mouseTarget = input->getHoverTarget().getEntity();
		if (mouseTarget != owner.getEntity() && mouseTarget != menuBox.getEntity() && !menuBox.isChild(mouseTarget)) {
			close();
		}
	}
}

void DropDown::onHoverover() {
	if (isOpen) return;
	Sprite* sprite = owner.getComponent<RectSprite>();
	if (sprite) {
		sprite->setColor(Color(100, 100, 100, 255));
	}
}

void DropDown::onHoverout() {
	if (isOpen) return;
	Sprite* sprite = owner.getComponent<RectSprite>();
	if (sprite) {
		sprite->setColor(Color(80, 80, 80, 255));
	}
}

void DropDown::open() {
	if (isOpen) return; // Already open
	isOpen = true;

	// Box
	RectTransform* transform = owner.getComponent<RectTransform>();
	const glm::ivec2& size = transform->getSize();
	glm::vec2 position = transform->getLocalToWorldMatrix() * (transform->getPosition() + glm::vec2(-size.x/2, size.y/2));
	float z = transform->getZ()+1.0f;
	menuBox = createEntity(owner.getEntityName() + "_DropDownBox",
		RectSprite(Color(20, 20, 20, 255), text.getLayerIndex())
	);
	// Text Alignment? Left-Middle by default

	// Menu Option settings
	int optionWidth = boxWidth - (int)(boxPaddingX*2);
	float yOffset = 0;

	for (std::size_t i = 0; i < options.size(); i++) {
		std::string label = options[i].text;
		// Button
		RectButton button = RectButton();
		button.colors[RectButton::DEFAULT] = { 20,20,20,255 };
		button.colors[RectButton::PRESSED_DOWN] = { 10,10,10,255 };
		button.colors[RectButton::HOVER_OVER] = { 80,80,80,255 };
		button.clickFunction = options[i].function;
		EntityHandle menuOption = createEntity(owner.getEntityName() + "_DropDownOption_" + std::to_string(i),
			RectTransform((float)boxPaddingX, (float)boxPaddingY + yOffset, optionWidth, optionHeight, z + 0.1f, Alignment::TOP_LEFT),
			button,
			RectSprite({ 20,20,20,255 }, text.getLayerIndex())
		);
		menuOption.setParent(menuBox.getEntity());

		// Button Text
		EntityHandle menuOptionText = createEntity(owner.getEntityName() + "_DropDownOption_Text_" + std::to_string(i),
			Text(label, optionFont.getFileName(), optionFont.getSize(), optionTextColor, text.getLayerIndex()),
			RectTransform((float)5, (float)optionHeight/2, 0, 0, z + 0.1f, Alignment::LEFT)
		);
		menuOptionText.setParent(menuOption.getEntity());

		yOffset += optionHeight;
	}
	int boxHeight = (int)(yOffset + boxPaddingY * 2);
	menuBox.addComponent(RectTransform(position.x, position.y, boxWidth, boxHeight, z, Alignment::TOP_LEFT));

	// Box Border
	bool border = true;
	int borderSize = 1;
	Color borderColor = Color(100,100,100,255);
	if (border) {
		// left, bottom, right
		EntityHandle menuBoxBorder = createEntity(owner.getEntityName() + "_DropDownBox_Border",
			RectSprite(borderColor, text.getLayerIndex()),
			RectTransform((float)-borderSize, 0.0f, boxWidth + borderSize * 2, boxHeight + borderSize, z - 0.01f, Alignment::TOP_LEFT)
		);
		menuBoxBorder.setParent(menuBox.getEntity());

		// top
		EntityHandle menuBoxBorderTop = createEntity(owner.getEntityName() + "_DropDownBox_BorderTop",
			RectSprite(borderColor, text.getLayerIndex()),
			RectTransform((float)size.x, (float)-borderSize, boxWidth - size.x, borderSize, z - 0.01f, Alignment::TOP_LEFT)
		);
		menuBoxBorderTop.setParent(menuBox.getEntity());
	}

	// Set to Open color
	owner.getComponent<RectSprite>()->setColor(Color(20, 20, 20, 255));
}

void DropDown::close() {
	if (!isOpen) return; // Already closed
	isOpen = false;
	// Close
	menuBox.destroy();
	// Set to Default color
	owner.getComponent<RectSprite>()->setColor(Color(80, 80, 80, 255));
}
void DropDown::addOption(std::string label, ComponentFunctionHandleWrapper function) {
	options.push_back({label, function});
}

void DropDown::test() {
	std::cout << "Option clicked\n";
}