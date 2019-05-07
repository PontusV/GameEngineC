#pragma once
#include "TypeList.h"
#include "Border.h"
#include "BoxComponent.h"
#include "ChildManager.h"
#include "Sprite.h"
#include "Image.h"
#include "Panel.h"
#include "ParentEntity.h"
#include "Rect.h"
#include "Text.h"
#include "Transform.h"
#include "Script.h"
#include "Button.h"
#include "RectButton.h"
#include "DropDown.h"
#include "WindowAlignment.h"
#include "MouseDrag.h"
#include "LayoutController.h"
#include "LayoutElement.h"
#include "HorizontalLayoutGroup.h"
#include "VerticalLayoutGroup.h"
#include "GridLayoutGroup.h"
namespace Core {
	Core::TypeList <
		Core::Border,
		Core::BoxComponent,
		Core::ChildManager,
		Core::Sprite,
		Core::Image,
		Core::Panel,
		Core::ParentEntity,
		Core::Rect,
		Core::Text,
		Core::Transform,
		Core::Script,
		Core::Button,
		Core::RectButton,
		Core::DropDown,
		Core::WindowAlignment,
		Core::MouseDrag,
		Core::LayoutController,
		Core::LayoutElement,
		Core::HorizontalLayoutGroup,
		Core::VerticalLayoutGroup,
		Core::VerticalGridLayoutGroup,
		Core::HorizontalGridLayoutGroup
	> componentTypeList;
}