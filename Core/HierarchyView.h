#ifndef HIERARCHY_VIEW_H
#define HIERARCHY_VIEW_H
#include "Behaviour.h"
#include "EntityHandle.h"
#include "ComponentHandle.h"
#include "HierarchyView.generated.h"
namespace Core {

	class RectTransform;

	CLASS(hideInInspector) HierarchyView : public Behaviour {
		GENERATED_BODY()
	public:
		HierarchyView(ComponentHandle editor);
		~HierarchyView();

		void onEnable() override;
		void onDisable() override;
		void lateUpdate(float deltaTime) override;
		void onMouseButtonPressed(int buttoncode, int mods) override;

		void refresh();
		void clearList();
		void addEntry(EntityHandle& entity, Handle& parent, RectTransform* rect);
		/* Filters out all entities who have a parent. */
		std::vector<EntityHandle> getRootEntities(std::vector<EntityHandle>& entities);
		std::vector<EntityHandle> getAllEntities();

		void onTargetEntityClick(EntityHandle entity);
		void onDestroyEntityClick(EntityHandle entity);
	private:
		float refreshTime = 0.2f;
		float timer = 0.0f;
	private:
		std::vector<std::pair<Entity, EntityHandle>> list; // List of current entries

		ComponentHandle editor;
		EntityHandle currentTarget;
	};
}
#endif