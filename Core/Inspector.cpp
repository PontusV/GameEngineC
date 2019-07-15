#include "Inspector.h"
#include "Input.h"
#include "VerticalLayoutGroup.h"
#include "RectTransform.h"
#include "RectSprite.h"
#include "Panel.h"
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



std::wstring Inspector::propertyValueToString(Mirror::Property& prop, Component* component) const {
	if (prop.type.isChar()) {
		return std::to_wstring(Mirror::polyGetValue<char>(prop, component));
	}
	else if (prop.type.isNumber()) {
		if (prop.type.isDecimal()) {
			std::string result = std::to_string(Mirror::polyGetValue<double>(prop, component));
			// Removes trailing 0s
			std::size_t offset = result.at(result.find_last_not_of('0')) == '.' ? 2 : 1;
			result.erase(result.find_last_not_of('0') + offset, std::string::npos);
			return std::wstring(result.begin(), result.end());
		}
		else if (prop.type.isSignedNumber())
			return std::to_wstring(Mirror::polyGetValue<int>(prop, component));
		else if (prop.type.isUnsignedNumber())
			return std::to_wstring(Mirror::polyGetValue<unsigned int>(prop, component));
	}
	else if (prop.type.isBool()) {
		if (Mirror::polyGetValue<bool>(prop, component))
			return L"true";
		else
			return L"false";
	}
	else if (prop.type.isString()) {
		std::string result = Mirror::polyGetValue<std::string>(prop, component);
		return std::wstring(result.begin(), result.end());
	}
	else if (prop.type.isWideString()) {
		return Mirror::polyGetValue<std::wstring>(prop, component);
	}
	else if (prop.type.isVector()) {
		return L"instance of std::vector<" + std::wstring(prop.type.getTemplateType().name.begin(), prop.type.getTemplateType().name.end()) + L">";
	}
	else if (prop.type.isObject()) {
		if (prop.type.name == "glm::vec2") {
			glm::vec2 size = Mirror::polyGetValue<glm::vec2>(prop, component);
			return L"x: " + std::to_wstring(size.x) + L", y: " + std::to_wstring(size.y);
		}
		else {
			return L"instance of " + std::wstring(prop.type.name.begin(), prop.type.name.end());
		}
	}
	return L"ERROR!";
}

bool Inspector::getSelectedProperty(PropertyInstance& ref) {
	std::size_t childCount = scrollPanel.getImmediateChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle entry = scrollPanel.getChild(i);
		std::size_t entryChildCount = entry.getImmediateChildCount();
		for (std::size_t ii = 0; ii < entryChildCount; ii++) {
			Handle propField = entry.getChild(ii);
			Selectable* selectable = propField.getComponent<Selectable>();
			if (selectable) {
				if (selectable->isSelected()) {
					// Component i, Property ii
					std::vector<Component*> components = currentTarget.getComponents();
					Component* instance = components[i];
					ref = { instance, instance->getType().properties[ii - 1] };
					return true;
				}
			}
		}
	}
	return false;
}

void Inspector::onPropertyValueSubmit(std::wstring value) {
	PropertyInstance propInstance;
	if (!getSelectedProperty(propInstance)) return;

	Component* instance = propInstance.instance;
	Mirror::Property prop = propInstance.prop;
	if (prop.type.isCArray()) {
		// WIP
		std::cout << "Inspector::onPropertyValueSubmit::ERROR Did not set value for the array type. Setting value of array types are still a WIP." << std::endl;
	}
	else {
		if (prop.type.isNumber()) {
			if (prop.type.isDecimal()) {
				Mirror::polySetValue(prop, instance, std::stod(value));
			}
			else {
				Mirror::polySetValue(prop, instance, std::stoi(value));
			}
		}
		else if (prop.type.isString()) {
			Mirror::polySetValue(prop, instance, std::string(value.begin(), value.end()));
		}
		else if (prop.type.isWideString()) {
			Mirror::polySetValue(prop, instance, value);
		}
		else {
			throw std::invalid_argument("Inspector::onPropertyValueSubmit::ERROR The property is not a valid type(" + prop.type.name + ") for this function.");
		}
	}
}

void Inspector::onPropertyValueToggle(bool value) {
	PropertyInstance propInstance;
	if (!getSelectedProperty(propInstance)) return;

	Component* instance = propInstance.instance;
	Mirror::Property prop = propInstance.prop;
	if (prop.type.isCArray()) {
		// WIP
		std::cout << "Inspector::onPropertyValueSubmit::ERROR Did not set value for the array type. Setting value of array types are still a WIP." << std::endl;
	}
	else {
		if (prop.type.isBool()) {
			Mirror::polySetValue(prop, instance, value);
		}
		else {
			throw std::invalid_argument("Inspector::onPropertyValueToggle::ERROR The property is not a valid type(" + prop.type.name + ") for this function.");
		}
	}
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
			Panel(),
			RectTransform(0, 0, 0, 0, rect->getZ() + 0.05f, Alignment::TOP_LEFT)
		);
		LayoutElement* element = scrollPanel.addComponent<LayoutElement>();
		element->setFlexibleSize(glm::vec2(1, 1));
		element->setFlexibleSizeEnabled(true);
		element->setPrefSizeEnabled(true);
		element->setMinSize(glm::vec2(400, 550/*470*/));
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


EntityHandle Inspector::createPropertyField(std::string fieldName, Mirror::Property& prop, Component* component) {
	RectTransform* rect = owner.getComponent<RectTransform>();

	// Create Field
	EntityHandle propField = createEntity(fieldName,
		Selectable(),
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
	);
	HorizontalLayoutGroup* fieldLayout = propField.addComponent<HorizontalLayoutGroup>();
	fieldLayout->spacing = 10;

	// -- Create Field Body

	if (prop.type.isNumber() && !prop.type.isCArray()) {
		// Property Name
		EntityHandle propLabel = createEntity(fieldName + "_Label");
		Text* propText = propLabel.addComponent(Text(prop.name + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propLabel.addComponent(RectTransform(0, 0, propText->getSize().x, propText->getSize().y, rect->getZ() + 0.1f, Alignment::LEFT));
		propLabel.setParent(propField);

		// Input Field
		EntityHandle inputField = createEntity(fieldName + "_InputField",
			RectSprite(Color(255, 255, 255)),
			Panel(),
			RectTransform(0, 0, 100, 16, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
		);
		InputField* inputFieldComponent = inputField.addComponent<InputField>();
		inputFieldComponent->setText(propertyValueToString(prop, component));
		if (prop.type.isDecimal())
			inputFieldComponent->contentType = InputField::ContentType::Decimal;
		else
			inputFieldComponent->contentType = InputField::ContentType::Integer;
		inputFieldComponent->onSubmit = Core::bind(this, &Inspector::onPropertyValueSubmit);
		inputField.setParent(propField);
	}
	else if ((prop.type.isString() || prop.type.isWideString()) && !prop.type.isCArray()) {
		// Property Name
		EntityHandle propLabel = createEntity(fieldName + "_Label");
		Text* propText = propLabel.addComponent(Text(prop.name + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propLabel.addComponent(RectTransform(0, 0, propText->getSize().x, propText->getSize().y, rect->getZ() + 0.1f, Alignment::LEFT));
		propLabel.setParent(propField);

		// Input Field
		EntityHandle inputField = createEntity(fieldName + "_InputField",
			RectSprite(Color(255, 255, 255)),
			Panel(),
			RectTransform(0, 0, 250, 16, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
		);
		InputField* inputFieldComponent = inputField.addComponent<InputField>();
		inputFieldComponent->setText(propertyValueToString(prop, component));
		inputFieldComponent->contentType = InputField::ContentType::Standard;
		inputFieldComponent->onSubmit = Core::bind(this, &Inspector::onPropertyValueSubmit);
		inputField.setParent(propField);
	}
	else if (prop.type.isBool() && !prop.type.isCArray()) {
		// Property Name
		EntityHandle propLabel = createEntity(fieldName + "_Label");
		Text* propText = propLabel.addComponent(Text(prop.name + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propLabel.addComponent(RectTransform(0, 0, propText->getSize().x, propText->getSize().y, rect->getZ() + 0.1f, Alignment::LEFT));
		propLabel.setParent(propField);

		// Property Value
		std::wstring propValueString = propertyValueToString(prop, component);
		EntityHandle propValue = createEntity(fieldName + "_Value",
			RectTransform(0, 0, 20, 20, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
		);
		CheckBox* checkBox = propValue.addComponent<CheckBox>();
		checkBox->setToggle(Mirror::polyGetValue<bool>(prop, component));
		checkBox->onToggle = Core::bind(this, &Inspector::onPropertyValueToggle);
		propValue.setParent(propField);
	}
	else {
		// Property Name
		EntityHandle propLabel = createEntity(fieldName + "_Label");
		Text * propText = propLabel.addComponent(Text(prop.name + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propLabel.addComponent(RectTransform(0, 0, propText->getSize().x, propText->getSize().y, rect->getZ() + 0.1f, Alignment::LEFT));
		propLabel.setParent(propField);

		// Property Value
		std::wstring propValueString = propertyValueToString(prop, component);
		EntityHandle propValue = createEntity(fieldName + "_Value");
		Text * propValueText = propValue.addComponent(Text(propValueString, "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propValue.addComponent(RectTransform(0, 0, propValueText->getSize().x, propValueText->getSize().y, rect->getZ() + 0.1f, Alignment::LEFT));
		propValue.setParent(propField);
	}
	// -- End of Field Body

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
	EntityHandle label = createEntity(entryName + "_Label");
	Text* labelText = label.addComponent(Text(type.name, "resources/fonts/segoeui.ttf", 20, Color(255, 255, 255)));
	label.addComponent(RectTransform(0, 0, labelText->getSize().x, labelText->getSize().y, rect->getZ() + 0.1f, Alignment::TOP_LEFT));
	label.setParent(entry);
	//*/
	// Property field
	for(std::size_t i = 0; i < type.properties.size(); i++) {
		EntityHandle propField = createPropertyField(entryName + "_Property_" + std::to_string(i), type.properties[i], component);
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