#ifndef HIERARCHY_VIEW_H
#define HIERARCHY_VIEW_H
#include "components/Behaviour.h"
#include "entity/handle/EntityHandle.h"
#include "entity/handle/ComponentHandle.h"
#include "graphics/data/Color.h"
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

	struct EntryHandle {
		EntityHandle entry;
		EntityHandle highlight;
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

		void target(EntityHandle entity);

	private:
		void refresh();
		void clearList();
		void addEntry(EntityHandle& entity, Handle& parent, std::size_t depth, RectTransform* rect);
		std::vector<HierarchyEntry> getAllEntities();

		void onTargetEntityClick(EntityHandle entity);
		void onDestroyEntityClick(EntityHandle entity);

		bool isDirty(HierarchyEntry entity);
		std::size_t getOrder(EntityHandle& entity, EntityHandle& parent, std::vector<Handle>& rootEntities) const;
	private:
		float refreshTime = 0.2f;
		float timer = 0.0f;
		Color highlightColor = Color(50, 50, 150, 255);

		std::vector<HierarchyEntry> list;
		std::map<Entity, EntryHandle> listMap;

		ComponentHandle editor;
		EntityHandle currentTarget;
	};
}
#endif