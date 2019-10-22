#include "DropDown.h"
#include "components/graphics/Image.h"
#include "components/graphics/RectSprite.h"
#include "components/graphics/Sprite.h"
#include "graphics/data/Color.h"
#include "maths/Vector2.h"
#include "maths/MatrixTransform.h"

#include "components/RectTransform.h"
#include "components/graphics/ui/RectButton.h"
#include "components/graphics/Text.h"
#include "input/Input.h"

using namespace Core;

DropDown::DropDown(Text text) : text(text) {
}

DropDown::DropDown() {
}

DropDown::~DropDown() {
}


void DropDown::start() {
	EntityHandle ownerHandle = owner;
	// Create text
	if (!owner.hasComponent<Text>())
		ownerHandle.addComponent(text);

	if (!owner.hasComponent<RectSprite>())
		ownerHandle.addComponent<RectSprite>(Color(80, 80, 80, 255));
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
	EntityHandle ownerHandle = owner;

	// Box
	RectTransform* transform = owner.getComponent<RectTransform>();
	const Vector2& size = transform->getSize();
	Vector2 position = transform->getPosition() + transform->getRectOffset() + Vector2(0, size.y);
	float z = transform->getZ()+1.0f;
	menuBox = createEntity(ownerHandle.getEntityName() + "_DropDownBox",
		RectSprite(Color(20, 20, 20, 255))
	);
	menuBox.setEntityHideFlags(HideFlags::HideInInspector | HideFlags::DontSave);

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
		button.onLeftClick = options[i].function;
		EntityHandle menuOption = createEntity(ownerHandle.getEntityName() + "_DropDownOption_" + std::to_string(i),
			RectTransform((float)boxPaddingX, (float)boxPaddingY + yOffset, optionWidth, optionHeight, z + 0.1f, Alignment::TOP_LEFT),
			button,
			RectSprite({ 20,20,20,255 })
		);
		menuOption.setParent(menuBox.getEntity());

		// Button Text
		Alignment textAlignment = Alignment::LEFT;
		Vector2 textPivot = Anchor(textAlignment);
		float textPaddingX = 5.0f;
		float textPaddingY = 0.0f;
		float textPosX = textPaddingX + (optionWidth  - textPaddingX * 2) * textPivot.x;
		float textPosY = textPaddingY + (optionHeight - textPaddingY * 2) * textPivot.y;
		EntityHandle menuOptionText = createEntity(ownerHandle.getEntityName() + "_DropDownOption_Text_" + std::to_string(i),
			Text(label, optionFont.getFileName(), optionFont.getSize(), optionTextColor),
			RectTransform(textPosX, textPosY, 0, 0, z + 0.1f, textAlignment)
		);
		menuOptionText.setParent(menuOption.getEntity());

		yOffset += optionHeight;
	}
	int boxHeight = (int)(yOffset + boxPaddingY * 2);
	menuBox.addComponent(RectTransform(position.x, position.y, boxWidth, boxHeight, z, Alignment::TOP_LEFT));

	// Box Border
	if (border) {
		// left, bottom, right
		EntityHandle menuBoxBorder = createEntity(ownerHandle.getEntityName() + "_DropDownBox_Border",
			RectSprite(borderColor),
			RectTransform((float)-borderSize, 0.0f, boxWidth + borderSize * 2, boxHeight + borderSize, z - 0.01f, Alignment::TOP_LEFT)
		);
		menuBoxBorder.setParent(menuBox.getEntity());

		// top
		EntityHandle menuBoxBorderTop = createEntity(ownerHandle.getEntityName() + "_DropDownBox_BorderTop",
			RectSprite(borderColor),
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
	destroyEntity(menuBox);
	// Set to Default color
	owner.getComponent<RectSprite>()->setColor(Color(80, 80, 80, 255));
}
void DropDown::addOption(std::string label, ComponentFunctionHandle<void> function) {
	options.push_back({label, function});
}

void DropDown::onDestroy() {
	close();
}

void DropDown::test() {
	std::cout << "Option clicked\n";
}