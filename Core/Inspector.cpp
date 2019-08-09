#include "Inspector.h"
#include "ReflectedObject.h"
#include "Input.h"
#include "HorizontalLayoutGroup.h"
#include "VerticalLayoutGroup.h"
#include "RectTransform.h"
#include "RectSprite.h"
#include "RectMask.h"
#include "LayoutElement.h"
#include "InputField.h"
#include "HideFlags.h"
#include "Selectable.h"
#include "CheckBox.h"
#include "ReflectionPolymorph.generated.h"
#include <limits>

using namespace Core;

Inspector::Inspector()
{
}


Inspector::~Inspector()
{
}


void Inspector::awake() {
	// create scroll panel for targetComponentList
	if (!scrollPanel.isValid()) {
		VerticalLayoutGroup* parentGroup = owner.addComponent<VerticalLayoutGroup>();
		parentGroup->childForceExpandHeight = true;
		parentGroup->childForceExpandWidth = true;
		parentGroup->shrinkableChildHeight = true;
		parentGroup->shrinkableChildWidth = true;
		RectTransform* rect = owner.getComponent<RectTransform>();

		scrollPanel = createEntity("Inspector_Scroll_Panel",
			RectSprite(Color(150, 0, 0)),
			RectMask(),
			RectTransform(0, 0, 0, 0, rect->getZ() + 0.05f, Alignment::TOP_LEFT)
		);
		LayoutElement* element = scrollPanel.addComponent<LayoutElement>();
		element->setFlexibleSize(glm::vec2(1, 1));
		element->setFlexibleSizeEnabled(true);
		element->setPrefSizeEnabled(true);
		element->setMinSize(glm::vec2(400, 1000/*470*/));
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
}

void Inspector::clearEntries() {
	for (EntityHandle& handle : targetComponentList) {
		handle.destroy();
	}
	targetComponentList.clear();
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
		Selectable(),
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
	Text* propText = propLabel.addComponent(Text(label + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
	propLabel.addComponent(RectTransform(0, 0, propText->getSize().x, propText->getSize().y, rect->getZ() + 0.1f, Alignment::LEFT));
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
			std::cout << classType.properties[i].name << std::endl;
		}
	}
	else {
		EntityHandle propValueDisplay = createEntity(entityName + "_Value");
		Text* propValueText = propValueDisplay.addComponent(Text(propValue, "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propValueDisplay.addComponent(RectTransform(0, 0, propValueText->getSize().x, propValueText->getSize().y, rect->getZ() + 0.3f, Alignment::LEFT));
		propValueDisplay.setParent(propValueField);
	}
	// -------------- End of Content --------------
	return propValueField;
}

EntityHandle Inspector::createPropertyField(std::string name, Mirror::Property& prop, Component* component, ReflectedObject* instance) {
	RectTransform* rect = owner.getComponent<RectTransform>();

	// Create Field
	EntityHandle propField = createEntity(name,
		Selectable(),
		//Border(1, Color(255,255,255,125)),
		//RectSprite(Color(40, 40, 40, 255)),
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
	);
	VerticalLayoutGroup* fieldLayout = propField.addComponent<VerticalLayoutGroup>();
	fieldLayout->spacing = 3;
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
		RectSprite(Color(50, 50, 50)),
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.09f, Alignment::TOP_LEFT)
	);
	VerticalLayoutGroup* group = entry.addComponent<VerticalLayoutGroup>();
	group->paddingTop = 5;
	group->paddingLeft = 5;
	group->paddingRight = 5;
	group->paddingBottom = 5;
	group->spacing = 5;
	group->childForceExpandHeight = false;
	group->childForceExpandWidth = false;
	group->shrinkableChildHeight = false;
	group->shrinkableChildWidth = false;
	entry.setParent(scrollPanel);

	// Entry content
	EntityHandle label = createEntity(entryName + "_Label", Selectable());
	Text* labelText = label.addComponent(Text(type.name, "resources/fonts/segoeui.ttf", 20, Color(255, 255, 255)));
	label.addComponent(RectTransform(0, 0, labelText->getSize().x, labelText->getSize().y, rect->getZ() + 0.1f, Alignment::TOP_LEFT));
	label.setParent(entry);
	//*/
	// Property field
	for (std::size_t i = 0; i < type.properties.size(); i++) {
		ComponentHandle componentHandle(component);
		EntityHandle propField = createPropertyField(entryName + "_Property_" + std::to_string(i), type.properties[i], component, component);
		propField.setParent(entry);
	}
	// End of Entry content
	targetComponentList.push_back(entry);
}

void Inspector::inspect(EntityHandle entity) {
	if (entity.refresh()) {
		HideFlags hideFlags = entity.getEntityHideFlags();
		if (hideFlags == HideFlags::HideInInspector) return;
		currentTarget = entity;
		// Clear old target component list
		clearEntries();
		// Get components and their reflection data
		std::size_t i = 0;
		for (Component* component : entity.getComponents()) {
			addComponentEntry(component, i++);
		}
	}
}

void Inspector::onMouseButtonPressed(int buttoncode, int mods) {
	EntityHandle target = input->getLastClicked();
	if (target.getEntity() != currentTarget.getEntity()) {
		inspect(target);
	}
}