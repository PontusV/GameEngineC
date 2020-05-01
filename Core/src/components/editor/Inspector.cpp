#include "Inspector.h"
#include "input/Input.h"
#include "engine/ResourceManager.h"
#include "components/ui/layout/HorizontalLayoutGroup.h"
#include "components/ui/layout/VerticalLayoutGroup.h"
#include "components/RectTransform.h"
#include "components/ui/input/RectButton.h"
#include "components/graphics/RectSprite.h"
#include "components/ui/RectMask.h"
#include "components/ui/layout/LayoutElement.h"
#include "components/ui/input/InputField.h"
#include "entity/HideFlags.h"
#include "components/ui/input/CheckBox.h"
#include "components/ui/input/DropDownScroll.h"
#include "maths/Vector2.h"
#include "maths/MatrixTransform.h"
#include "components/entity/ChildManager.h"
#include "components/entity/ParentEntity.h"
#include "ReflectionPolymorph.generated.h"
#include <limits>

using namespace Core;


Inspector::Inspector()
{
}


Inspector::~Inspector()
{
}

void Inspector::setNextSelectable(const EntityHandle& entity) {
	for (Selectable* selectable : owner.getScene()->getComponentsFromQueue<Selectable>(prevSelectable.getEntity())) {
		selectable->setNext(entity);
	}
	if (!prevSelectable.isValid()) firstSelectable = entity;
	prevSelectable = entity;
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

void Inspector::onEnable() {
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (rect) {
		LayoutElement* element = owner.getComponent<LayoutElement>();
		element->setFlexibleSize(Vector2(1, 1));
		element->setFlexibleSizeEnabled(true);
		element->setMinSize(Vector2(0, 0));
		element->setMinSizeEnabled(true);

		VerticalLayoutGroup* group = owner.getComponent<VerticalLayoutGroup>();
		group->childForceExpandHeight = false;
		group->childForceExpandWidth = true;
		group->shrinkableChildHeight = false;
		group->shrinkableChildWidth = true;
		group->spacing = 5;
		group->paddingTop = 10;
		group->paddingLeft = 10;
		group->paddingRight = 10;
	}
	// Check if a target exists
	if (currentTarget.refresh())
		refresh();
}

void Inspector::onDisable() {
	clearEntries();
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
	std::size_t propTypeID = Mirror::getTypeID(value.prop.type.name);
	Mirror::VariableType& propType = value.prop.type;

	// Label
	HorizontalLayoutGroup propLabelFieldLayout = HorizontalLayoutGroup();
	propLabelFieldLayout.spacing = 5;
	EntityHandle propLabelField = createEntity(entityName + "_Label_Field",
		propLabelFieldLayout,
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.1f)
	);
	propLabelField.setParent(propValueField);

	EntityHandle propLabel = createEntity(entityName + "_Label");
	RectTransform* propTextRect = propLabel.addComponent(RectTransform(0, 0, 0, 0, rect->getZ() + 0.1f, Alignment::LEFT));
	Text* propText = propLabel.addComponent(Text(label + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
	propTextRect->setSize(propText->getSize());
	propLabel.setParent(propLabelField);

	if (propTypeID) {
		VerticalLayoutGroup fieldLayout = VerticalLayoutGroup();
		fieldLayout.spacing = 3;
		fieldLayout.shrinkableChildHeight = false;
		fieldLayout.shrinkableChildWidth = false;
		propValueField.addComponent(fieldLayout);

		Shader figureShader = ResourceManager::getInstance().loadShader("resources/shaders/figure");
		EntityHandle collapsibleIcon = createEntity(entityName + "_Collapsible_Icon",
			Image("resources/images/ui/arrow.png", figureShader),
			RectTransform(0, 0, 12, 12, rect->getZ() + 0.11f, Alignment::CENTER, maths::radians(90))
		);
		LayoutElement* collapsibleLayout = collapsibleIcon.addComponent<LayoutElement>();
		collapsibleLayout->setFlexibleSizeEnabled(true);
		collapsibleLayout->setFlexibleSize(Vector2(0, 0));
		collapsibleLayout->setMinSizeEnabled(true);
		collapsibleLayout->setMinSize(Vector2(10, 10));
		collapsibleIcon.setParent(propLabelField);
		collapsibleIcon.setSiblingIndexQueued(0);

		EntityHandle collapsibleContent = createEntity(entityName + "_Collapsible_Content",
			fieldLayout,
			RectTransform(0, 0, 0, 0, 0, Alignment::TOP_LEFT)
		);
		collapsibleContent.setParent(propValueField);

		void* propInstance = getInstanceOfValue(instance, typeID, value);
		Mirror::Class classType = Mirror::getType(propType.name);
		for (std::size_t i = 0; i < classType.properties.size(); i++) {
			EntityHandle propField = createPropertyField(entityName + "_Property_" + std::to_string(i), classType.properties[i], root, value.prop, propInstance, propTypeID);
			propField.setParent(collapsibleContent);
		}

		Button* collapsibleButton = collapsibleIcon.addComponent(Button(Image("resources/images/ui/arrow.png", figureShader, Color(150, 150, 150)), Image("resources/images/ui/arrow.png", figureShader, Color(0, 0, 0)), Image("resources/images/ui/arrow.png", figureShader, Color(255, 255, 255))));
		collapsibleButton->onLeftClick = Core::bind(this, &Inspector::collapse, collapsibleContent, collapsibleIcon);

		return propValueField;
	}
	else {
		HorizontalLayoutGroup* fieldLayout = propValueField.addComponent<HorizontalLayoutGroup>();
		fieldLayout->spacing = 10;
		fieldLayout->shrinkableChildHeight = false;
		fieldLayout->shrinkableChildWidth = false;
		fieldLayout->childAlignment = Alignment::LEFT;
	}
	PropertyEditor* editor = propValueField.addComponent(PropertyEditor(value, rootProp, instanceHandle));
	// -------------- Prop Value Field (Display & Input) --------------
	std::wstring propValue = PropertyEditor::propertyValueToString(value, instanceHandle);
	// Value Display & Input
	if (propType.isNumber()) {
		InputField inputFieldComponent = InputField();
		inputFieldComponent.setText(propValue);
		if (propType.isDecimal())
			inputFieldComponent.contentType = InputField::ContentType::Decimal;
		else
			inputFieldComponent.contentType = InputField::ContentType::Integer;
		inputFieldComponent.onChange = Core::bind(editor, &PropertyEditor::onTextSubmit);
		EntityHandle inputField = createEntity(entityName + "_InputField",
			inputFieldComponent,
			RectSprite(Color(255, 255, 255)),
			RectMask(),
			RectTransform(0, 0, 100, 16, rect->getZ() + 0.3f, Alignment::TOP_LEFT)
		);
		inputField.setParent(propValueField);
		setNextSelectable(inputField);
	}
	else if (propType.isString() || propType.isWideString()) {
		InputField inputFieldComponent = InputField();
		inputFieldComponent.setText(propValue);
		inputFieldComponent.contentType = InputField::ContentType::Standard;
		inputFieldComponent.onChange = Core::bind(editor, &PropertyEditor::onTextSubmit);
		EntityHandle inputField = createEntity(entityName + "_InputField",
			inputFieldComponent,
			RectSprite(Color(255, 255, 255)),
			RectMask(),
			RectTransform(0, 0, 250, 16, rect->getZ() + 0.3f, Alignment::TOP_LEFT)
		);
		inputField.setParent(propValueField);
		setNextSelectable(inputField);
	}
	else if (propType.isBool()) {
		CheckBox checkBox = CheckBox();
		checkBox.setToggle(propValue == L"true");
		checkBox.onToggle = Core::bind(editor, &PropertyEditor::onBoolSubmit);
		EntityHandle propValueDisplay = createEntity(entityName + "_Value",
			checkBox,
			RectTransform(0, 0, 20, 20, rect->getZ() + 0.3f, Alignment::TOP_LEFT)
		);
		propValueDisplay.setParent(propValueField);
	}
	else {
		Text propValueText = Text(propValue, "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255));
		Vector2 textSize = propValueText.getSize();
		RectTransform propValueTextRect = RectTransform(0, 0, textSize.x, textSize.y, rect->getZ() + 0.3f, Alignment::LEFT);
		EntityHandle propValueDisplay = createEntity(entityName + "_Value",
			propValueTextRect,
			propValueText
		);
		propValueDisplay.setParent(propValueField);
	}
	// -------------- End of Content --------------
	return propValueField;
}

EntityHandle Inspector::createPropertyField(std::string name, Mirror::Property& prop, Component* component, Mirror::Property& rootProp, void* instance, std::size_t typeID) {
	RectTransform* rect = owner.getComponent<RectTransform>();

	// Create Field
	VerticalLayoutGroup fieldLayout = VerticalLayoutGroup();
	fieldLayout.spacing = 15;
	fieldLayout.paddingTop = 5;
	fieldLayout.paddingBottom = 5;
	fieldLayout.paddingRight = 5;
	fieldLayout.paddingLeft = component == instance ? 5 : 10;
	EntityHandle propField = createEntity(name,
		fieldLayout,
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
	);

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
	VerticalLayoutGroup group = VerticalLayoutGroup();
	group.paddingTop = 1;
	group.paddingLeft = 1;
	group.paddingRight = 1;
	group.paddingBottom = 1;
	group.spacing = 1;
	group.childForceExpandHeight = false;
	group.childForceExpandWidth = true;
	group.shrinkableChildHeight = false;
	group.shrinkableChildWidth = true;
	EntityHandle entry = createEntity(entryName,
		group,
		RectSprite(Color(55, 55, 55)),
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.09f, Alignment::TOP_LEFT)
	);
	entry.setParent(owner);

	// Entry Label Field
	LayoutElement labelFieldLayout = LayoutElement();
	labelFieldLayout.setMinSizeEnabled(true);
	labelFieldLayout.setMinSize(Vector2(0, 24));
	labelFieldLayout.setFlexibleSizeEnabled(true);
	labelFieldLayout.setFlexibleSize(Vector2(1, 0));
	HorizontalLayoutGroup labelLayoutGroup = HorizontalLayoutGroup();
	labelLayoutGroup.childForceExpandWidth = true;
	labelLayoutGroup.shrinkableChildWidth = true;
	labelLayoutGroup.paddingLeft = 5;
	labelLayoutGroup.paddingRight = 2;
	labelLayoutGroup.spacing = 5;
	labelLayoutGroup.childAlignment = Alignment::LEFT;
	EntityHandle labelField = createEntity(entryName + "_LabelField",
		labelFieldLayout,
		labelLayoutGroup,
		RectSprite(Color(40, 40, 40)),
		RectTransform(0, 0, 0, 24, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
	);
	labelField.setParent(entry);

	Shader figureShader = ResourceManager::getInstance().loadShader("resources/shaders/figure");
	LayoutElement collapsibleLayout = LayoutElement();
	collapsibleLayout.setFlexibleSizeEnabled(true);
	collapsibleLayout.setFlexibleSize(Vector2(0, 0));
	collapsibleLayout.setMinSizeEnabled(true);
	collapsibleLayout.setMinSize(Vector2(10, 10));
	EntityHandle collapsibleIcon = createEntity(entryName + "_Collapsible_Icon",
		collapsibleLayout,
		Image("resources/images/ui/arrow.png", figureShader),
		RectTransform(0, 0, 12, 12, rect->getZ() + 0.11f, Alignment::CENTER, maths::radians(90))
	);
	collapsibleIcon.setParent(labelField);
	LayoutElement labelLayout = LayoutElement();
	labelLayout.setFlexibleSizeEnabled(true);
	labelLayout.setFlexibleSize(Vector2(1.0f, 0));
	EntityHandle label = createEntity(entryName + "_Label",
		labelLayout,
		Text(type.name, "resources/fonts/segoeui.ttf", 16, Color(255, 255, 255)),
		RectTransform(0, 12, 0, 24, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	label.setParent(labelField);
	RectButton removeButton = RectButton();
	removeButton.colors[RectButton::ButtonState::DEFAULT] = Color(255, 50, 50);
	removeButton.colors[RectButton::ButtonState::HOVER_OVER] = Color(255, 100, 100);
	removeButton.colors[RectButton::ButtonState::PRESSED_DOWN] = Color(200, 0, 0);
	removeButton.onLeftClick = Core::bind(this, &Inspector::removeComponentFromTarget, (ComponentTypeID)component->getType().typeID);
	LayoutElement removeButtonLayout = LayoutElement();
	removeButtonLayout.setFlexibleSizeEnabled(true);
	removeButtonLayout.setFlexibleSize(Vector2(0, 0));
	removeButtonLayout.setMinSizeEnabled(true);
	removeButtonLayout.setMinSize(Vector2(20, 20));
	EntityHandle removeButtonEntity = createEntity(entryName + "_RemoveButton",
		removeButton,
		removeButtonLayout,
		RectSprite(Color(255, 40, 40)),
		RectTransform(0, 0, 20, 20, rect->getZ() + 0.2f, Alignment::TOP_LEFT)
	);
	removeButtonEntity.setParent(labelField);
	EntityHandle removeButtonCross = createEntity(entryName + "_RemoveButton_Cross",
		Image("resources/images/ui/cross.png"),
		RectTransform(10, 10, 10, 10, rect->getZ() + 0.201f, Alignment::CENTER)
	);
	removeButtonCross.setParent(removeButtonEntity);

	// Entry content
	VerticalLayoutGroup contentGroup = VerticalLayoutGroup();
	contentGroup.paddingTop = 5;
	contentGroup.paddingLeft = 5;
	contentGroup.paddingRight = 5;
	contentGroup.paddingBottom = 5;
	contentGroup.spacing = 5;
	contentGroup.childForceExpandHeight = false;
	contentGroup.childForceExpandWidth = false;
	contentGroup.shrinkableChildHeight = false;
	contentGroup.shrinkableChildWidth = false;
	EntityHandle entryContent = createEntity(entryName + "_Content",
		contentGroup,
		RectTransform(0,0,0,0,rect->getZ() + 0.1f, Alignment::TOP_LEFT)
	);
	// Property field
	for (std::size_t i = 0; i < type.properties.size(); i++) {
		EntityHandle propField = createPropertyField(entryName + "_Property_" + std::to_string(i), type.properties[i], component, type.properties[i], component, component->getType().typeID);
		propField.setParent(entryContent);
	}
	entryContent.setParent(entry);
	Button* collapsibleButton = collapsibleIcon.addComponent(Button(Image("resources/images/ui/arrow.png", figureShader, Color(150, 150, 150)), Image("resources/images/ui/arrow.png", figureShader, Color(0, 0, 0)), Image("resources/images/ui/arrow.png", figureShader, Color(255, 255, 255))));
	collapsibleButton->onLeftClick = Core::bind(this, &Inspector::collapse, entryContent, collapsibleIcon);

	// End of Entry content
	targetComponentList.push_back(entry);
	targetComponents.push_back(component->getType().typeID);

	// Clear prevSelectable & Complete chain
	for (Selectable* selectable : owner.getScene()->getComponentsFromQueue<Selectable>(prevSelectable.getEntity())) {
		selectable->setNext(firstSelectable);
	}
	firstSelectable.clear();
	prevSelectable.clear();
}

void Inspector::createEntries() {
	// Get components and their reflection data
	std::size_t i = 0;
	for (Component* component : getInspectableComponents(currentTarget)) {
		addComponentEntry(component, i++);
	}
	LayoutElement layoutElement = LayoutElement();
	layoutElement.setMinSize(Vector2(200, 50));
	layoutElement.setMinSizeEnabled(true);
	layoutElement.setFlexibleSize(Vector2(1, 0));
	layoutElement.setFlexibleSizeEnabled(true);
	// Add an 'Add Component' button
	EntityHandle addComponentButton = createEntity("Inspector_Add_Component_Button",
		layoutElement,
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
	std::vector<ComponentTypeID> filter;
	for (Component* component : currentTarget.getComponents()) {
		filter.push_back(component->getType().typeID);
	}
	addComponentDropDownOption(dropDown, this, filter, Mirror::ReflectedTypes{});
	addComponentButton.setParent(owner);
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
		if (isEnabled())
			refresh();
	}
}

void Inspector::lateUpdate(float deltaTime) {
	if (currentTarget.getEntity().getID() == Entity::INVALID_ID) return;
	if (!currentTarget.refresh()) {
		currentTarget = EntityHandle();
		clearEntries();
	}
	// Check if refresh is necessary
	std::vector<Component*> components = getInspectableComponents(currentTarget);
	if (components.size() != targetComponents.size()) {
		refresh();
	}
	else {
		for (std::size_t i = 0; i < components.size(); i++) {
			if (components[i]->getType().typeID != targetComponents[i]) {
				refresh();
				break;
			}
		}
	}
}

void Inspector::removeComponentFromTarget(ComponentTypeID typeID) {
	currentTarget.removeComponent(typeID);
}

void Inspector::collapse(EntityHandle contentHandle, EntityHandle iconHandle) {
	if (contentHandle.isActive()) {
		if (RectTransform* rect = iconHandle.getComponent<RectTransform>()) {
			contentHandle.deactivate();
			rect->setLocalRotation(0);
		}
	}
	else {
		if (RectTransform * rect = iconHandle.getComponent<RectTransform>()) {
			contentHandle.activate();
			rect->setLocalRotation(maths::radians(90));
		}
	}
}