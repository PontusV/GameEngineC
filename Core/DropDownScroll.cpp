#include "DropDownScroll.h"
#include "RectSprite.h"
#include "RectButton.h"
#include "ScrollRect.h"
#include "ScrollBar.h"
#include "RectMask.h"
#include "Maths/MatrixTransform.h"
using namespace Core;

DropDownScroll::DropDownScroll() {}
DropDownScroll::DropDownScroll(Text text) : DropDown(text) {}
DropDownScroll::~DropDownScroll() {}

void DropDownScroll::open() {
	if (isOpen) return; // Already open
	isOpen = true;

	// Box
	RectTransform* transform = owner.getComponent<RectTransform>();
	const Vector2& size = transform->getSize();
	Vector2 position = transform->getPosition() + transform->getRectOffset() + Vector2(0, size.y);
	float z = transform->getZ() + 1.0f;
	menuBox = createEntity(owner.getEntityName() + "_DropDownBox",
		RectTransform(position.x, position.y, boxWidth + 20, boxHeight, z, Alignment::TOP_LEFT)
	);
	menuBox.setEntityHideFlags(HideFlags::HideInInspector | HideFlags::DontSave);
	EntityHandle menuBoxContent = createEntity(owner.getEntityName() + "_DropDownBox_Content",
		RectSprite(Color(20, 20, 20, 255)),
		RectMask(),
		RectTransform(0, 0, boxWidth, boxHeight, z, Alignment::TOP_LEFT)
	);
	ScrollRect* scrollRect = menuBoxContent.addComponent<ScrollRect>();
	scrollRect->paddingTop = boxPaddingY;
	scrollRect->paddingBottom = boxPaddingY;
	menuBoxContent.setParent(menuBox);

	// Menu Option settings
	int optionWidth = boxWidth - (int)(boxPaddingX * 2);
	float yOffset = 0;

	for (std::size_t i = 0; i < options.size(); i++) {
		// Button
		RectButton button = RectButton();
		button.colors[RectButton::DEFAULT] = { 20,20,20,255 };
		button.colors[RectButton::PRESSED_DOWN] = { 10,10,10,255 };
		button.colors[RectButton::HOVER_OVER] = { 80,80,80,255 };
		button.clickFunction = options[i].function;
		EntityHandle menuOption = createEntity(owner.getEntityName() + "_DropDownOption_" + std::to_string(i),
			RectTransform((float)boxPaddingX, (float)boxPaddingY + yOffset, optionWidth, optionHeight, z + 0.1f, Alignment::TOP_LEFT),
			button,
			RectSprite({ 20,20,20,255 })
		);
		menuOption.setParent(menuBoxContent.getEntity());

		// Button Text
		Alignment textAlignment = Alignment::LEFT;
		Vector2 textPivot = Anchor(textAlignment);
		float textPaddingX = 5.0f;
		float textPaddingY = 0.0f;
		float textPosX = textPaddingX + (optionWidth - textPaddingX * 2) * textPivot.x;
		float textPosY = textPaddingY + (optionHeight - textPaddingY * 2) * textPivot.y;
		EntityHandle menuOptionText = createEntity(owner.getEntityName() + "_DropDownOption_Text_" + std::to_string(i),
			Text(options[i].text, optionFont.getFileName(), optionFont.getSize(), optionTextColor),
			RectTransform(textPosX, textPosY, 0, 0, z + 0.1f, textAlignment)
		);
		menuOptionText.setParent(menuOption.getEntity());

		yOffset += optionHeight;
	}

	// Box Border
	if (border) {
		// left, bottom, right
		EntityHandle menuBoxBorder = createEntity(owner.getEntityName() + "_DropDownBox_Border",
			RectSprite(borderColor),
			RectTransform((float)-borderSize, 0.0f, boxWidth + 20 + borderSize * 2, boxHeight + borderSize, z - 0.01f, Alignment::TOP_LEFT)
		);
		menuBoxBorder.setParent(menuBox.getEntity());

		// top
		EntityHandle menuBoxBorderTop = createEntity(owner.getEntityName() + "_DropDownBox_BorderTop",
			RectSprite(borderColor),
			RectTransform(size.x, (float)-borderSize, boxWidth + 20 - size.x, borderSize, z - 0.01f, Alignment::TOP_LEFT)
		);
		menuBoxBorderTop.setParent(menuBox.getEntity());
	}

	// Scroll Bar
	EntityHandle scrollBar = createEntity(owner.getEntityName() + "_DropDownBox_Scroll_Bar",
		ScrollBar(menuBoxContent),
		RectTransform(boxWidth, 0, 20, boxHeight, z + 0.2f, Alignment::TOP_LEFT)
	);
	scrollBar.setParent(menuBox);

	// Set to Open color
	owner.getComponent<RectSprite>()->setColor(Color(20, 20, 20, 255));
}