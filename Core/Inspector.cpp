#include "Inspector.h"
#include "Input.h"
#include "VerticalLayoutGroup.h"
#include "ReflectionPolymorph.generated.h"

using namespace Core;

Inspector::Inspector()
{
}


Inspector::~Inspector()
{
}

void Inspector::awake() {
	VerticalLayoutGroup* layoutGroup = owner.addComponent<VerticalLayoutGroup>();

}

void Inspector::inspect(EntityHandle entity) {
	if (entity.refresh()) {
		currentTarget = entity;
		std::cout << "Inspecting " << entity.getEntityName() << "\n";
		// Get components and their reflection data
		for (Component* component : entity.getComponents()) {
			Mirror::Class type = component->getType();
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
					std::cout << "instance of " << prop.type.name;
				}
				else {
					std::cout << "ERROR!";
				}
				std::cout << std::endl;
			}
		}
	}
}

void Inspector::onMouseButtonPressed(int buttoncode, int mods) {
	EntityHandle target = input->getLastClicked();
	if (target.getEntity() != currentTarget.getEntity()) {
		inspect(target);
	}
}