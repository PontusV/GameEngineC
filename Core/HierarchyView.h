#ifndef HIERARCHY_VIEW_H
#define HIERARCHY_VIEW_H
#include "Behaviour.h"
#include "HierarchyView.generated.h"
namespace Core {
	CLASS(hideInInspector) HierarchyView : public Behaviour {
		GENERATED_BODY()
	public:
		HierarchyView();
		~HierarchyView();

		void onEnable() override;
		void onDisable() override;
		void lateUpdate(float deltaTime) override;
		void onMouseButtonPressed(int buttoncode, int mods) override;

	private:
		EntityHandle currentTarget;
		EntityHandle scrollPanel;
		EntityHandle scrollBar;
	};
}
#endif