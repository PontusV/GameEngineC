#include "Inspector.h"
#include "Input.h"
#include "VerticalLayoutGroup.h"

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

	}
}

void Inspector::onMouseButtonPressed(int buttoncode, int mods) {
	EntityHandle target = input->getLastClicked();
	if (target.getEntity() != currentTarget.getEntity()) {
		inspect(target);
	}
}