#ifndef LAYOUT_CONTROLLER_H
#define LAYOUT_CONTROLLER_H
#include "UIBehaviour.h"
#include "ILayoutElement.h"
#include "Maths/Vector2.h"
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

	CLASS() LayoutController : public UIBehaviour {
		GENERATED_BODY()
	protected:
		LayoutController();

	public:
		virtual ~LayoutController() = 0;

		void onRectTransformResized() override;
		void onChildRectTransformResized() override;
		void onChildAdded(Handle entity) override;
		void onChildRemoved(Handle entity) override;

		/* Updates layout and sets dirty to false. */
		void refresh();
		void setDirty();
		bool isDirty();

	protected:
		/* Returns LayoutElement minSize from the given Entity. */
		static Vector2 getMinSize(Handle entity);
		/* Returns LayoutElement prefSize from the given Entity. */
		static Vector2 getPrefSize(Handle entity);
		/* Returns LayoutElement flexibleSize from the given Entity. */
		static Vector2 getFlexibleSize(Handle entity);
		/* Returns all components inheriting from the Layout Element Interface (ILayoutElement) */
		static std::vector<ILayoutElement*> getLayoutGroups(Handle entity);

	private:
		virtual void updateLayout() = 0;

		bool dirty; // Determines if the layout needs to be updated
	};
}
#endif