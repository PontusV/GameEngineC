#pragma once
#include "TypeList.h"
#include "Border.h"
#include "BoxComponent.h"
#include "Button.h"
#include "ChildManager.h"
#include "GraphicComponent.h"
#include "Image.h"
#include "LayoutElement.h"
#include "Panel.h"
#include "ParentEntity.h"
#include "Rect.h"
#include "Text.h"
#include "Transform.h"
namespace Core {
	Core::TypeList<
		Core::Border,
		Core::BoxComponent,
		Core::Button,
		Core::ChildManager,
		Core::GraphicComponent,
		Core::Image,
		Core::LayoutElement,
		Core::Panel,
		Core::ParentEntity,
		Core::Rect,
		Core::Text,
		Core::Transform
	> componentTypeList;
}