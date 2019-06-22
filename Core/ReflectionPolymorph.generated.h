#ifndef REFLECTION_PARSER
#pragma once
#include <ReflectionTypes.h>
#include <stdexcept>
#include <TypeList.h>

#include "G:/Projects/ProjectGE/Core/Component.h"
#include "G:/Projects/ProjectGE/Core/Transform.h"
#include "G:/Projects/ProjectGE/Core/BoxComponent.h"
#include "G:/Projects/ProjectGE/Core/RectTransform.h"
#include "G:/Projects/ProjectGE/Core/Behaviour.h"
#include "G:/Projects/ProjectGE/Core/Sprite.h"
#include "G:/Projects/ProjectGE/Core/Border.h"
#include "G:/Projects/ProjectGE/Core/Image.h"
#include "G:/Projects/ProjectGE/Core/Button.h"
#include "G:/Projects/ProjectGE/Core/ChildManager.h"
#include "G:/Projects/ProjectGE/Core/ParentEntity.h"
#include "G:/Projects/ProjectGE/Core/UIBehaviour.h"
#include "G:/Projects/ProjectGE/Core/LayoutController.h"
#include "G:/Projects/ProjectGE/Core/ContentSizeFitter.h"
#include "G:/Projects/ProjectGE/Core/Panel.h"
#include "G:/Projects/ProjectGE/Core/RectSprite.h"
#include "G:/Projects/ProjectGE/Core/Text.h"
#include "G:/Projects/ProjectGE/Core/RectButton.h"
#include "G:/Projects/ProjectGE/Core/DropDown.h"
#include "G:/Projects/ProjectGE/Core/WindowAnchor.h"
#include "G:/Projects/ProjectGE/Core/WindowScale.h"
#include "G:/Projects/ProjectGE/Core/MouseDrag.h"
#include "G:/Projects/ProjectGE/Core/LayoutGroup.h"
#include "G:/Projects/ProjectGE/Core/LayoutElement.h"
#include "G:/Projects/ProjectGE/Core/HorizontalLayoutGroup.h"
#include "G:/Projects/ProjectGE/Core/VerticalLayoutGroup.h"
#include "G:/Projects/ProjectGE/Core/GridLayoutGroup.h"
#include "G:/Projects/ProjectGE/Core/DragAndResize.h"
#include "G:/Projects/ProjectGE/Core/Inspector.h"

namespace Mirror {
/* Converts the given pointer to the derived type. Calls invoke on the function instance with the casted pointer. */
template<typename ClassType, typename... Args>
void polyInvoke(Mirror::Function fun, ClassType* instance, Args... args) {
	if (instance->getType().name == "Core::Component")
		return fun.invoke(reinterpret_cast<Core::Component*>(instance), args...);
	else if (instance->getType().name == "Core::Transform")
		return fun.invoke(reinterpret_cast<Core::Transform*>(instance), args...);
	else if (instance->getType().name == "Core::BoxComponent")
		return fun.invoke(reinterpret_cast<Core::BoxComponent*>(instance), args...);
	else if (instance->getType().name == "Core::RectTransform")
		return fun.invoke(reinterpret_cast<Core::RectTransform*>(instance), args...);
	else if (instance->getType().name == "Core::Behaviour")
		return fun.invoke(reinterpret_cast<Core::Behaviour*>(instance), args...);
	else if (instance->getType().name == "Core::Sprite")
		return fun.invoke(reinterpret_cast<Core::Sprite*>(instance), args...);
	else if (instance->getType().name == "Core::Border")
		return fun.invoke(reinterpret_cast<Core::Border*>(instance), args...);
	else if (instance->getType().name == "Core::Image")
		return fun.invoke(reinterpret_cast<Core::Image*>(instance), args...);
	else if (instance->getType().name == "Core::Button")
		return fun.invoke(reinterpret_cast<Core::Button*>(instance), args...);
	else if (instance->getType().name == "Core::ChildManager")
		return fun.invoke(reinterpret_cast<Core::ChildManager*>(instance), args...);
	else if (instance->getType().name == "Core::ParentEntity")
		return fun.invoke(reinterpret_cast<Core::ParentEntity*>(instance), args...);
	else if (instance->getType().name == "Core::UIBehaviour")
		return fun.invoke(reinterpret_cast<Core::UIBehaviour*>(instance), args...);
	else if (instance->getType().name == "Core::LayoutController")
		return fun.invoke(reinterpret_cast<Core::LayoutController*>(instance), args...);
	else if (instance->getType().name == "Core::ContentSizeFitter")
		return fun.invoke(reinterpret_cast<Core::ContentSizeFitter*>(instance), args...);
	else if (instance->getType().name == "Core::Panel")
		return fun.invoke(reinterpret_cast<Core::Panel*>(instance), args...);
	else if (instance->getType().name == "Core::RectSprite")
		return fun.invoke(reinterpret_cast<Core::RectSprite*>(instance), args...);
	else if (instance->getType().name == "Core::Text")
		return fun.invoke(reinterpret_cast<Core::Text*>(instance), args...);
	else if (instance->getType().name == "Core::RectButton")
		return fun.invoke(reinterpret_cast<Core::RectButton*>(instance), args...);
	else if (instance->getType().name == "Core::DropDown")
		return fun.invoke(reinterpret_cast<Core::DropDown*>(instance), args...);
	else if (instance->getType().name == "Core::WindowAnchor")
		return fun.invoke(reinterpret_cast<Core::WindowAnchor*>(instance), args...);
	else if (instance->getType().name == "Core::WindowScale")
		return fun.invoke(reinterpret_cast<Core::WindowScale*>(instance), args...);
	else if (instance->getType().name == "Core::MouseDrag")
		return fun.invoke(reinterpret_cast<Core::MouseDrag*>(instance), args...);
	else if (instance->getType().name == "Core::LayoutGroup")
		return fun.invoke(reinterpret_cast<Core::LayoutGroup*>(instance), args...);
	else if (instance->getType().name == "Core::LayoutElement")
		return fun.invoke(reinterpret_cast<Core::LayoutElement*>(instance), args...);
	else if (instance->getType().name == "Core::HorizontalLayoutGroup")
		return fun.invoke(reinterpret_cast<Core::HorizontalLayoutGroup*>(instance), args...);
	else if (instance->getType().name == "Core::VerticalLayoutGroup")
		return fun.invoke(reinterpret_cast<Core::VerticalLayoutGroup*>(instance), args...);
	else if (instance->getType().name == "Core::GridLayoutGroup")
		return fun.invoke(reinterpret_cast<Core::GridLayoutGroup*>(instance), args...);
	else if (instance->getType().name == "Core::DragAndResize")
		return fun.invoke(reinterpret_cast<Core::DragAndResize*>(instance), args...);
	else if (instance->getType().name == "Core::Inspector")
		return fun.invoke(reinterpret_cast<Core::Inspector*>(instance), args...);
	throw std::invalid_argument("polyInvoke::ERROR");
}
template<typename T, typename ClassType>
T polyGetValue(Mirror::Property prop, ClassType* instance) {
	if (instance->getType().name == "Core::Component")
		return prop.getValue<T>(reinterpret_cast<Core::Component*>(instance));
	else if (instance->getType().name == "Core::Transform")
		return prop.getValue<T>(reinterpret_cast<Core::Transform*>(instance));
	else if (instance->getType().name == "Core::BoxComponent")
		return prop.getValue<T>(reinterpret_cast<Core::BoxComponent*>(instance));
	else if (instance->getType().name == "Core::RectTransform")
		return prop.getValue<T>(reinterpret_cast<Core::RectTransform*>(instance));
	else if (instance->getType().name == "Core::Behaviour")
		return prop.getValue<T>(reinterpret_cast<Core::Behaviour*>(instance));
	else if (instance->getType().name == "Core::Sprite")
		return prop.getValue<T>(reinterpret_cast<Core::Sprite*>(instance));
	else if (instance->getType().name == "Core::Border")
		return prop.getValue<T>(reinterpret_cast<Core::Border*>(instance));
	else if (instance->getType().name == "Core::Image")
		return prop.getValue<T>(reinterpret_cast<Core::Image*>(instance));
	else if (instance->getType().name == "Core::Button")
		return prop.getValue<T>(reinterpret_cast<Core::Button*>(instance));
	else if (instance->getType().name == "Core::ChildManager")
		return prop.getValue<T>(reinterpret_cast<Core::ChildManager*>(instance));
	else if (instance->getType().name == "Core::ParentEntity")
		return prop.getValue<T>(reinterpret_cast<Core::ParentEntity*>(instance));
	else if (instance->getType().name == "Core::UIBehaviour")
		return prop.getValue<T>(reinterpret_cast<Core::UIBehaviour*>(instance));
	else if (instance->getType().name == "Core::LayoutController")
		return prop.getValue<T>(reinterpret_cast<Core::LayoutController*>(instance));
	else if (instance->getType().name == "Core::ContentSizeFitter")
		return prop.getValue<T>(reinterpret_cast<Core::ContentSizeFitter*>(instance));
	else if (instance->getType().name == "Core::Panel")
		return prop.getValue<T>(reinterpret_cast<Core::Panel*>(instance));
	else if (instance->getType().name == "Core::RectSprite")
		return prop.getValue<T>(reinterpret_cast<Core::RectSprite*>(instance));
	else if (instance->getType().name == "Core::Text")
		return prop.getValue<T>(reinterpret_cast<Core::Text*>(instance));
	else if (instance->getType().name == "Core::RectButton")
		return prop.getValue<T>(reinterpret_cast<Core::RectButton*>(instance));
	else if (instance->getType().name == "Core::DropDown")
		return prop.getValue<T>(reinterpret_cast<Core::DropDown*>(instance));
	else if (instance->getType().name == "Core::WindowAnchor")
		return prop.getValue<T>(reinterpret_cast<Core::WindowAnchor*>(instance));
	else if (instance->getType().name == "Core::WindowScale")
		return prop.getValue<T>(reinterpret_cast<Core::WindowScale*>(instance));
	else if (instance->getType().name == "Core::MouseDrag")
		return prop.getValue<T>(reinterpret_cast<Core::MouseDrag*>(instance));
	else if (instance->getType().name == "Core::LayoutGroup")
		return prop.getValue<T>(reinterpret_cast<Core::LayoutGroup*>(instance));
	else if (instance->getType().name == "Core::LayoutElement")
		return prop.getValue<T>(reinterpret_cast<Core::LayoutElement*>(instance));
	else if (instance->getType().name == "Core::HorizontalLayoutGroup")
		return prop.getValue<T>(reinterpret_cast<Core::HorizontalLayoutGroup*>(instance));
	else if (instance->getType().name == "Core::VerticalLayoutGroup")
		return prop.getValue<T>(reinterpret_cast<Core::VerticalLayoutGroup*>(instance));
	else if (instance->getType().name == "Core::GridLayoutGroup")
		return prop.getValue<T>(reinterpret_cast<Core::GridLayoutGroup*>(instance));
	else if (instance->getType().name == "Core::DragAndResize")
		return prop.getValue<T>(reinterpret_cast<Core::DragAndResize*>(instance));
	else if (instance->getType().name == "Core::Inspector")
		return prop.getValue<T>(reinterpret_cast<Core::Inspector*>(instance));
	throw std::invalid_argument("polyGetValue::ERROR");
}
template<typename T, std::size_t N, typename ClassType>
std::array<T, N> polyGetArrayValue(Mirror::Property prop, ClassType* instance) {
	if (instance->getType().name == "Core::Component")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::Component*>(instance));
	else if (instance->getType().name == "Core::Transform")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::Transform*>(instance));
	else if (instance->getType().name == "Core::BoxComponent")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::BoxComponent*>(instance));
	else if (instance->getType().name == "Core::RectTransform")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::RectTransform*>(instance));
	else if (instance->getType().name == "Core::Behaviour")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::Behaviour*>(instance));
	else if (instance->getType().name == "Core::Sprite")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::Sprite*>(instance));
	else if (instance->getType().name == "Core::Border")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::Border*>(instance));
	else if (instance->getType().name == "Core::Image")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::Image*>(instance));
	else if (instance->getType().name == "Core::Button")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::Button*>(instance));
	else if (instance->getType().name == "Core::ChildManager")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::ChildManager*>(instance));
	else if (instance->getType().name == "Core::ParentEntity")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::ParentEntity*>(instance));
	else if (instance->getType().name == "Core::UIBehaviour")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::UIBehaviour*>(instance));
	else if (instance->getType().name == "Core::LayoutController")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::LayoutController*>(instance));
	else if (instance->getType().name == "Core::ContentSizeFitter")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::ContentSizeFitter*>(instance));
	else if (instance->getType().name == "Core::Panel")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::Panel*>(instance));
	else if (instance->getType().name == "Core::RectSprite")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::RectSprite*>(instance));
	else if (instance->getType().name == "Core::Text")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::Text*>(instance));
	else if (instance->getType().name == "Core::RectButton")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::RectButton*>(instance));
	else if (instance->getType().name == "Core::DropDown")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::DropDown*>(instance));
	else if (instance->getType().name == "Core::WindowAnchor")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::WindowAnchor*>(instance));
	else if (instance->getType().name == "Core::WindowScale")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::WindowScale*>(instance));
	else if (instance->getType().name == "Core::MouseDrag")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::MouseDrag*>(instance));
	else if (instance->getType().name == "Core::LayoutGroup")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::LayoutGroup*>(instance));
	else if (instance->getType().name == "Core::LayoutElement")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::LayoutElement*>(instance));
	else if (instance->getType().name == "Core::HorizontalLayoutGroup")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::HorizontalLayoutGroup*>(instance));
	else if (instance->getType().name == "Core::VerticalLayoutGroup")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::VerticalLayoutGroup*>(instance));
	else if (instance->getType().name == "Core::GridLayoutGroup")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::GridLayoutGroup*>(instance));
	else if (instance->getType().name == "Core::DragAndResize")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::DragAndResize*>(instance));
	else if (instance->getType().name == "Core::Inspector")
		return prop.getArrayValue<T, N>(reinterpret_cast<Core::Inspector*>(instance));
	throw std::invalid_argument("polyGetArrayValue::ERROR");
}
template<typename T, typename ClassType>
void polySetValue(Mirror::Property prop, ClassType* instance, T value) {
	if (instance->getType().name == "Core::Component")
		return prop.setValue(reinterpret_cast<Core::Component*>(instance), value);
	else if (instance->getType().name == "Core::Transform")
		return prop.setValue(reinterpret_cast<Core::Transform*>(instance), value);
	else if (instance->getType().name == "Core::BoxComponent")
		return prop.setValue(reinterpret_cast<Core::BoxComponent*>(instance), value);
	else if (instance->getType().name == "Core::RectTransform")
		return prop.setValue(reinterpret_cast<Core::RectTransform*>(instance), value);
	else if (instance->getType().name == "Core::Behaviour")
		return prop.setValue(reinterpret_cast<Core::Behaviour*>(instance), value);
	else if (instance->getType().name == "Core::Sprite")
		return prop.setValue(reinterpret_cast<Core::Sprite*>(instance), value);
	else if (instance->getType().name == "Core::Border")
		return prop.setValue(reinterpret_cast<Core::Border*>(instance), value);
	else if (instance->getType().name == "Core::Image")
		return prop.setValue(reinterpret_cast<Core::Image*>(instance), value);
	else if (instance->getType().name == "Core::Button")
		return prop.setValue(reinterpret_cast<Core::Button*>(instance), value);
	else if (instance->getType().name == "Core::ChildManager")
		return prop.setValue(reinterpret_cast<Core::ChildManager*>(instance), value);
	else if (instance->getType().name == "Core::ParentEntity")
		return prop.setValue(reinterpret_cast<Core::ParentEntity*>(instance), value);
	else if (instance->getType().name == "Core::UIBehaviour")
		return prop.setValue(reinterpret_cast<Core::UIBehaviour*>(instance), value);
	else if (instance->getType().name == "Core::LayoutController")
		return prop.setValue(reinterpret_cast<Core::LayoutController*>(instance), value);
	else if (instance->getType().name == "Core::ContentSizeFitter")
		return prop.setValue(reinterpret_cast<Core::ContentSizeFitter*>(instance), value);
	else if (instance->getType().name == "Core::Panel")
		return prop.setValue(reinterpret_cast<Core::Panel*>(instance), value);
	else if (instance->getType().name == "Core::RectSprite")
		return prop.setValue(reinterpret_cast<Core::RectSprite*>(instance), value);
	else if (instance->getType().name == "Core::Text")
		return prop.setValue(reinterpret_cast<Core::Text*>(instance), value);
	else if (instance->getType().name == "Core::RectButton")
		return prop.setValue(reinterpret_cast<Core::RectButton*>(instance), value);
	else if (instance->getType().name == "Core::DropDown")
		return prop.setValue(reinterpret_cast<Core::DropDown*>(instance), value);
	else if (instance->getType().name == "Core::WindowAnchor")
		return prop.setValue(reinterpret_cast<Core::WindowAnchor*>(instance), value);
	else if (instance->getType().name == "Core::WindowScale")
		return prop.setValue(reinterpret_cast<Core::WindowScale*>(instance), value);
	else if (instance->getType().name == "Core::MouseDrag")
		return prop.setValue(reinterpret_cast<Core::MouseDrag*>(instance), value);
	else if (instance->getType().name == "Core::LayoutGroup")
		return prop.setValue(reinterpret_cast<Core::LayoutGroup*>(instance), value);
	else if (instance->getType().name == "Core::LayoutElement")
		return prop.setValue(reinterpret_cast<Core::LayoutElement*>(instance), value);
	else if (instance->getType().name == "Core::HorizontalLayoutGroup")
		return prop.setValue(reinterpret_cast<Core::HorizontalLayoutGroup*>(instance), value);
	else if (instance->getType().name == "Core::VerticalLayoutGroup")
		return prop.setValue(reinterpret_cast<Core::VerticalLayoutGroup*>(instance), value);
	else if (instance->getType().name == "Core::GridLayoutGroup")
		return prop.setValue(reinterpret_cast<Core::GridLayoutGroup*>(instance), value);
	else if (instance->getType().name == "Core::DragAndResize")
		return prop.setValue(reinterpret_cast<Core::DragAndResize*>(instance), value);
	else if (instance->getType().name == "Core::Inspector")
		return prop.setValue(reinterpret_cast<Core::Inspector*>(instance), value);
	throw std::invalid_argument("polySetValue::ERROR");
}
template<typename T, std::size_t N, typename ClassType>
void polySetArrayValue(Mirror::Property prop, ClassType* instance, T(&value)[N]) {
	if (instance->getType().name == "Core::Component")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::Component*>(instance), value);
	else if (instance->getType().name == "Core::Transform")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::Transform*>(instance), value);
	else if (instance->getType().name == "Core::BoxComponent")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::BoxComponent*>(instance), value);
	else if (instance->getType().name == "Core::RectTransform")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::RectTransform*>(instance), value);
	else if (instance->getType().name == "Core::Behaviour")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::Behaviour*>(instance), value);
	else if (instance->getType().name == "Core::Sprite")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::Sprite*>(instance), value);
	else if (instance->getType().name == "Core::Border")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::Border*>(instance), value);
	else if (instance->getType().name == "Core::Image")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::Image*>(instance), value);
	else if (instance->getType().name == "Core::Button")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::Button*>(instance), value);
	else if (instance->getType().name == "Core::ChildManager")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::ChildManager*>(instance), value);
	else if (instance->getType().name == "Core::ParentEntity")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::ParentEntity*>(instance), value);
	else if (instance->getType().name == "Core::UIBehaviour")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::UIBehaviour*>(instance), value);
	else if (instance->getType().name == "Core::LayoutController")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::LayoutController*>(instance), value);
	else if (instance->getType().name == "Core::ContentSizeFitter")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::ContentSizeFitter*>(instance), value);
	else if (instance->getType().name == "Core::Panel")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::Panel*>(instance), value);
	else if (instance->getType().name == "Core::RectSprite")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::RectSprite*>(instance), value);
	else if (instance->getType().name == "Core::Text")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::Text*>(instance), value);
	else if (instance->getType().name == "Core::RectButton")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::RectButton*>(instance), value);
	else if (instance->getType().name == "Core::DropDown")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::DropDown*>(instance), value);
	else if (instance->getType().name == "Core::WindowAnchor")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::WindowAnchor*>(instance), value);
	else if (instance->getType().name == "Core::WindowScale")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::WindowScale*>(instance), value);
	else if (instance->getType().name == "Core::MouseDrag")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::MouseDrag*>(instance), value);
	else if (instance->getType().name == "Core::LayoutGroup")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::LayoutGroup*>(instance), value);
	else if (instance->getType().name == "Core::LayoutElement")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::LayoutElement*>(instance), value);
	else if (instance->getType().name == "Core::HorizontalLayoutGroup")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::HorizontalLayoutGroup*>(instance), value);
	else if (instance->getType().name == "Core::VerticalLayoutGroup")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::VerticalLayoutGroup*>(instance), value);
	else if (instance->getType().name == "Core::GridLayoutGroup")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::GridLayoutGroup*>(instance), value);
	else if (instance->getType().name == "Core::DragAndResize")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::DragAndResize*>(instance), value);
	else if (instance->getType().name == "Core::Inspector")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::Inspector*>(instance), value);
	throw std::invalid_argument("polySetArrayValue::ERROR");
}
typedef TypeList<Core::Component,Core::Transform,Core::BoxComponent,Core::RectTransform,Core::Behaviour,Core::Sprite,Core::Border,Core::Image,Core::Button,Core::ChildManager,Core::ParentEntity,Core::UIBehaviour,Core::LayoutController,Core::ContentSizeFitter,Core::Panel,Core::RectSprite,Core::Text,Core::RectButton,Core::DropDown,Core::WindowAnchor,Core::WindowScale,Core::MouseDrag,Core::LayoutGroup,Core::LayoutElement,Core::HorizontalLayoutGroup,Core::VerticalLayoutGroup,Core::GridLayoutGroup,Core::DragAndResize,Core::Inspector> ReflectedTypes;
}

#endif