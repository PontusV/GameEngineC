#ifndef EDITOR_PANEL_H
#define EDITOR_PANEL_H
#include "Behaviour.h"
#include "EntityHandle.h"
#include "ComponentHandle.h"
#include "EditorPanel.generated.h"
namespace Core {
	CLASS(hideInInspector) EditorPanel : public Behaviour {
		GENERATED_BODY()
	private:
		enum class Tab {
			Inspector = 0,
			Hierarchy
		};
	public:
		EditorPanel();
		~EditorPanel();


		void start() override;
		void onMouseButtonPressed(int buttoncode, int mods) override;
		void selectTarget(EntityHandle target);
		void selectTab(Tab index);
	private:
		Tab currentTab;
		EntityHandle currentTarget;

		EntityHandle inspectorTab;
		EntityHandle hierarchyTab;
		ComponentHandle inspectorPanel;
		ComponentHandle hierarchyPanel;
	};
}
#endif