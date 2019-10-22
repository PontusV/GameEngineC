#ifndef HIERARCHY_SCENE_MOVER_h
#define HIERARCHY_SCENE_MOVER_h
#include "components/Behaviour.h"
#include "maths/Vector2.h"
#include "entity/handle/EntityHandle.h"
#include "entity/handle/ComponentHandle.h"
#include "HierarchySceneMover.generated.h"
namespace Core {
	class Scene;

	/* Moves Scenes in the Hierarchy by drag and drop. */
	CLASS() HierarchySceneMover : public Behaviour {
		GENERATED_BODY()
	public:
		HierarchySceneMover(ComponentHandle hierarchy, Scene* scene);
		~HierarchySceneMover();

		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonReleased(int buttoncode, int mods) override;
		void onMouseDrag(float mouseX, float mouseY) override;
		void onDestroy() override;

	public:
		Scene* scene;
	private:
		bool dragging = false;
		Vector2 startPos;
		EntityHandle graphics;
		ComponentHandle hierarchy;
		static const float START_DRAG_DISTANCE;
	};
}
#endif