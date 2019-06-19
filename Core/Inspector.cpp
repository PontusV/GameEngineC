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
				if (offset < 0) offset = 0;
				for (std::size_t i = 0; i < offset; i++) {
					std::cout << " ";
				}
				std::cout << "type: " << prop.type.name << "\n";
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