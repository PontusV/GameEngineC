#ifndef HIERARCHY
#define HIERARCHY

#include <vector>
#include <unordered_map>
#include "entity/handle/Handle.h"
#include "scene/SceneManager.h"

namespace Core {
	class Scene;
}

namespace Editor {

	class Hierarchy {
	public:
		Hierarchy(Core::SceneManager* sceneManager);
		~Hierarchy();

		/* Renders ImGui window */
		void tick(Core::Handle& target);
		/* Updates scene and root orders */
		void update();
	private:
		std::size_t getRootIndex(Core::Scene* scene, const Core::Handle& handle);
		bool setRootIndex(Core::Scene* scene, const Core::Handle& handle, std::size_t index);
	private:
		Core::SceneManager* sceneManager;
		std::unordered_map<Core::ScenePtr, std::vector<Core::Handle>> sceneOrder;
	};
}

#endif