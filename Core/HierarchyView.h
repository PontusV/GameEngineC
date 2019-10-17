#ifndef HIERARCHY_VIEW_H
#define HIERARCHY_VIEW_H
#include "Behaviour.h"
#include "EntityHandle.h"
#include "ComponentHandle.h"
#include <vector>
#include <map>
#include "HierarchyView.generated.h"
namespace Core {

	struct HierarchyEntry {
		HierarchyEntry(EntityHandle entity, EntityHandle parent, std::size_t order, std::size_t depth) : entity(entity), parent(parent), order(order), depth(depth) {}
		EntityHandle entity;
		EntityHandle parent;
		std::size_t order;
		std::size_t depth;
	};

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
		std::vector<HierarchyEntry> getAllEntities();

		void onTargetEntityClick(EntityHandle entity);
		void onDestroyEntityClick(EntityHandle entity);
	private:
		bool isDirty(HierarchyEntry entity);
		std::size_t getOrder(EntityHandle& entity, EntityHandle& parent, std::vector<Handle>& rootEntities);
	private:
		float refreshTime = 0.2f;
		float timer = 0.0f;

		std::vector<HierarchyEntry> list;
		std::map<Entity, EntityHandle> listMap;

		ComponentHandle editor;
		EntityHandle currentTarget;
	};
}
#endif