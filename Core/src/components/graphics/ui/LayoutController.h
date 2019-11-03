#ifndef LAYOUT_CONTROLLER_H
#define LAYOUT_CONTROLLER_H
#include "UIBehaviour.h"
#include "ILayoutElement.h"
#include "maths/Vector2.h"
#include "LayoutController.generated.h"
namespace Core {

	class LayoutElement;

	struct LayoutElementData {
		LayoutElementData(Handle child, Vector2 minSize, Vector2 preferredSize, Vector2 flexibleSize) : child(child), minSize(minSize), preferredSize(preferredSize), flexibleSize(flexibleSize) {}
		Vector2 size;
		Vector2 preStretchSize;

		Handle child;
		Vector2 minSize;
		Vector2 preferredSize;
		Vector2 flexibleSize;
	};

	CLASS() LayoutController : public UIBehaviour, public ILayoutElement {
		GENERATED_BODY()
	protected:
		LayoutController();

	public:
		virtual ~LayoutController() = 0;

		void onRectTransformResized() override;
		void onChildRectTransformResized() override;
		void onChildAdded(EntityHandle entity) override;
		void onChildRemoved(EntityHandle entity) override;
		void onChildChanged(EntityHandle entity) override;
		void onEnable() override;
		void onDisable() override;

		/* Updates layout and sets dirty to false. */
		void refresh();
		/* Marks all parents and self for layout update. */
		void setDirty();
		bool isDirty();
		bool isDirtySize();

	protected:
		/* Returns LayoutElement minSize from the given Entity. */
		static Vector2 getMinSize(Handle entity);
		/* Returns LayoutElement prefSize from the given Entity. */
		static Vector2 getPrefSize(Handle entity);
		/* Returns LayoutElement flexibleSize from the given Entity. */
		static Vector2 getFlexibleSize(Handle entity);
		/* Returns all components inheriting from the Layout Element Interface (ILayoutElement) */
		static std::vector<ILayoutElement*> getLayoutGroups(Handle entity);

		/* Returns the minimum valid size for this element. */
		Vector2 getMinSize();
		/* Returns the prefered size for this element. The returned size will always be equal to or more than the minimum size. */
		Vector2 getPrefSize();
		/* Returns how much extra relative space this element may be allocated if there is additional available space. */
		Vector2 getFlexibleSize();

	public:
		virtual void updateLayoutSizes() = 0;

	protected:
		Vector2 minSize;
		Vector2 prefSize;
		Vector2 flexibleSize;
		bool dirtySize; // Determines if the layout size preferences need to be updated

	private:
		virtual void updateLayout() = 0;

		bool dirty; // Determines if the layout needs to be updated
	};
}
#endif