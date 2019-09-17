#include "Inspector.h"
#include "ReflectedObject.h"
#include "Input.h"
#include "HorizontalLayoutGroup.h"
#include "VerticalLayoutGroup.h"
#include "RectTransform.h"
#include "RectSprite.h"
#include "RectMask.h"
#include "ScrollRect.h"
#include "ScrollBar.h"
#include "LayoutElement.h"
#include "InputField.h"
#include "HideFlags.h"
#include "CheckBox.h"
#include "DropDownScroll.h"
#include "Maths/Vector2.h"
#include "ReflectionPolymorph.generated.h"
#include <limits>

using namespace Core;

Inspector::Inspector()
{
}


Inspector::~Inspector()
{
}

template<typename... Ts>
void addComponentDropDownOption(DropDown* dropDown, Inspector* inspector, Mirror::TypeList<Ts...>) {} // End
template<typename T, typename... Ts>
typename std::enable_if_t<std::is_base_of<Component, T>::value&& std::is_default_constructible<T>::value && !std::is_abstract<T>::value, void> addComponentDropDownOption(DropDown* dropDown, Inspector* inspector, Mirror::TypeList<T, Ts...>);
template<typename T, typename... Ts>
typename std::enable_if_t<!std::is_base_of<Component, T>::value || !std::is_default_constructible<T>::value || std::is_abstract<T>::value, void> addComponentDropDownOption(DropDown* dropDown, Inspector* inspector, Mirror::TypeList<T, Ts...>);

template<typename T, typename... Ts>
typename std::enable_if_t<std::is_base_of<Component, T>::value && std::is_default_constructible<T>::value && !std::is_abstract<T>::value, void> addComponentDropDownOption(DropDown* dropDown, Inspector* inspector, Mirror::TypeList<T, Ts...>) {
	dropDown->addOption(T::getClassType().name, Core::bind(inspector, &Inspector::addComponentToTarget<T>));
	addComponentDropDownOption(dropDown, inspector, Mirror::TypeList<Ts...>{}); // Continue
}

template<typename T, typename... Ts>
typename std::enable_if_t<!std::is_base_of<Component, T>::value || !std::is_default_constructible<T>::value || std::is_abstract<T>::value, void> addComponentDropDownOption(DropDown* dropDown, Inspector* inspector, Mirror::TypeList<T, Ts...>) {
	addComponentDropDownOption(dropDown, inspector, Mirror::TypeList<Ts...>{}); // Continue
}


void Inspector::awake() {
	// create scroll panel for targetComponentList
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (rect) {
		if (!scrollPanel.isValid()) {
			HorizontalLayoutGroup* parentGroup = owner.addComponent<HorizontalLayoutGroup>();
			parentGroup->childForceExpandHeight = true;
			parentGroup->childForceExpandWidth = true;
			parentGroup->shrinkableChildHeight = true;
			parentGroup->shrinkableChildWidth = true;
			parentGroup->spacing = 5;

			scrollPanel = createEntity("Inspector_Scroll_Panel",
				RectMask(),
				RectTransform(0, 0, 0, 0, rect->getZ() + 0.05f, Alignment::TOP_LEFT)
			);
			ScrollRect* scrollRect = scrollPanel.addComponent<ScrollRect>();
			scrollRect->paddingBottom = 10;
			LayoutElement* element = scrollPanel.addComponent<LayoutElement>();
			element->setFlexibleSize(Vector2(1, 1));
			element->setFlexibleSizeEnabled(true);
			element->setMinSize(Vector2(0, 0));
			element->setMinSizeEnabled(true);
			VerticalLayoutGroup* group = scrollPanel.addComponent<VerticalLayoutGroup>();
			group->childForceExpandHeight = false;
			group->childForceExpandWidth = true;
			group->shrinkableChildHeight = false;
			group->shrinkableChildWidth = false;
			group->spacing = 5;
			group->paddingTop = 10;
			group->paddingLeft = 10;
			group->paddingRight = 10;
			scrollPanel.setParent(owner);
		}
		if (!scrollBar.isValid()) {
			scrollBar = createEntity("Inspector_Scroll_Bar",
				ScrollBar(scrollPanel),
				RectTransform(0, 0, 20, 500, rect->getZ() + 10.0f)
			);
			LayoutElement* element = scrollBar.addComponent<LayoutElement>();
			element->setMinSize(Vector2(20, 0));
			element->setMinSizeEnabled(true);
			element->setFlexibleSize(Vector2(0, 1));
			element->setFlexibleSizeEnabled(true);
			scrollBar.setParent(owner);
		}
	}
}

void Inspector::clearEntries() {
	for (EntityHandle& handle : targetComponentList) {
		for (std::size_t i = 0; i < handle.getChildCount(); i++) {
			EntityHandle child = handle.getChild(i);
		}
		handle.destroy();
	}
	targetComponentList.clear();
	targetComponents.clear();
}

ReflectedObject* getInstanceOfValue(ReflectedObject* instance, PropertyValueID value) {
	if (value.isArrayElement) {
		return static_cast<ReflectedObject*>(instance->getArrayElementPointers(value.prop.name)[value.arrayIndex]);
	}
	else {
		return static_cast<ReflectedObject*>(instance->getPointer(value.prop.name));
	}
}

EntityHandle Inspector::createPropertyValueField(std::string label, PropertyValueID value, Component* root, ReflectedObject* instance, std::string entityName) {
	std::size_t offset = reinterpret_cast<unsigned char*>(instance) - reinterpret_cast<unsigned char*>(root);
	ReflectedObjectHandle instanceHandle(ComponentHandle(root), offset);

	RectTransform* rect = owner.getComponent<RectTransform>();
	EntityHandle propValueField = createEntity(entityName,
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.2f, Alignment::TOP_LEFT)
	);
	PropertyEditor* editor = propValueField.addComponent(PropertyEditor(value, instanceHandle));
	if (Mirror::isReflected(value.prop.type.name)) {
		VerticalLayoutGroup* fieldLayout = propValueField.addComponent<VerticalLayoutGroup>();
		fieldLayout->spacing = 3;
		fieldLayout->shrinkableChildHeight = false;
		fieldLayout->shrinkableChildWidth = false;
	}
	else {
		HorizontalLayoutGroup* fieldLayout = propValueField.addComponent<HorizontalLayoutGroup>();
		fieldLayout->spacing = 10;
		fieldLayout->shrinkableChildHeight = false;
		fieldLayout->shrinkableChildWidth = false;
	}
	// -------------- Content --------------
	Mirror::VariableType& propType = value.prop.type;
	std::wstring propValue = PropertyEditor::propertyValueToString(value, instanceHandle);
	// Label
	EntityHandle propLabel = createEntity(entityName + "_Label");
	RectTransform* propTextRect = propLabel.addComponent(RectTransform(0, 0, 0, 0, rect->getZ() + 0.1f, Alignment::LEFT));
	Text* propText = propLabel.addComponent(Text(label + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
	propTextRect->setSize(propText->getSize());
	propLabel.setParent(propValueField);
	// Value Display & Input
	if (propType.isNumber()) {
		EntityHandle inputField = createEntity(entityName + "_InputField",
			RectSprite(Color(255, 255, 255)),
			RectMask(),
			RectTransform(0, 0, 100, 16, rect->getZ() + 0.3f, Alignment::TOP_LEFT)
		);
		InputField* inputFieldComponent = inputField.addComponent<InputField>();
		inputFieldComponent->setText(propValue);
		if (propType.isDecimal())
			inputFieldComponent->contentType = InputField::ContentType::Decimal;
		else
			inputFieldComponent->contentType = InputField::ContentType::Integer;
		inputFieldComponent->onSubmit = Core::bind(editor, &PropertyEditor::onTextSubmit);
		inputField.setParent(propValueField);
	}
	else if (propType.isString() || propType.isWideString()) {
		EntityHandle inputField = createEntity(entityName + "_InputField",
			RectSprite(Color(255, 255, 255)),
			RectMask(),
			RectTransform(0, 0, 250, 16, rect->getZ() + 0.3f, Alignment::TOP_LEFT)
		);
		InputField* inputFieldComponent = inputField.addComponent<InputField>();
		inputFieldComponent->setText(propValue);
		inputFieldComponent->contentType = InputField::ContentType::Standard;
		inputFieldComponent->onSubmit = Core::bind(editor, &PropertyEditor::onTextSubmit);
		inputField.setParent(propValueField);
	}
	else if (propType.isBool()) {
		EntityHandle propValueDisplay = createEntity(entityName + "_Value",
			RectTransform(0, 0, 20, 20, rect->getZ() + 0.3f, Alignment::TOP_LEFT)
		);
		CheckBox* checkBox = propValueDisplay.addComponent<CheckBox>();
		checkBox->setToggle(propValue == L"true");
		checkBox->onToggle = Core::bind(editor, &PropertyEditor::onBoolSubmit);
		propValueDisplay.setParent(propValueField);
	}
	else if (Mirror::isReflected(propType.name)) {
		ReflectedObject* propInstance = getInstanceOfValue(instance, value); // instance->getPointer(value.prop.name)
		Mirror::Class classType = propInstance->getType();
		for (std::size_t i = 0; i < classType.properties.size(); i++) {
			EntityHandle propField = createPropertyField(entityName + "_Property_" + std::to_string(i), classType.properties[i], root, propInstance);
			propField.setParent(propValueField);
		}
	}
	else {
		EntityHandle propValueDisplay = createEntity(entityName + "_Value");
		RectTransform* propValueTextRect = propValueDisplay.addComponent(RectTransform(0, 0, 0, 0, rect->getZ() + 0.3f, Alignment::LEFT));
		Text* propValueText = propValueDisplay.addComponent(Text(propValue, "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propValueTextRect->setSize(propValueText->getSize());
		propValueDisplay.setParent(propValueField);
	}
	// -------------- End of Content --------------
	return propValueField;
}

EntityHandle Inspector::createPropertyField(std::string name, Mirror::Property& prop, Component* component, ReflectedObject* instance) {
	RectTransform* rect = owner.getComponent<RectTransform>();

	// Create Field
	EntityHandle propField = createEntity(name,
		//Border(1, Color(255,255,255,125)),
		//RectSprite(Color(40, 40, 40, 255)),
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
	);
	VerticalLayoutGroup* fieldLayout = propField.addComponent<VerticalLayoutGroup>();
	fieldLayout->spacing = 5;
	fieldLayout->paddingTop = 5;
	fieldLayout->paddingBottom = 5;
	fieldLayout->paddingRight = 5;
	fieldLayout->paddingLeft = 5;

	// Create Field Body
	if (Mirror::isArrayType(prop.type)) {
		std::size_t arraySize = prop.getArraySize(instance);
		for (std::size_t i = 0; i < arraySize; i++) {
			PropertyValueID value(prop, i);

			EntityHandle line = createPropertyValueField(prop.name + "[" + std::to_string(i) + "]", value, component, instance, name + "_line_" + std::to_string(i));
			line.setParent(propField);
		}
	}
	else {
		PropertyValueID value(prop);

		EntityHandle line = createPropertyValueField(prop.name, value, component, instance, name + "_line");
		line.setParent(propField);
	}

	return propField;
}

void Inspector::addComponentEntry(Component* component, std::size_t id) {
	// Reflection Data
	Mirror::Class type = component->getType();

	RectTransform* rect = owner.getComponent<RectTransform>();
	std::string entryName = "Inspector_Component_" + std::to_string(id);

	// Entry
	EntityHandle entry = createEntity(entryName,
		RectSprite(Color(55, 55, 55)),
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.09f, Alignment::TOP_LEFT)
	);
	VerticalLayoutGroup* group = entry.addComponent<VerticalLayoutGroup>();
	group->paddingTop = 1;
	group->paddingLeft = 1;
	group->paddingRight = 1;
	group->paddingBottom = 1;
	group->spacing = 1;
	group->childForceExpandHeight = false;
	group->childForceExpandWidth = true;
	group->shrinkableChildHeight = false;
	group->shrinkableChildWidth = false;
	entry.setParent(scrollPanel);

	// Entry content
	EntityHandle labelField = createEntity(entryName + "_LabelField",
		RectSprite(Color(40, 40, 40)),
		RectTransform(0, 0, 0, 24, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
	);
	LayoutElement* labelLayout = labelField.addComponent<LayoutElement>();
	labelLayout->setFlexibleSizeEnabled(true);
	labelLayout->setFlexibleSize(Vector2(1, 0));
	labelField.setParent(entry);
	EntityHandle label = createEntity(entryName + "_Label",
		Text(type.name, "resources/fonts/segoeui.ttf", 16, Color(255, 255, 255)),
		RectTransform(5, 12, 0, 24, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	label.setParent(labelField);
	//*/
	EntityHandle entryContent = createEntity(entryName + "_Content",
		RectTransform(0,0,0,0,rect->getZ() + 0.1f, Alignment::TOP_LEFT)
	);
	VerticalLayoutGroup* contentGroup = entryContent.addComponent<VerticalLayoutGroup>();
	contentGroup->paddingTop = 5;
	contentGroup->paddingLeft = 5;
	contentGroup->paddingRight = 5;
	contentGroup->paddingBottom = 5;
	contentGroup->spacing = 5;
	contentGroup->childForceExpandHeight = false;
	contentGroup->childForceExpandWidth = false;
	contentGroup->shrinkableChildHeight = false;
	contentGroup->shrinkableChildWidth = false;
	// Property field
	for (std::size_t i = 0; i < type.properties.size(); i++) {
		ComponentHandle componentHandle(component);
		EntityHandle propField = createPropertyField(entryName + "_Property_" + std::to_string(i), type.properties[i], component, component);
		propField.setParent(entryContent);
	}
	entryContent.setParent(entry);
	// End of Entry content
	targetComponentList.push_back(entry);
	targetComponents.push_back(component->getTypeID());
}

void Inspector::createEntries() {
	// Get components and their reflection data
	std::size_t i = 0;
	for (Component* component : currentTarget.getComponents()) {
		addComponentEntry(component, i++);
	}
	// Add an 'Add Component' button
	EntityHandle addComponentButton = createEntity("Inspector_Add_Component_Button",
		Text("Add Component", "resources/fonts/segoeui.ttf", 14, Color(255, 255, 255, 255)),
		RectTransform(0, 0, 200, 50, owner.getComponent<RectTransform>()->getZ() + 0.1f)
	);
	DropDownScroll* dropDown = addComponentButton.addComponent(DropDownScroll(Text("Add Component", "resources/fonts/segoeui.ttf", 14, Color(255, 255, 255, 255))));
	dropDown->boxWidth = 200;
	dropDown->boxHeight = 100;
	dropDown->optionFont = Font("resources/fonts/segoeui.ttf", 14);
	dropDown->optionTextColor = Color(255, 255, 255, 255);
	dropDown->border = true;
	dropDown->borderColor = Color(255, 255, 255, 255);
	dropDown->borderSize = 1;
	dropDown->boxPaddingY = 5;
	LayoutElement* layoutElement = addComponentButton.addComponent<LayoutElement>();
	layoutElement->setMinSize(Vector2(200, 50));
	layoutElement->setMinSizeEnabled(true);
	layoutElement->setFlexibleSize(Vector2(1, 0));
	layoutElement->setFlexibleSizeEnabled(true);
	addComponentDropDownOption(dropDown, this, Mirror::ReflectedTypes{});
	addComponentButton.setParent(scrollPanel);
	targetComponentList.push_back(addComponentButton);
}

void Inspector::refresh() {
	clearEntries();
	createEntries();
}

void Inspector::inspect(EntityHandle entity) {
	if (entity.refresh()) {
		HideFlags hideFlags = entity.getEntityHideFlags();
		if (hideFlags == HideFlags::HideInInspector) return;
		currentTarget = entity;
		refresh();
	}
}

void Inspector::onMouseButtonPressed(int buttoncode, int mods) {
	EntityHandle target = input->getLastClicked();
	if (target.getEntity() != currentTarget.getEntity()) {
		inspect(target);
	}
}

void Inspector::lateUpdate(float deltaTime) {
	if (currentTarget.getEntity().getID() == Entity::INVALID_ID) return;
	// Check if refresh is necessary
	std::vector<Component*> components = currentTarget.getComponents();
	if (components.size() != targetComponents.size()) {
		refresh();
	}
	else {
		for (std::size_t i = 0; i < components.size(); i++) {
			if (components[i]->getTypeID() != targetComponents[i]) {
				refresh();
				break;
			}
		}
	}
}