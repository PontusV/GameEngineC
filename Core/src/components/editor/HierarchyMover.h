#ifndef HIERARCHY_MOVER_h
#define HIERARCHY_MOVER_h
#include "components/Behaviour.h"
#include "maths/Vector2.h"
#include "entity/handle/EntityHandle.h"
#include "entity/handle/ComponentHandle.h"
#include "HierarchyMover.generated.h"
namespace Core {
	/* Moves Entities in the Hierarchy by drag and drop. */
	CLASS() HierarchyMover : public Behaviour {
		GENERATED_BODY()
	public:
		HierarchyMover(ComponentHandle hierarchy, EntityHandle target);
		~HierarchyMover();

		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonReleased(int buttoncode, int mods) override;
		void onMouseDrag(float mouseX, float mouseY) override;
		void onDestroy() override;

	public:
		EntityHandle target;
	private:
		bool dragging = false;
		Vector2 startPos;
		EntityHandle graphics;
		ComponentHandle hierarchy;
		static const float START_DRAG_DISTANCE;
	};
}
#endif