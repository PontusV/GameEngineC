#include "Inspector.h"
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


template<typename T>
std::wstring toWString(T value);

// Char
template<>
std::wstring toWString<char>(char value) {
	return std::to_wstring(value);
}

// Number
template<>
std::wstring toWString<int>(int value) {
	return std::to_wstring(value);
}

// Unsigned Number
template<>
std::wstring toWString<unsigned int>(unsigned int value) {
	return std::to_wstring(value);
}

// Decimal Number
template<>
std::wstring toWString<double>(double value) {
	std::wstring result = std::to_wstring(value);
	// Removes trailing 0s
	std::size_t offset = result.at(result.find_last_not_of('0')) == '.' ? 2 : 1;
	result.erase(result.find_last_not_of('0') + offset, std::string::npos);
	return result;
}

// Bool
template<>
std::wstring toWString<bool>(bool value) {
	if (value)
		return L"true";
	else
		return L"false";
}

// String
template<>
std::wstring toWString<std::string>(std::string value) {
	return std::wstring(value.begin(), value.end());
}

// WString
template<>
std::wstring toWString<std::wstring>(std::wstring value) {
	return value;
}

std::wstring Inspector::propertyValueToString(Mirror::Property& prop, Component* component) const {
	if (prop.type.isChar()) {
		return toWString(Mirror::polyGetValue<char>(prop, component));
	}
	else if (prop.type.isNumber()) {
		if (prop.type.isDecimal()) {
			return toWString(Mirror::polyGetValue<double>(prop, component));
		}
		else if (prop.type.isSignedNumber())
			return toWString(Mirror::polyGetValue<int>(prop, component));
		else if (prop.type.isUnsignedNumber())
			return toWString(Mirror::polyGetValue<unsigned int>(prop, component));
	}
	else if (prop.type.isBool()) {
		return toWString(Mirror::polyGetValue<bool>(prop, component));
	}
	else if (prop.type.isString()) {
		return toWString(Mirror::polyGetValue<std::string>(prop, component));
	}
	else if (prop.type.isWideString()) {
		return Mirror::polyGetValue<std::wstring>(prop, component);
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

std::wstring Inspector::propertyArrayValueToString(Mirror::Property& prop, Component* component, std::size_t index) const {
	// WIP
	return L"ERROR!";
}

bool Inspector::getSelectedProperty(PropertyInstance& ref) {
	std::size_t childCount = scrollPanel.getImmediateChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		EntityHandle entry = scrollPanel.getChild(i);
		std::size_t entryChildCount = entry.getImmediateChildCount();
		for (std::size_t ii = 0; ii < entryChildCount; ii++) {
			EntityHandle propField = entry.getChild(ii);
			if (propField.getComponent<Selectable>()->isSelected()) {
				// Component i, Property ii
				std::size_t lineIndex = 0;
				bool lineFound = false;

				// Get selected line index of the propField
				std::size_t totalLineAmount = propField.getImmediateChildCount();
				for (std::size_t currentLineIndex = 0; currentLineIndex < totalLineAmount; currentLineIndex++) {
					EntityHandle line = propField.getChild(currentLineIndex);
					if (line.getComponent<Selectable>()->isSelected()) {
						lineIndex = currentLineIndex;
						lineFound = true;
						break;
					}
				}
				if (!lineFound) throw "Inspector::getSelectedProperty::ERROR Selected line was not found!";

				std::vector<Component*> components = currentTarget.getComponents();
				Component* instance = components[i];
				ref = { lineIndex, instance, instance->getType().properties[ii - 1] };
				return true;
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
		std::size_t lineIndex = propInstance.lineIndex;
		//auto arrayType = Mirror::polyGetArrayValue<std::string>(prop, instance);
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
			std::cout << "Inspector::onPropertyValueToggle::ERROR The property is not a valid type(" + prop.type.name + ") for this function." << std::endl;
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
			RectMask(),
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

EntityHandle Inspector::createPropertyFieldLine(std::string propName, Mirror::VariableType& propType, std::wstring propValue, std::string lineName) {
	RectTransform* rect = owner.getComponent<RectTransform>();
	EntityHandle propLine = createEntity(lineName,
		Selectable(),
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.2f, Alignment::TOP_LEFT)
	);
	HorizontalLayoutGroup* fieldLayout = propLine.addComponent<HorizontalLayoutGroup>();
	fieldLayout->spacing = 10;

	if (propType.isNumber()) {
		// Property Name
		EntityHandle propLabel = createEntity(lineName + "_Label");
		Text* propText = propLabel.addComponent(Text(propName + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propLabel.addComponent(RectTransform(0, 0, propText->getSize().x, propText->getSize().y, rect->getZ() + 0.1f, Alignment::LEFT));
		propLabel.setParent(propLine);

		// Input Field
		EntityHandle inputField = createEntity(lineName + "_InputField",
			RectSprite(Color(255, 255, 255)),
			RectMask(),
			RectTransform(0, 0, 100, 16, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
		);
		InputField* inputFieldComponent = inputField.addComponent<InputField>();
		inputFieldComponent->setText(propValue);
		if (propType.isDecimal())
			inputFieldComponent->contentType = InputField::ContentType::Decimal;
		else
			inputFieldComponent->contentType = InputField::ContentType::Integer;
		inputFieldComponent->onSubmit = Core::bind(this, &Inspector::onPropertyValueSubmit);
		inputField.setParent(propLine);
	}
	else if (propType.isString() || propType.isWideString()) {
		// Property Name
		EntityHandle propLabel = createEntity(lineName + "_Label");
		Text* propText = propLabel.addComponent(Text(propName + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propLabel.addComponent(RectTransform(0, 0, propText->getSize().x, propText->getSize().y, rect->getZ() + 0.1f, Alignment::LEFT));
		propLabel.setParent(propLine);

		// Input Field
		EntityHandle inputField = createEntity(lineName + "_InputField",
			RectSprite(Color(255, 255, 255)),
			RectMask(),
			RectTransform(0, 0, 250, 16, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
		);
		InputField* inputFieldComponent = inputField.addComponent<InputField>();
		inputFieldComponent->setText(propValue);
		inputFieldComponent->contentType = InputField::ContentType::Standard;
		inputFieldComponent->onSubmit = Core::bind(this, &Inspector::onPropertyValueSubmit);
		inputField.setParent(propLine);
	}
	else if (propType.isBool()) {
		// Property Name
		EntityHandle propLabel = createEntity(lineName + "_Label");
		Text* propText = propLabel.addComponent(Text(propName + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propLabel.addComponent(RectTransform(0, 0, propText->getSize().x, propText->getSize().y, rect->getZ() + 0.1f, Alignment::LEFT));
		propLabel.setParent(propLine);

		// Property Value
		EntityHandle propValueField = createEntity(lineName + "_Value",
			RectTransform(0, 0, 20, 20, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
		);
		CheckBox* checkBox = propValueField.addComponent<CheckBox>();
		checkBox->setToggle(propValue == L"true");
		checkBox->onToggle = Core::bind(this, &Inspector::onPropertyValueToggle);
		propValueField.setParent(propLine);
	}
	// -------- DEFAULT --------------
	else {
		// Property Name
		EntityHandle propLabel = createEntity(lineName + "_Label");
		Text* propText = propLabel.addComponent(Text(propName + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propLabel.addComponent(RectTransform(0, 0, propText->getSize().x, propText->getSize().y, rect->getZ() + 0.1f, Alignment::LEFT));
		propLabel.setParent(propLine);

		// Property Value
		EntityHandle propValueField = createEntity(lineName + "_Value");
		Text* propValueText = propValueField.addComponent(Text(propValue, "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255)));
		propValueField.addComponent(RectTransform(0, 0, propValueText->getSize().x, propValueText->getSize().y, rect->getZ() + 0.1f, Alignment::LEFT));
		propValueField.setParent(propLine);
	}
	return propLine;
}

template<typename T>
static void createPropertyFieldLines(Inspector* inspector, EntityHandle& field, Mirror::Property& prop, Component* component) {
	std::size_t i = 0;
	for (T value : Mirror::polyGetArrayValue<T>(prop, component)) {
		EntityHandle line = inspector->createPropertyFieldLine(prop.name + "[" + std::to_string(i) + "]", prop.type, toWString(value), field.getEntityName() + "_line_" + std::to_string(i));
		line.setParent(field);
		i++;
	}
}

EntityHandle Inspector::createPropertyField(std::string name, Mirror::Property& prop, Component* component) {
	RectTransform* rect = owner.getComponent<RectTransform>();

	// Create Field
	EntityHandle propField = createEntity(name,
		Selectable(),
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
	);
	VerticalLayoutGroup* fieldLayout = propField.addComponent<VerticalLayoutGroup>();
	fieldLayout->spacing = 3;

	// Create Field Body
	if (Mirror::isArrayType(prop.type)) {
		Mirror::VariableType& type = prop.type;
		if (prop.type.name == "std::vector") {
			type = prop.type.templateParams[0];
		}
		else if (prop.type.name == "std::array") {
			type = prop.type.templateParams[0];
		}
		//
		if (type.isChar()) {
			createPropertyFieldLines<char>(this, propField, prop, component);
		}
		else if (type.isNumber()) {
			if (type.isDecimal()) {
				createPropertyFieldLines<double>(this, propField, prop, component);
			}
			else if (type.isSignedNumber()) {
				createPropertyFieldLines<int>(this, propField, prop, component);
			}
			else if (type.isUnsignedNumber()) {
				createPropertyFieldLines<unsigned int>(this, propField, prop, component);
			}
		}
		else if (type.isBool()) {
			createPropertyFieldLines<bool>(this, propField, prop, component);
		}
		else if (type.isString()) {
			createPropertyFieldLines<std::string>(this, propField, prop, component);
		}
		else if (type.isWideString()) {
			createPropertyFieldLines<std::wstring>(this, propField, prop, component);
		}
		else if (type.name == "glm::vec2") {

		}
		else if (Mirror::isReflected(type.name)) {
			// WIP
			//Mirror::getType(prop.type.name);
			//TODO: get pointer of property
			Mirror::Class classType = Mirror::getType(type.name);
			//void* instance = prop.getPointer(component);
			std::vector<void*> instances = prop.getArrayElementPointers(component);
			std::size_t instanceIndex = 0;

			for (void* instance : instances) {
				for (std::size_t i = 0; i < classType.properties.size(); i++) {
					EntityHandle innerField = createPropertyField(name + "_Reflected_Property_" + std::to_string(instanceIndex) + "_Property_" + std::to_string(i), classType.properties[i], static_cast<Component*>(instance));
					innerField.setParent(propField);
				}
				instanceIndex++;
			}
		}
		else {
			std::cout << "Inspector: Unknown property type(" << type.name << ")" << std::endl;
		}

	}
	else {
		//createPropertyLine
		EntityHandle line = createPropertyFieldLine(prop.name, prop.type, propertyValueToString(prop, component), name + "_line");
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