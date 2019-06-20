#include "Inspector.h"
#include "Input.h"
#include "VerticalLayoutGroup.h"
#include "RectTransform.h"
#include "RectSprite.h"
#include "Panel.h"
#include "LayoutElement.h"
#include "ContentSizeFitter.h"
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
			RectSprite(Color(150, 0, 0), 0),
			RectTransform(0, 0, 100, 100, rect->getZ() + 0.05f, Alignment::TOP_LEFT)
		);
		LayoutElement* element = scrollPanel.addComponent<LayoutElement>();
		element->setFlexibleSize(glm::ivec2(1,1));
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

void Inspector::addComponentEntry(Component* component) {
	// Reflection Data
	Mirror::Class type = component->getType();
	// Check for reset for counter
	if (entryCount == std::numeric_limits<std::size_t>::max()) entryCount = 0;
	static constexpr unsigned char layer = 0;
	static constexpr int height = 1;
	static constexpr int yOffset = 10;
	static constexpr int xPadding = 10;
	static constexpr int yPadding = 10;

	RectTransform* rect = owner.getComponent<RectTransform>();
	std::size_t entryIndex = entryCount++;
	std::string entryName = "Inspector_Component_" + std::to_string(entryIndex);

	// Entry
	EntityHandle entry = createEntity(entryName,
		RectSprite(Color(150, 150, 150), layer),
		RectTransform(0, 0, rect->getSize().x - xPadding * 2, height, rect->getZ() + 0.09f, Alignment::TOP_LEFT)
	);
	LayoutElement* element = entry.addComponent<LayoutElement>();
	element->setFlexibleSize(glm::vec2(1, 1));
	element->setFlexibleSizeEnabled(true);
	element->setPrefSize(glm::vec2(1, 1));
	ContentSizeFitter* fit = entry.addComponent<ContentSizeFitter>();
	fit->horizontalFit = ContentSizeFitter::PREFERREDSIZE;
	fit->verticalFit = ContentSizeFitter::PREFERREDSIZE;
	VerticalLayoutGroup* group = entry.addComponent<VerticalLayoutGroup>();
	group->paddingTop = 5;
	group->paddingLeft = 5;
	group->paddingRight = 5;
	group->paddingBottom = 5;
	entry.setParent(scrollPanel);

	// Entry content
	EntityHandle label = createEntity(entryName + "_Label");
	Text* labelText = label.addComponent(Text(type.name, "resources/fonts/segoeui.ttf", 20, Color(255, 255, 255), layer));
	RectTransform* labelRect = label.addComponent(RectTransform(0, 0, labelText->getSize().x, labelText->getSize().y, rect->getZ() + 0.1f, Alignment::TOP_LEFT));
	label.setParent(entry);
	// Property field
	for (const Mirror::Property& prop : type.properties) {

	}
	// End of Entry content
	targetComponentList.push_back(entry);

	// Print reflection data
	std::cout << "\tComponent: " << type.name << "\n";
	for (Mirror::Property& prop : type.properties) {
		std::cout << "\t\tProperty: " << prop.name << ", ";
		std::size_t offset = 30 - prop.name.size();
		if (30 < prop.name.size()) offset = 0;
		for (std::size_t i = 0; i < offset; i++) {
			std::cout << " ";
		}
		std::cout << "type: " << prop.type.name << ", ";
		std::size_t offset2 = 30 - prop.type.name.size();
		if (30 < prop.type.name.size()) offset2 = 0;
		for (std::size_t i = 0; i < offset2; i++) {
			std::cout << " ";
		}
		std::cout << "value: ";
		if (prop.type.isChar()) {
			std::cout << std::to_string(polyGetValue<char>(prop, component));
		}
		else if (prop.type.isNumber()) {
			if (prop.type.name == "float")
				std::cout << std::to_string(polyGetValue<float>(prop, component));
			else if (prop.type.isSignedNumber())
				std::cout << std::to_string(polyGetValue<int>(prop, component));
			else if (prop.type.isUnsignedNumber())
				std::cout << std::to_string(polyGetValue<unsigned int>(prop, component));
		}
		else if (prop.type.isBool()) {
			if (polyGetValue<bool>(prop, component))
				std::cout << "true";
			else
				std::cout << "false";
		}
		else if (prop.type.isString()) {
			std::cout << polyGetValue<std::string>(prop, component);
		}
		else if (prop.type.isVector()) {
			std::cout << "instance of std::vector<" + prop.type.getTemplateType().name + ">";
		}
		else if (prop.type.isObject()) {
			if (prop.type.name == "glm::vec2") {
				glm::vec2 size = polyGetValue<glm::vec2>(prop, component);
				std::cout << "x: " << size.x << ", y: " << size.y;
			}
			else {
				std::cout << "instance of " << prop.type.name;
			}
		}
		else {
			std::cout << "ERROR!";
		}
		std::cout << std::endl;
	}
}

void Inspector::inspect(EntityHandle entity) {
	if (entity.refresh()) {
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