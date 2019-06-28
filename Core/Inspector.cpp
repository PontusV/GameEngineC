#include "Inspector.h"
#include "Input.h"
#include "VerticalLayoutGroup.h"
#include "RectTransform.h"
#include "RectSprite.h"
#include "Panel.h"
#include "LayoutElement.h"
#include "InputField.h"
#include "HideFlags.h"
#include "ReflectionPolymorph.generated.h"
#include <limits>

using namespace Core;

Inspector::Inspector()
{
}


Inspector::~Inspector()
{
}

std::string propertyValueToString(Mirror::Property& prop, Component* component) {
	if (prop.type.isChar()) {
		return std::to_string(Mirror::polyGetValue<char>(prop, component));
	}
	else if (prop.type.isNumber()) {
		if (prop.type.isDecimal()) {
			std::string result = std::to_string(Mirror::polyGetValue<double>(prop, component));
			// Removes trailing 0s
			std::size_t offset = result.at(result.find_last_not_of('0')) == '.' ? 2 : 1;
			result.erase(result.find_last_not_of('0') + offset, std::string::npos);
			return result;
		}
		else if (prop.type.isSignedNumber())
			return std::to_string(Mirror::polyGetValue<int>(prop, component));
		else if (prop.type.isUnsignedNumber())
			return std::to_string(Mirror::polyGetValue<unsigned int>(prop, component));
	}
	else if (prop.type.isBool()) {
		if (Mirror::polyGetValue<bool>(prop, component))
			return "true";
		else
			return "false";
	}
	else if (prop.type.isString()) {
		return Mirror::polyGetValue<std::string>(prop, component);
	}
	else if (prop.type.isVector()) {
		return "instance of std::vector<" + prop.type.getTemplateType().name + ">";
	}
	else if (prop.type.isObject()) {
		if (prop.type.name == "glm::vec2") {
			glm::vec2 size = Mirror::polyGetValue<glm::vec2>(prop, component);
			return "x: " + std::to_string(size.x) + ", y: " + std::to_string(size.y);
		}
		else {
			return "instance of " + prop.type.name;
		}
	}
	return "ERROR!";
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
			RectSprite(Color(150, 0, 0), 0),
			RectTransform(0, 0, 100, 100, rect->getZ() + 0.05f, Alignment::TOP_LEFT)
		);
		LayoutElement* element = scrollPanel.addComponent<LayoutElement>();
		element->setFlexibleSize(glm::vec2(1, 1));
		element->setPrefSize(glm::vec2(1, 1));
		element->setMinSize(glm::vec2(1, 1));
		element->setFlexibleSizeEnabled(true);
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
	static constexpr unsigned char layer = 0;
	RectTransform* rect = owner.getComponent<RectTransform>();

	// Create Field
	EntityHandle propField = createEntity(fieldName,
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
	);
	HorizontalLayoutGroup* fieldLayout = propField.addComponent<HorizontalLayoutGroup>();
	fieldLayout->spacing = 10;

	// -- Create Field Body

	if (prop.type.isNumber() && !prop.type.isCArray()) {
		// Property Name
		EntityHandle propLabel = createEntity(fieldName + "_Label");
		Text * propText = propLabel.addComponent(Text(prop.name + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255), layer));
		propLabel.addComponent(RectTransform(0, 0, propText->getSize().x, propText->getSize().y, rect->getZ() + 0.1f, Alignment::TOP_LEFT));
		propLabel.setParent(propField);

		// Input Field
		if (prop.type.isDecimal()) {
			double propertyValue = Mirror::polyGetValue<double>(prop, component);
			EntityHandle inputField = createEntity(fieldName + "_InputField",
				RectSprite(Color(255, 255, 255), layer),
				RectTransform(0, 0, 100, 16, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
			);
			InputField* inputFieldComponent = inputField.addComponent<InputField>();
			inputFieldComponent->setText(propertyValueToString(prop, component));
			inputFieldComponent->contentType = InputField::ContentType::Decimal;
			//inputFieldComponent->onSubmit = Core::bind(behaviour, functionPtr);
			inputField.setParent(propField);
		}
		else {
			int propertyValue = Mirror::polyGetValue<int>(prop, component);
			EntityHandle inputField = createEntity(fieldName + "_InputField",
				RectSprite(Color(255, 255, 255), layer),
				RectTransform(0, 0, 100, 16, rect->getZ() + 0.1f, Alignment::TOP_LEFT)
			);
			InputField* inputFieldComponent = inputField.addComponent<InputField>();
			inputFieldComponent->setText(propertyValueToString(prop, component));
			inputFieldComponent->contentType = InputField::ContentType::Decimal;
			//inputFieldComponent->onSubmit = Core::bind(behaviour, functionPtr);
			inputField.setParent(propField);
		}
	}
	else {
		// Property Name
		EntityHandle propLabel = createEntity(fieldName + "_Label");
		Text * propText = propLabel.addComponent(Text(prop.name + ":", "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255), layer));
		propLabel.addComponent(RectTransform(0, 0, propText->getSize().x, propText->getSize().y, rect->getZ() + 0.1f, Alignment::TOP_LEFT));
		propLabel.setParent(propField);

		// Property Value
		std::string propValueString = propertyValueToString(prop, component);
		EntityHandle propValue = createEntity(fieldName + "_Value");
		Text * propValueText = propValue.addComponent(Text(propValueString, "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255), layer));
		propValue.addComponent(RectTransform(0, 0, propValueText->getSize().x, propValueText->getSize().y, rect->getZ() + 0.1f, Alignment::TOP_LEFT));
		propValue.setParent(propField);
	}
	// -- End of Field Body

	return propField;
}

void Inspector::addComponentEntry(Component* component) {
	static constexpr unsigned char layer = 0;
	// Reflection Data
	Mirror::Class type = component->getType();
	// Check for reset for counter
	if (entryCount == std::numeric_limits<std::size_t>::max()) entryCount = 0;

	RectTransform* rect = owner.getComponent<RectTransform>();
	std::size_t entryIndex = entryCount++;
	std::string entryName = "Inspector_Component_" + std::to_string(entryIndex);

	// Entry
	EntityHandle entry = createEntity(entryName,
		RectSprite(Color(50, 50, 50), layer),
		RectTransform(0, 0, 0, 0, rect->getZ() + 0.09f, Alignment::TOP_LEFT)
	);
	LayoutElement* element = entry.addComponent<LayoutElement>();
	element->setFlexibleSize(glm::vec2(1, 1));
	element->setFlexibleSizeEnabled(true);
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
	Text* labelText = label.addComponent(Text(type.name, "resources/fonts/segoeui.ttf", 20, Color(255, 255, 255), layer));
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
		std::cout << "Inspecting " << entity.getEntityName() << "\n";
		// Clear old target component list
		clearEntries();
		// Get components and their reflection data
		for (Component* component : entity.getComponents()) {
			addComponentEntry(component);
		}
	}
}

void Inspector::onMouseButtonPressed(int buttoncode, int mods) {
	EntityHandle target = input->getLastClicked();
	if (target.getEntity() != currentTarget.getEntity()) {
		inspect(target);
	}
}