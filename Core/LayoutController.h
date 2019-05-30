#ifndef LAYOUT_CONTROLLER_H
#define LAYOUT_CONTROLLER_H
#include "UIBehaviour.h"
#include "ILayoutElement.h"
#include <glm/glm.hpp>
#include "LayoutController.LayoutController.generated.h"
namespace Core {

	class LayoutElement;

	struct LayoutElementData {
		LayoutElementData(Handle child, glm::vec2 minSize, glm::vec2 preferredSize, glm::vec2 flexibleSize) : child(child), minSize(minSize), preferredSize(preferredSize), flexibleSize(flexibleSize) {}
		glm::vec2 size;
		glm::vec2 preStretchSize;

		Handle child;
		glm::vec2 minSize;
		glm::vec2 preferredSize;
		glm::vec2 flexibleSize;
	};

	CLASS() LayoutController : public UIBehaviour {
		GENERATED_BODY()
	protected:
		LayoutController();

	public:
		virtual ~LayoutController() = 0;

		void onRectTransformResized() override;
		void onChildRectTransformResized() override;
		void onChildrenChanged() override;

		/* Updates layout and sets dirty to false. */
		void refresh();
		void setDirty();
		bool isDirty();

	protected:
		/* Returns LayoutElement minSize from the given Entity. */
		static glm::vec2 getMinSize(Handle entity);
		/* Returns LayoutElement prefSize from the given Entity. */
		static glm::vec2 getPrefSize(Handle entity);
		/* Returns LayoutElement flexibleSize from the given Entity. */
		static glm::vec2 getFlexibleSize(Handle entity);
		/* Returns all components inheriting from the Layout Element Interface (ILayoutElement) */
		static std::vector<ILayoutElement*> getLayoutElements(Handle entity);

	private:
		virtual void updateLayout() = 0;

		bool dirty; // Determines if the layout needs to be updated
	};
}
#endif