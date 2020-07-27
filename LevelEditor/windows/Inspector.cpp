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
		ImGui::BeginChild(target.getEntityName().c_str(), ImVec2(0, 0));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
		for (Component* component : target.getComponents()) {
			Mirror::Class type = component->getType();

			if (ImGui::CollapsingHeader(type.name.c_str())) {
				for (Mirror::Property& prop : type.properties) {
					ImGui::Text(prop.name.c_str());
				}
			}
		}
		ImGui::PopStyleVar();
		ImGui::EndChild();
	}
	else {
		ImGui::Text("No target");
	}

	ImGui::End();
}