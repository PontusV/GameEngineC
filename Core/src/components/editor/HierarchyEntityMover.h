#ifndef HIERARCHY_MOVER_h
#define HIERARCHY_MOVER_h
#include "components/Behaviour.h"
#include "maths/Vector2.h"
#include "entity/handle/EntityHandle.h"
#include "entity/handle/ComponentHandle.h"
#include "HierarchyEntityMover.generated.h"
namespace Core {
	/* Moves Entities in the Hierarchy by drag and drop. */
	CLASS() HierarchyEntityMover : public Behaviour {
		GENERATED_BODY()
	public:
		HierarchyEntityMover(ComponentHandle hierarchy, EntityHandle target);
		~HierarchyEntityMover();

		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonReleased(int buttoncode, int mods) override;
		void onMouseDrag(float mouseX, float mouseY) override;
		void onHoverOver() override;
		void onHoverOut() override;
		void onDestroy() override;

		/* Called when a HierarchyEntry is moved over. */
		void onMoveOver();
		/* Called when a HierarchyEntry is moved out. */
		void onMoveOut();

	public:
		EntityHandle target;
	private:
		bool setOrder(Handle entity, EntityHandle parent, std::size_t order);
		Handle getEntityBelow();

		void hoverOut(EntityHandle entity);
		void hoverOver(EntityHandle entity);
	private:
		bool dragging = false;
		EntityHandle entityBelow;
		Vector2 startPos;
		EntityHandle graphics;
		ComponentHandle hierarchy;
		static const float START_DRAG_DISTANCE;
	};
}
#endif