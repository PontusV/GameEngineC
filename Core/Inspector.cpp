#include "Inspector.h"
#include "ReflectedObject.h"
#include "Input.h"
#include "HorizontalLayoutGroup.h"
#include "VerticalLayoutGroup.h"
#include "RectTransform.h"
#include "RectButton.h"
#include "RectSprite.h"
#include "RectMask.h"
#include "ScrollRect.h"
#include "ScrollBar.h"
#include "LayoutElement.h"
#include "InputField.h"
#include "HideFlags.h"
#include "CheckBox.h"
#include "DropDownScroll.h"
#include "ComponentEntry.h"
#include "Maths/Vector2.h"
#include "ChildManager.h"
#include "ParentEntity.h"
#include "ReflectionPolymorph.generated.h"
#include <limits>

using namespace Core;

Inspector::Inspector()
{
}


Inspector::~Inspector()
{
}

std::vector<Component*> getInspectableComponents(EntityHandle target) {
	std::vector<Component*> components;
	for (Component* component : target.getComponents()) {
		if (!component->getType().hasAnnotation("hideInInspector"))
			components.push_back(component);
	}
	return components;
}

/* Utility */
template<typename T>
bool contains(const std::vector<T>& vec, const T& value) {
	for (const T& v : vec) {
		if (v == value) return true;
	}
	return false;
}

bool filterComponentType(const std::vector<ComponentTypeID>& filter, const ComponentTypeID& typeID) {
	for (const ComponentTypeID& filterID : filter) {
		if (typeID == filterID || contains(Mirror::polyGetDerivedTypeIDs(typeID), filterID)) {
			return true;
		}
	}
	return false;
}

template<typename... Ts>
void addComponentDropDownOption(DropDown* dropDown, Inspector* inspector, std::vector<ComponentTypeID> filter, Mirror::TypeList<Ts...>) {} // End
template<typename T, typename... Ts>
typename std::enable_if_t<std::is_base_of<Component, T>::value&& std::is_default_constructible<T>::value && !std::is_abstract<T>::value && !std::is_same<T, ChildManager>::value && !std::is_same<T, ParentEntity>::value, void> addComponentDropDownOption(DropDown* dropDown, Inspector* inspector, std::vector<ComponentTypeID> filter, Mirror::TypeList<T, Ts...>);
template<typename T, typename... Ts>
typename std::enable_if_t<!std::is_base_of<Component, T>::value || !std::is_default_constructible<T>::value || std::is_abstract<T>::value || std::is_same<T, ChildManager>::value || std::is_same<T, ParentEntity>::value, void> addComponentDropDownOption(DropDown* dropDown, Inspector* inspector, std::vector<ComponentTypeID> filter, Mirror::TypeList<T, Ts...>);

template<typename T, typename... Ts>
typename std::enable_if_t<std::is_base_of<Component, T>::value && std::is_default_constructible<T>::value && !std::is_abstract<T>::value && !std::is_same<T, ChildManager>::value && !std::is_same<T, ParentEntity>::value, void> addComponentDropDownOption(DropDown* dropDown, Inspector* inspector, std::vector<ComponentTypeID> filter, Mirror::TypeList<T, Ts...>) {
	Mirror::Class type = T::getClassType();
	if (!filterComponentType(filter, type.typeID))
		dropDown->addOption(type.name, Core::bind(inspector, &Inspector::addComponentToTarget<T>));
	addComponentDropDownOption(dropDown, inspector, filter, Mirror::TypeList<Ts...>{}); // Continue
}

template<typename T, typename... Ts>
typename std::enable_if_t<!std::is_base_of<Component, T>::value || !std::is_default_constructible<T>::value || std::is_abstract<T>::value || std::is_same<T, ChildManager>::value || std::is_same<T, ParentEntity>::value, void> addComponentDropDownOption(DropDown* dropDown, Inspector* inspector, std::vector<ComponentTypeID> filter, Mirror::TypeList<T, Ts...>) {
	addComponentDropDownOption(dropDown, inspector, filter, Mirror::TypeList<Ts...>{}); // Continue
}


void Inspector::awake() {
	// create scroll panel for targetComponentList
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (rect) {
		if (!scrollPanel.isValid()) {
			HorizontalLayoutGroup* parentGroup = ((EntityHandle)owner).addComponent<HorizontalLayoutGroup>();
			parentGroup->childForceExpandHeight = true;
			parentGroup->childForceExpandWidth = true;
			parentGroup->shrinkableChildHeight = true;
			parentGroup->shrinkableChildWidth = true;
			parentGroup->spacing = 10;

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
			group->shrinkableChildWidth = true;
			group->spacing = 5;
			group->paddingTop = 10;
			group->paddingLeft = 10;
			group->paddingRight = 0;
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
		destroyEntity(handle);
	}
	targetComponentList.clear();
	targetComponents.clear();
}

void* getInstanceOfValue(void* instance, std::size_t typeID, PropertyValueID value) {
	if (value.isArrayElement) {
		return Mirror::getArrayElementPointers(value.prop, instance, typeID)[value.arrayIndex];
	}
	else {
		return Mirror::getPointer(value.prop, instance, typeID);
	}
}

EntityHandle Inspector::createPropertyValueField(std::string label, PropertyValueID value, Component* root, Mirror::Property& rootProp, void* instance, std::size_t typeID, std::string entityName) {
	std::size_t offset = reinterpret_cast<unsigned char*>(instance) - reinterpret_cast<unsigned char*>(root);
	ReflectedObjectHandle instanceHandle(ComponentHandle(root), offset, typeID);

	RectTransform* rect = owner.getComponent<RectTransform>();
	EntityHandle propValueField = createEntity(entityName,
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.2f, Alignment::TOP_LEFT)
	);
	PropertyEditor* editor = propValueField.addComponent(PropertyEditor(value, rootProp, instanceHandle));
	std::size_t propTypeID = Mirror::getTypeID(value.prop.type.name);
	if (propTypeID) {
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
	else if (propTypeID) {
		void* propInstance = getInstanceOfValue(instance, typeID, value);
		Mirror::Class classType = Mirror::getType(propType.name);
		for (std::size_t i = 0; i < classType.properties.size(); i++) {
			EntityHandle propField = createPropertyField(entityName + "_Property_" + std::to_string(i), classType.properties[i], root, value.prop, propInstance, propTypeID);
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

EntityHandle Inspector::createPropertyField(std::string name, Mirror::Property& prop, Component* component, Mirror::Property& rootProp, void* instance, std::size_t typeID) {
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
		std::size_t arraySize = Mirror::getArraySize(prop, instance, typeID);
		for (std::size_t i = 0; i < arraySize; i++) {
			PropertyValueID value(prop, i);

			EntityHandle line = createPropertyValueField(prop.name + "[" + std::to_string(i) + "]", value, component, rootProp, instance, typeID, name + "_line_" + std::to_string(i));
			line.setParent(propField);
		}
	}
	else {
		PropertyValueID value(prop);

		EntityHandle line = createPropertyValueField(prop.name, value, component, rootProp, instance, typeID, name + "_line");
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
	LayoutElement* labelFieldLayout = labelField.addComponent<LayoutElement>();
	labelFieldLayout->setMinSizeEnabled(true);
	labelFieldLayout->setMinSize(Vector2(0, 24));
	labelFieldLayout->setFlexibleSizeEnabled(true);
	labelFieldLayout->setFlexibleSize(Vector2(1, 0));
	HorizontalLayoutGroup* labelLayoutGroup = labelField.addComponent<HorizontalLayoutGroup>();
	labelLayoutGroup->childForceExpandWidth = true;
	labelLayoutGroup->paddingLeft = 5;
	labelLayoutGroup->paddingRight = 2;
	labelLayoutGroup->childAlignment = Alignment::LEFT;
	labelField.setParent(entry);
	EntityHandle label = createEntity(entryName + "_Label",
		Text(type.name, "resources/fonts/segoeui.ttf", 16, Color(255, 255, 255)),
		RectTransform(0, 12, 0, 24, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	LayoutElement* labelLayout = label.addComponent<LayoutElement>();
	labelLayout->setFlexibleSizeEnabled(true);
	labelLayout->setFlexibleSize(Vector2(1.0f, 0));
	label.setParent(labelField);
	EntityHandle removeButtonEntity = createEntity(entryName + "_RemoveButton",
		RectSprite(Color(255, 40, 40)),
		RectTransform(0, 0, 20, 20, rect->getZ() + 0.2f, Alignment::TOP_LEFT)
	);
	ComponentEntry* entryComponent = removeButtonEntity.addComponent(ComponentEntry(currentTarget, component->getType().typeID));
	RectButton* removeButton = removeButtonEntity.addComponent<RectButton>();
	removeButton->colors[RectButton::ButtonState::DEFAULT] = Color(255, 50, 50);
	removeButton->colors[RectButton::ButtonState::HOVER_OVER] = Color(255, 100, 100);
	removeButton->colors[RectButton::ButtonState::PRESSED_DOWN] = Color(200, 0, 0);
	removeButton->clickFunction = Core::bind(entryComponent, &ComponentEntry::removeComponent);
	LayoutElement* removeButtonLayout = removeButtonEntity.addComponent<LayoutElement>();
	removeButtonLayout->setFlexibleSizeEnabled(true);
	removeButtonLayout->setFlexibleSize(Vector2(0, 0));
	removeButtonLayout->setMinSizeEnabled(true);
	removeButtonLayout->setMinSize(Vector2(20, 20));
	removeButtonEntity.setParent(labelField);
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
		EntityHandle propField = createPropertyField(entryName + "_Property_" + std::to_string(i), type.properties[i], component, type.properties[i], component, component->getType().typeID);
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
	for (Component* component : getInspectableComponents(currentTarget)) {
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
	std::vector<ComponentTypeID> filter;
	for (Component* component : currentTarget.getComponents()) {
		filter.push_back(component->getType().typeID);
	}
	addComponentDropDownOption(dropDown, this, filter, Mirror::ReflectedTypes{});
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
	std::vector<Component*> components = getInspectableComponents(currentTarget);
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