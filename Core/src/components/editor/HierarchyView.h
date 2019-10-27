#ifndef HIERARCHY_VIEW_H
#define HIERARCHY_VIEW_H
#include "components/Behaviour.h"
#include "entity/handle/EntityHandle.h"
#include "entity/handle/ComponentHandle.h"
#include "entity/handle/ComponentFunctionHandle.h"
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
	class Scene;

	CLASS(hideInInspector) HierarchyView : public Behaviour {
		GENERATED_BODY()
	public:
		HierarchyView(ComponentHandle editor);
		~HierarchyView();

		void onEnable() override;
		void onDisable() override;
		void lateUpdate(float deltaTime) override;

		void target(EntityHandle entity);
		std::size_t getRootIndex(const Handle& entity);
		void setRootIndex(const Handle& entity, std::size_t index);

	private:
		void refresh();
		void clearList();
		void updateOrder();
		std::vector<HierarchyEntry> getAllEntities();
		EntityHandle getEntryHandle(const Entity& entity);
		void onTargetEntityClick(EntityHandle entity);
		void onDestroyEntityClick(EntityHandle entity);

		bool isDirty(HierarchyEntry entity);

		void createEntityEntry(HierarchyEntry& entry, RectTransform* rect);
		void createSceneEntry(std::string name, Scene* scene, RectTransform* rect);
		EntityHandle createOrderRect(std::string name, EntityHandle entity, std::size_t order, RectTransform* rect);

	private:
		float refreshTime = 0.2f;
		float timer = 0.0f;
		Color highlightColor = Color(50, 50, 150, 255);

		std::vector<HierarchyEntry> list;
		std::map<Entity, EntryHandle> listMap;
		std::map<std::string, std::vector<Entity>> rootMap;
		std::map<std::string, EntityHandle> sceneMap;
		std::map<Entity, std::size_t> dirtyOrderMap;

		ComponentHandle editor;
		EntityHandle currentTarget;
	};
}
#endif