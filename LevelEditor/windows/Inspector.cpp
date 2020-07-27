#include "Inspector.h"
#include "imgui/imgui.h"
#include "ReflectionPolymorph.generated.h"

using namespace Editor;
using namespace Core;

Inspector::Inspector(GameView* gameView) : gameView(gameView) {}
Inspector::~Inspector() {}

void Inspector::tick() {
	EntityHandle target = gameView->getTarget();

	ImGui::Begin("Inspector");

	if (target.isValid()) {
		ImGui::Text(target.getEntityName().c_str());

		for (Component* component : target.getComponents()) {
			Mirror::Class type = component->getType();

			ImGui::Text(type.name.c_str());

			for (Mirror::Property& prop : type.properties) {
				ImGui::Text(prop.name.c_str());
			}

		}
	}
	else {
		ImGui::Text("No target");
	}

	ImGui::End();
}