#ifndef REFLECTION_PARSER
#pragma once
#include <ReflectionParser/ReflectionTypes.h>
#include <ReflectionParser/TypeList.h>
#include <stdexcept>

#include "G:/Projects/ProjectGE/Core/Component.h"
#include "G:/Projects/ProjectGE/Core/Transform.h"
#include "G:/Projects/ProjectGE/Core/BoxComponent.h"
#include "G:/Projects/ProjectGE/Core/RectTransform.h"
#include "G:/Projects/ProjectGE/Core/Behaviour.h"
#include "G:/Projects/ProjectGE/Core/Sprite.h"
#include "G:/Projects/ProjectGE/Core/Border.h"
#include "G:/Projects/ProjectGE/Core/Image.h"
#include "G:/Projects/ProjectGE/Core/Button.h"
#include "G:/Projects/ProjectGE/Core/CheckBox.h"
#include "G:/Projects/ProjectGE/Core/ChildManager.h"
#include "G:/Projects/ProjectGE/Core/ParentEntity.h"
#include "G:/Projects/ProjectGE/Core/UIBehaviour.h"
#include "G:/Projects/ProjectGE/Core/LayoutController.h"
#include "G:/Projects/ProjectGE/Core/ContentSizeFitter.h"
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
#include "G:/Projects/ProjectGE/Core/Selectable.h"
#include "G:/Projects/ProjectGE/Core/InputField.h"
#include "G:/Projects/ProjectGE/Core/PropertyField.h"
#include "G:/Projects/ProjectGE/Core/RectMask.h"

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
	else if (instance->getType().name == "Core::CheckBox")
		return fun.invoke(reinterpret_cast<Core::CheckBox*>(instance), args...);
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
	else if (instance->getType().name == "Core::Selectable")
		return fun.invoke(reinterpret_cast<Core::Selectable*>(instance), args...);
	else if (instance->getType().name == "Core::InputField")
		return fun.invoke(reinterpret_cast<Core::InputField*>(instance), args...);
	else if (instance->getType().name == "Core::PropertyField")
		return fun.invoke(reinterpret_cast<Core::PropertyField*>(instance), args...);
	else if (instance->getType().name == "Core::RectMask")
		return fun.invoke(reinterpret_cast<Core::RectMask*>(instance), args...);
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
	else if (instance->getType().name == "Core::CheckBox")
		return prop.getValue<T>(reinterpret_cast<Core::CheckBox*>(instance));
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
	else if (instance->getType().name == "Core::Selectable")
		return prop.getValue<T>(reinterpret_cast<Core::Selectable*>(instance));
	else if (instance->getType().name == "Core::InputField")
		return prop.getValue<T>(reinterpret_cast<Core::InputField*>(instance));
	else if (instance->getType().name == "Core::PropertyField")
		return prop.getValue<T>(reinterpret_cast<Core::PropertyField*>(instance));
	else if (instance->getType().name == "Core::RectMask")
		return prop.getValue<T>(reinterpret_cast<Core::RectMask*>(instance));
	throw std::invalid_argument("polyGetValue::ERROR");
}
template<typename T, typename ClassType>
std::vector<T> polyGetArrayValue(Mirror::Property prop, ClassType* instance) {
	if (instance->getType().name == "Core::Component")
		return prop.getArrayValue<T>(reinterpret_cast<Core::Component*>(instance));
	else if (instance->getType().name == "Core::Transform")
		return prop.getArrayValue<T>(reinterpret_cast<Core::Transform*>(instance));
	else if (instance->getType().name == "Core::BoxComponent")
		return prop.getArrayValue<T>(reinterpret_cast<Core::BoxComponent*>(instance));
	else if (instance->getType().name == "Core::RectTransform")
		return prop.getArrayValue<T>(reinterpret_cast<Core::RectTransform*>(instance));
	else if (instance->getType().name == "Core::Behaviour")
		return prop.getArrayValue<T>(reinterpret_cast<Core::Behaviour*>(instance));
	else if (instance->getType().name == "Core::Sprite")
		return prop.getArrayValue<T>(reinterpret_cast<Core::Sprite*>(instance));
	else if (instance->getType().name == "Core::Border")
		return prop.getArrayValue<T>(reinterpret_cast<Core::Border*>(instance));
	else if (instance->getType().name == "Core::Image")
		return prop.getArrayValue<T>(reinterpret_cast<Core::Image*>(instance));
	else if (instance->getType().name == "Core::Button")
		return prop.getArrayValue<T>(reinterpret_cast<Core::Button*>(instance));
	else if (instance->getType().name == "Core::CheckBox")
		return prop.getArrayValue<T>(reinterpret_cast<Core::CheckBox*>(instance));
	else if (instance->getType().name == "Core::ChildManager")
		return prop.getArrayValue<T>(reinterpret_cast<Core::ChildManager*>(instance));
	else if (instance->getType().name == "Core::ParentEntity")
		return prop.getArrayValue<T>(reinterpret_cast<Core::ParentEntity*>(instance));
	else if (instance->getType().name == "Core::UIBehaviour")
		return prop.getArrayValue<T>(reinterpret_cast<Core::UIBehaviour*>(instance));
	else if (instance->getType().name == "Core::LayoutController")
		return prop.getArrayValue<T>(reinterpret_cast<Core::LayoutController*>(instance));
	else if (instance->getType().name == "Core::ContentSizeFitter")
		return prop.getArrayValue<T>(reinterpret_cast<Core::ContentSizeFitter*>(instance));
	else if (instance->getType().name == "Core::RectSprite")
		return prop.getArrayValue<T>(reinterpret_cast<Core::RectSprite*>(instance));
	else if (instance->getType().name == "Core::Text")
		return prop.getArrayValue<T>(reinterpret_cast<Core::Text*>(instance));
	else if (instance->getType().name == "Core::RectButton")
		return prop.getArrayValue<T>(reinterpret_cast<Core::RectButton*>(instance));
	else if (instance->getType().name == "Core::DropDown")
		return prop.getArrayValue<T>(reinterpret_cast<Core::DropDown*>(instance));
	else if (instance->getType().name == "Core::WindowAnchor")
		return prop.getArrayValue<T>(reinterpret_cast<Core::WindowAnchor*>(instance));
	else if (instance->getType().name == "Core::WindowScale")
		return prop.getArrayValue<T>(reinterpret_cast<Core::WindowScale*>(instance));
	else if (instance->getType().name == "Core::MouseDrag")
		return prop.getArrayValue<T>(reinterpret_cast<Core::MouseDrag*>(instance));
	else if (instance->getType().name == "Core::LayoutGroup")
		return prop.getArrayValue<T>(reinterpret_cast<Core::LayoutGroup*>(instance));
	else if (instance->getType().name == "Core::LayoutElement")
		return prop.getArrayValue<T>(reinterpret_cast<Core::LayoutElement*>(instance));
	else if (instance->getType().name == "Core::HorizontalLayoutGroup")
		return prop.getArrayValue<T>(reinterpret_cast<Core::HorizontalLayoutGroup*>(instance));
	else if (instance->getType().name == "Core::VerticalLayoutGroup")
		return prop.getArrayValue<T>(reinterpret_cast<Core::VerticalLayoutGroup*>(instance));
	else if (instance->getType().name == "Core::GridLayoutGroup")
		return prop.getArrayValue<T>(reinterpret_cast<Core::GridLayoutGroup*>(instance));
	else if (instance->getType().name == "Core::DragAndResize")
		return prop.getArrayValue<T>(reinterpret_cast<Core::DragAndResize*>(instance));
	else if (instance->getType().name == "Core::Inspector")
		return prop.getArrayValue<T>(reinterpret_cast<Core::Inspector*>(instance));
	else if (instance->getType().name == "Core::Selectable")
		return prop.getArrayValue<T>(reinterpret_cast<Core::Selectable*>(instance));
	else if (instance->getType().name == "Core::InputField")
		return prop.getArrayValue<T>(reinterpret_cast<Core::InputField*>(instance));
	else if (instance->getType().name == "Core::PropertyField")
		return prop.getArrayValue<T>(reinterpret_cast<Core::PropertyField*>(instance));
	else if (instance->getType().name == "Core::RectMask")
		return prop.getArrayValue<T>(reinterpret_cast<Core::RectMask*>(instance));
	throw std::invalid_argument("polyGetArrayValue::ERROR");
}
template<typename T, typename ClassType>
T polyGetArrayElementValue(Mirror::Property prop, std::size_t index, ClassType* instance) {
	if (instance->getType().name == "Core::Component")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::Component*>(instance), index);
	else if (instance->getType().name == "Core::Transform")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::Transform*>(instance), index);
	else if (instance->getType().name == "Core::BoxComponent")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::BoxComponent*>(instance), index);
	else if (instance->getType().name == "Core::RectTransform")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::RectTransform*>(instance), index);
	else if (instance->getType().name == "Core::Behaviour")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::Behaviour*>(instance), index);
	else if (instance->getType().name == "Core::Sprite")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::Sprite*>(instance), index);
	else if (instance->getType().name == "Core::Border")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::Border*>(instance), index);
	else if (instance->getType().name == "Core::Image")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::Image*>(instance), index);
	else if (instance->getType().name == "Core::Button")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::Button*>(instance), index);
	else if (instance->getType().name == "Core::CheckBox")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::CheckBox*>(instance), index);
	else if (instance->getType().name == "Core::ChildManager")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::ChildManager*>(instance), index);
	else if (instance->getType().name == "Core::ParentEntity")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::ParentEntity*>(instance), index);
	else if (instance->getType().name == "Core::UIBehaviour")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::UIBehaviour*>(instance), index);
	else if (instance->getType().name == "Core::LayoutController")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::LayoutController*>(instance), index);
	else if (instance->getType().name == "Core::ContentSizeFitter")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::ContentSizeFitter*>(instance), index);
	else if (instance->getType().name == "Core::RectSprite")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::RectSprite*>(instance), index);
	else if (instance->getType().name == "Core::Text")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::Text*>(instance), index);
	else if (instance->getType().name == "Core::RectButton")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::RectButton*>(instance), index);
	else if (instance->getType().name == "Core::DropDown")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::DropDown*>(instance), index);
	else if (instance->getType().name == "Core::WindowAnchor")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::WindowAnchor*>(instance), index);
	else if (instance->getType().name == "Core::WindowScale")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::WindowScale*>(instance), index);
	else if (instance->getType().name == "Core::MouseDrag")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::MouseDrag*>(instance), index);
	else if (instance->getType().name == "Core::LayoutGroup")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::LayoutGroup*>(instance), index);
	else if (instance->getType().name == "Core::LayoutElement")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::LayoutElement*>(instance), index);
	else if (instance->getType().name == "Core::HorizontalLayoutGroup")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::HorizontalLayoutGroup*>(instance), index);
	else if (instance->getType().name == "Core::VerticalLayoutGroup")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::VerticalLayoutGroup*>(instance), index);
	else if (instance->getType().name == "Core::GridLayoutGroup")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::GridLayoutGroup*>(instance), index);
	else if (instance->getType().name == "Core::DragAndResize")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::DragAndResize*>(instance), index);
	else if (instance->getType().name == "Core::Inspector")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::Inspector*>(instance), index);
	else if (instance->getType().name == "Core::Selectable")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::Selectable*>(instance), index);
	else if (instance->getType().name == "Core::InputField")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::InputField*>(instance), index);
	else if (instance->getType().name == "Core::PropertyField")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::PropertyField*>(instance), index);
	else if (instance->getType().name == "Core::RectMask")
		return prop.getArrayElementValue<T>(reinterpret_cast<Core::RectMask*>(instance), index);
	throw std::invalid_argument("polyGetArrayElementValue::ERROR");
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
	else if (instance->getType().name == "Core::CheckBox")
		return prop.setValue(reinterpret_cast<Core::CheckBox*>(instance), value);
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
	else if (instance->getType().name == "Core::Selectable")
		return prop.setValue(reinterpret_cast<Core::Selectable*>(instance), value);
	else if (instance->getType().name == "Core::InputField")
		return prop.setValue(reinterpret_cast<Core::InputField*>(instance), value);
	else if (instance->getType().name == "Core::PropertyField")
		return prop.setValue(reinterpret_cast<Core::PropertyField*>(instance), value);
	else if (instance->getType().name == "Core::RectMask")
		return prop.setValue(reinterpret_cast<Core::RectMask*>(instance), value);
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
	else if (instance->getType().name == "Core::CheckBox")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::CheckBox*>(instance), value);
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
	else if (instance->getType().name == "Core::Selectable")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::Selectable*>(instance), value);
	else if (instance->getType().name == "Core::InputField")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::InputField*>(instance), value);
	else if (instance->getType().name == "Core::PropertyField")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::PropertyField*>(instance), value);
	else if (instance->getType().name == "Core::RectMask")
		return prop.setArrayValue<T, N>(reinterpret_cast<Core::RectMask*>(instance), value);
	throw std::invalid_argument("polySetArrayValue::ERROR");
}
template<typename T, typename ClassType>
void polySetArrayElementValue(Mirror::Property prop, std::size_t index, ClassType* instance, T value) {
	if (instance->getType().name == "Core::Component")
		return prop.setArrayElementValue(reinterpret_cast<Core::Component*>(instance), index, value);
	else if (instance->getType().name == "Core::Transform")
		return prop.setArrayElementValue(reinterpret_cast<Core::Transform*>(instance), index, value);
	else if (instance->getType().name == "Core::BoxComponent")
		return prop.setArrayElementValue(reinterpret_cast<Core::BoxComponent*>(instance), index, value);
	else if (instance->getType().name == "Core::RectTransform")
		return prop.setArrayElementValue(reinterpret_cast<Core::RectTransform*>(instance), index, value);
	else if (instance->getType().name == "Core::Behaviour")
		return prop.setArrayElementValue(reinterpret_cast<Core::Behaviour*>(instance), index, value);
	else if (instance->getType().name == "Core::Sprite")
		return prop.setArrayElementValue(reinterpret_cast<Core::Sprite*>(instance), index, value);
	else if (instance->getType().name == "Core::Border")
		return prop.setArrayElementValue(reinterpret_cast<Core::Border*>(instance), index, value);
	else if (instance->getType().name == "Core::Image")
		return prop.setArrayElementValue(reinterpret_cast<Core::Image*>(instance), index, value);
	else if (instance->getType().name == "Core::Button")
		return prop.setArrayElementValue(reinterpret_cast<Core::Button*>(instance), index, value);
	else if (instance->getType().name == "Core::CheckBox")
		return prop.setArrayElementValue(reinterpret_cast<Core::CheckBox*>(instance), index, value);
	else if (instance->getType().name == "Core::ChildManager")
		return prop.setArrayElementValue(reinterpret_cast<Core::ChildManager*>(instance), index, value);
	else if (instance->getType().name == "Core::ParentEntity")
		return prop.setArrayElementValue(reinterpret_cast<Core::ParentEntity*>(instance), index, value);
	else if (instance->getType().name == "Core::UIBehaviour")
		return prop.setArrayElementValue(reinterpret_cast<Core::UIBehaviour*>(instance), index, value);
	else if (instance->getType().name == "Core::LayoutController")
		return prop.setArrayElementValue(reinterpret_cast<Core::LayoutController*>(instance), index, value);
	else if (instance->getType().name == "Core::ContentSizeFitter")
		return prop.setArrayElementValue(reinterpret_cast<Core::ContentSizeFitter*>(instance), index, value);
	else if (instance->getType().name == "Core::RectSprite")
		return prop.setArrayElementValue(reinterpret_cast<Core::RectSprite*>(instance), index, value);
	else if (instance->getType().name == "Core::Text")
		return prop.setArrayElementValue(reinterpret_cast<Core::Text*>(instance), index, value);
	else if (instance->getType().name == "Core::RectButton")
		return prop.setArrayElementValue(reinterpret_cast<Core::RectButton*>(instance), index, value);
	else if (instance->getType().name == "Core::DropDown")
		return prop.setArrayElementValue(reinterpret_cast<Core::DropDown*>(instance), index, value);
	else if (instance->getType().name == "Core::WindowAnchor")
		return prop.setArrayElementValue(reinterpret_cast<Core::WindowAnchor*>(instance), index, value);
	else if (instance->getType().name == "Core::WindowScale")
		return prop.setArrayElementValue(reinterpret_cast<Core::WindowScale*>(instance), index, value);
	else if (instance->getType().name == "Core::MouseDrag")
		return prop.setArrayElementValue(reinterpret_cast<Core::MouseDrag*>(instance), index, value);
	else if (instance->getType().name == "Core::LayoutGroup")
		return prop.setArrayElementValue(reinterpret_cast<Core::LayoutGroup*>(instance), index, value);
	else if (instance->getType().name == "Core::LayoutElement")
		return prop.setArrayElementValue(reinterpret_cast<Core::LayoutElement*>(instance), index, value);
	else if (instance->getType().name == "Core::HorizontalLayoutGroup")
		return prop.setArrayElementValue(reinterpret_cast<Core::HorizontalLayoutGroup*>(instance), index, value);
	else if (instance->getType().name == "Core::VerticalLayoutGroup")
		return prop.setArrayElementValue(reinterpret_cast<Core::VerticalLayoutGroup*>(instance), index, value);
	else if (instance->getType().name == "Core::GridLayoutGroup")
		return prop.setArrayElementValue(reinterpret_cast<Core::GridLayoutGroup*>(instance), index, value);
	else if (instance->getType().name == "Core::DragAndResize")
		return prop.setArrayElementValue(reinterpret_cast<Core::DragAndResize*>(instance), index, value);
	else if (instance->getType().name == "Core::Inspector")
		return prop.setArrayElementValue(reinterpret_cast<Core::Inspector*>(instance), index, value);
	else if (instance->getType().name == "Core::Selectable")
		return prop.setArrayElementValue(reinterpret_cast<Core::Selectable*>(instance), index, value);
	else if (instance->getType().name == "Core::InputField")
		return prop.setArrayElementValue(reinterpret_cast<Core::InputField*>(instance), index, value);
	else if (instance->getType().name == "Core::PropertyField")
		return prop.setArrayElementValue(reinterpret_cast<Core::PropertyField*>(instance), index, value);
	else if (instance->getType().name == "Core::RectMask")
		return prop.setArrayElementValue(reinterpret_cast<Core::RectMask*>(instance), index, value);
	throw std::invalid_argument("polySetValue::ERROR");
}
#include "ReflectionPolymorph.generated.h"
using namespace Mirror;

inline bool isReflected(std::string className) {
	if (className == "Core::Component")
		return true;
	if (className == "Core::Transform")
		return true;
	if (className == "Core::BoxComponent")
		return true;
	if (className == "Core::RectTransform")
		return true;
	if (className == "Core::Behaviour")
		return true;
	if (className == "Core::Sprite")
		return true;
	if (className == "Core::Border")
		return true;
	if (className == "Core::Image")
		return true;
	if (className == "Core::Button")
		return true;
	if (className == "Core::CheckBox")
		return true;
	if (className == "Core::ChildManager")
		return true;
	if (className == "Core::ParentEntity")
		return true;
	if (className == "Core::UIBehaviour")
		return true;
	if (className == "Core::LayoutController")
		return true;
	if (className == "Core::ContentSizeFitter")
		return true;
	if (className == "Core::RectSprite")
		return true;
	if (className == "Core::Text")
		return true;
	if (className == "Core::RectButton")
		return true;
	if (className == "Core::DropDown")
		return true;
	if (className == "Core::WindowAnchor")
		return true;
	if (className == "Core::WindowScale")
		return true;
	if (className == "Core::MouseDrag")
		return true;
	if (className == "Core::LayoutGroup")
		return true;
	if (className == "Core::LayoutElement")
		return true;
	if (className == "Core::HorizontalLayoutGroup")
		return true;
	if (className == "Core::VerticalLayoutGroup")
		return true;
	if (className == "Core::GridLayoutGroup")
		return true;
	if (className == "Core::DragAndResize")
		return true;
	if (className == "Core::Inspector")
		return true;
	if (className == "Core::Selectable")
		return true;
	if (className == "Core::InputField")
		return true;
	if (className == "Core::PropertyField")
		return true;
	if (className == "Core::RectMask")
		return true;
	return false;
}
inline Mirror::Class getType(std::string className) {
	if (className == "Core::Component")
		return Core::Component::getClassType();
	if (className == "Core::Transform")
		return Core::Transform::getClassType();
	if (className == "Core::BoxComponent")
		return Core::BoxComponent::getClassType();
	if (className == "Core::RectTransform")
		return Core::RectTransform::getClassType();
	if (className == "Core::Behaviour")
		return Core::Behaviour::getClassType();
	if (className == "Core::Sprite")
		return Core::Sprite::getClassType();
	if (className == "Core::Border")
		return Core::Border::getClassType();
	if (className == "Core::Image")
		return Core::Image::getClassType();
	if (className == "Core::Button")
		return Core::Button::getClassType();
	if (className == "Core::CheckBox")
		return Core::CheckBox::getClassType();
	if (className == "Core::ChildManager")
		return Core::ChildManager::getClassType();
	if (className == "Core::ParentEntity")
		return Core::ParentEntity::getClassType();
	if (className == "Core::UIBehaviour")
		return Core::UIBehaviour::getClassType();
	if (className == "Core::LayoutController")
		return Core::LayoutController::getClassType();
	if (className == "Core::ContentSizeFitter")
		return Core::ContentSizeFitter::getClassType();
	if (className == "Core::RectSprite")
		return Core::RectSprite::getClassType();
	if (className == "Core::Text")
		return Core::Text::getClassType();
	if (className == "Core::RectButton")
		return Core::RectButton::getClassType();
	if (className == "Core::DropDown")
		return Core::DropDown::getClassType();
	if (className == "Core::WindowAnchor")
		return Core::WindowAnchor::getClassType();
	if (className == "Core::WindowScale")
		return Core::WindowScale::getClassType();
	if (className == "Core::MouseDrag")
		return Core::MouseDrag::getClassType();
	if (className == "Core::LayoutGroup")
		return Core::LayoutGroup::getClassType();
	if (className == "Core::LayoutElement")
		return Core::LayoutElement::getClassType();
	if (className == "Core::HorizontalLayoutGroup")
		return Core::HorizontalLayoutGroup::getClassType();
	if (className == "Core::VerticalLayoutGroup")
		return Core::VerticalLayoutGroup::getClassType();
	if (className == "Core::GridLayoutGroup")
		return Core::GridLayoutGroup::getClassType();
	if (className == "Core::DragAndResize")
		return Core::DragAndResize::getClassType();
	if (className == "Core::Inspector")
		return Core::Inspector::getClassType();
	if (className == "Core::Selectable")
		return Core::Selectable::getClassType();
	if (className == "Core::InputField")
		return Core::InputField::getClassType();
	if (className == "Core::PropertyField")
		return Core::PropertyField::getClassType();
	if (className == "Core::RectMask")
		return Core::RectMask::getClassType();
	throw std::invalid_argument("Mirror::getType::ERROR There is no such reflected class!");
}
inline bool isArrayType(const Mirror::VariableType& type) {
	if (type.isArray) return true;
	if (type.name == "std::vector")
		return true;
	if (type.name == "std::array")
		return true;
	return false;
}
typedef TypeList<Core::Component,Core::Transform,Core::BoxComponent,Core::RectTransform,Core::Behaviour,Core::Sprite,Core::Border,Core::Image,Core::Button,Core::CheckBox,Core::ChildManager,Core::ParentEntity,Core::UIBehaviour,Core::LayoutController,Core::ContentSizeFitter,Core::RectSprite,Core::Text,Core::RectButton,Core::DropDown,Core::WindowAnchor,Core::WindowScale,Core::MouseDrag,Core::LayoutGroup,Core::LayoutElement,Core::HorizontalLayoutGroup,Core::VerticalLayoutGroup,Core::GridLayoutGroup,Core::DragAndResize,Core::Inspector,Core::Selectable,Core::InputField,Core::PropertyField,Core::RectMask> ReflectedTypes;
}

#endif