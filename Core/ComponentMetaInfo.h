#pragma once
#include "TypeList.h"
#include "Border.h"
#include "BoxComponent.h"
#include "ChildManager.h"
#include "Sprite.h"
#include "Image.h"
#include "Panel.h"
#include "ParentEntity.h"
#include "RectSprite.h"
#include "Text.h"
#include "Transform.h"
#include "RectTransform.h"
#include "Behaviour.h"
#include "UIBehaviour.h"
#include "Button.h"
#include "RectButton.h"
#include "DropDown.h"
#include "WindowAnchor.h"
#include "WindowScale.h"
#include "MouseDrag.h"
#include "LayoutController.h"
#include "LayoutGroup.h"
#include "ILayoutElement.h"
#include "LayoutElement.h"
#include "HorizontalLayoutGroup.h"
#include "VerticalLayoutGroup.h"
#include "GridLayoutGroup.h"
#include "ContentSizeFitter.h"
#include "DragAndResize.h"
#include "Inspector.h"
namespace Core {
	Core::TypeList <
		Core::Border,
		Core::BoxComponent,
		Core::ChildManager,
		Core::Sprite,
		Core::Image,
		Core::Panel,
		Core::ParentEntity,
		Core::RectSprite,
		Core::Text,
		Core::Transform,
		Core::RectTransform,
		Core::Behaviour,
		Core::UIBehaviour,
		Core::Button,
		Core::RectButton,
		Core::DropDown,
		Core::WindowAnchor,
		Core::WindowScale,
		Core::MouseDrag,
		Core::LayoutController,
		Core::LayoutGroup,
		Core::ILayoutElement,
		Core::LayoutElement,
		Core::HorizontalLayoutGroup,
		Core::VerticalLayoutGroup,
		Core::GridLayoutGroup,
		Core::ContentSizeFitter,
		Core::DragAndResize,
		Core::Inspector
	> componentTypeList;
}