#include "DropDown.h"
#include "Image.h"
#include "Rect.h"
#include "Sprite.h"
#include "Color.h"
#include "Rect.h"
#include <glm/glm.hpp>

#include "Transform.h"
#include "RectButton.h"
#include "Rect.h"
#include "Text.h"
#include "TransformMaths.h"
#include "Input.h"

using namespace Core;

DropDown::DropDown(Text text, int width, int height) : text(text), width(width), height(height), isOpen(false) {
}

DropDown::DropDown() : isOpen(false) {
}

DropDown::~DropDown() {
}


void DropDown::awake() {
	if (!owner.hasComponent<Text>())
		owner.addComponent(text);

	if (!owner.hasComponent<Rect>())
		owner.addComponent<Rect>(width, height, Color(80, 80, 80, 255), text.getLayerIndex());
}

void DropDown::update(float deltaTime) {
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
	Sprite* sprite = owner.getComponent<Rect>();
	if (sprite) {
		sprite->setColor(Color(100, 100, 100, 255));
	}
}

void DropDown::onHoverout() {
	if (isOpen) return;
	Sprite* sprite = owner.getComponent<Rect>();
	if (sprite) {
		sprite->setColor(Color(80, 80, 80, 255));
	}
}

void DropDown::open() {
	if (isOpen) return; // Already open
	isOpen = true;

	// Box
	Transform* transform = owner.getComponent<Transform>();
	glm::vec2 position = transform->getLocalToWorldMatrix() * glm::vec2(-width/2, height/2);
	float z = transform->getZ()+1.0f;
	Transform newTransform(position.x, position.y, z, Alignment::TOP_LEFT);
	menuBox = createEntity(owner.getEntityName() + "_DropDownBox",
		newTransform
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
			Transform((float)boxPaddingX, (float)boxPaddingY + yOffset, z + 0.1f, Alignment::TOP_LEFT),
			button,
			Rect(optionWidth, optionHeight, { 20,20,20,255 }, text.getLayerIndex())
		);
		menuOption.setParent(menuBox.getEntity());

		// Button Text
		EntityHandle menuOptionText = createEntity(owner.getEntityName() + "_DropDownOption_Text_" + std::to_string(i),
			Text(label, optionFont.getFileName(), optionFont.getSize(), optionTextColor, text.getLayerIndex()),
			Transform((float)5, (float)optionHeight/2, z + 0.1f, Alignment::LEFT)
		);
		menuOptionText.setParent(menuOption.getEntity());

		yOffset += optionHeight;
	}
	int boxHeight = (int)(yOffset + boxPaddingY * 2);
	menuBox.addComponent(Rect(boxWidth, boxHeight, Color(20, 20, 20, 255), text.getLayerIndex()));

	// Box Border
	bool border = true;
	int borderSize = 1;
	Color borderColor = Color(100,100,100,255);
	if (border) {
		// left, bottom, right
		EntityHandle menuBoxBorder = createEntity(owner.getEntityName() + "_DropDownBox_Border",
			Rect(boxWidth + borderSize*2, boxHeight + borderSize, borderColor, text.getLayerIndex()),
			Transform((float)-borderSize, 0.0f, z - 0.01f, Alignment::TOP_LEFT)
		);
		menuBoxBorder.setParent(menuBox.getEntity());

		// top
		EntityHandle menuBoxBorderTop = createEntity(owner.getEntityName() + "_DropDownBox_BorderTop",
			Rect(boxWidth - width, borderSize, borderColor, text.getLayerIndex()),
			Transform((float)width, (float)-borderSize, z - 0.01f, Alignment::TOP_LEFT)
		);
		menuBoxBorderTop.setParent(menuBox.getEntity());
	}

	// Set to Open color
	owner.getComponent<Rect>()->setColor(Color(20, 20, 20, 255));
}

void DropDown::close() {
	if (!isOpen) return; // Already closed
	isOpen = false;
	// Close
	menuBox.destroy();
	// Set to Default color
	owner.getComponent<Rect>()->setColor(Color(80, 80, 80, 255));
}
void DropDown::addOption(std::string label, ComponentFunctionHandleWrapper function) {
	options.push_back({label, function});
}

void DropDown::test() {
	std::cout << "Option clicked\n";
}