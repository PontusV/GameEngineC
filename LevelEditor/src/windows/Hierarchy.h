#ifndef HIERARCHY
#define HIERARCHY

#include <vector>
#include <unordered_map>
#include <Core/Scene.h>
#include "GameView.h"

namespace Core {
	class IScene;
	class IEntityHandle;
	class ISceneManager;
}

namespace Editor {

	struct EntityHierarchy {
		EntityHierarchy() : entity(nullptr), children({}) {}
		EntityHierarchy(Core::IEntityHandle* entity, std::vector<EntityHierarchy> children) : entity(entity), children(children) {}
		Core::IEntityHandle* entity;
		std::vector<EntityHierarchy> children;
	};

	class LevelEditor;

	class Hierarchy {
	public:
		Hierarchy(LevelEditor* editor, GameView* gameView);
		~Hierarchy();

		/* Renders ImGui window */
		void tick(Core::IEntityHandle* target);
		/* Updates scene and root orders */
		void update();
		/* Initializes for the currently loaded Engine DLL */
		void initiate();
		/* Removes all currently added scenes */
		void clear();

		void onSceneAdded(Core::IScene* scene);
		void onSceneRemoved(Core::IScene* scene);
		void onSceneChanged(std::unordered_map<Core::IScene*, std::vector<EntityHierarchy>>::iterator& iterator);
	private:
		std::size_t getRootIndex(Core::IScene* scene, const Core::IEntityHandle* handle);
		bool setRootIndex(Core::IScene* scene, Core::IEntityHandle* handle, std::size_t index);
	private:
		LevelEditor* editor;
		GameView* gameView;
		std::unordered_map<Core::IScene*, std::vector<EntityHierarchy>> sceneOrder;
	};
}

#endif