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

		void refresh();
		void clearList();
		std::vector<EntityHandle> getAllEntities();

		void onDestroyEntityClick(EntityHandle entity);
	private:
		float refreshTime = 0.2f;
		float timer = 0.0f;
	private:
		std::vector<std::pair<Entity, EntityHandle>> list; // List of current entries

		EntityHandle currentTarget;
		EntityHandle scrollPanel;
		EntityHandle scrollBar;
	};
}
#endif