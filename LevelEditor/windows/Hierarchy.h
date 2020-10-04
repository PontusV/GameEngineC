#ifndef HIERARCHY
#define HIERARCHY

#include <vector>
#include <unordered_map>
#include "entity/handle/Handle.h"
#include "scene/SceneManager.h"
#include "GameView.h"

namespace Core {
	class Scene;
}

namespace Editor {

	class LevelEditor;

	class Hierarchy {
	public:
		Hierarchy(LevelEditor* editor, Core::SceneManager* sceneManager, GameView* gameView);
		~Hierarchy();

		/* Renders ImGui window */
		void tick(Core::Handle& target);
		/* Updates scene and root orders */
		void update();
	private:
		std::size_t getRootIndex(Core::Scene* scene, const Core::Handle& handle);
		bool setRootIndex(Core::Scene* scene, const Core::Handle& handle, std::size_t index);
	private:
		LevelEditor* editor;
		Core::SceneManager* sceneManager;
		GameView* gameView;
		std::unordered_map<Core::ScenePtr, std::vector<Core::Handle>> sceneOrder;
	};
}

#endif