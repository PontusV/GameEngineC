#ifndef LAYOUT_GROUP_H
#define LAYOUT_GROUP_H
#include "LayoutController.h"
#include "ILayoutElement.h"
#include "Alignment.h"
#include <glm/glm.hpp>
#include "LayoutGroup.LayoutGroup.generated.h"

namespace Core {
	CLASS() LayoutGroup : public LayoutController, public ILayoutElement {
		GENERATED_BODY()
	protected:
		LayoutGroup();
	public:
		virtual ~LayoutGroup() = 0;

		/* Returns the minimum valid size for this element. */
		glm::vec2 getMinSize();
		/* Returns the prefered size for this element. The returned size will always be equal to or more than the minimum size. */
		glm::vec2 getPrefSize();
		/* Returns how much extra relative space this element may be allocated if there is additional available space. */
		glm::vec2 getFlexibleSize();

		PROPERTY()
		float paddingTop = 0;
		PROPERTY()
		float paddingRight = 0;
		PROPERTY()
		float paddingBottom = 0;
		PROPERTY()
		float paddingLeft = 0;
		PROPERTY()
		float spacing = 0;
		PROPERTY()
		Alignment childAlignment = Alignment::TOP_LEFT;

	protected:
		/* Returns the size of the allocated space for all immediate child LayoutElements */
		glm::vec2 getAllocatedSpace();

		/* Returns LayoutElementData from all children with a RectTransform. */
		std::vector<LayoutElementData> getLayoutElementData(bool shrinkableChildWidth = true, bool shrinkableChildHeight = true);

		float getTotalMinWidth(const std::vector<LayoutElementData>& elements);
		float getTotalPrefWidth(const std::vector<LayoutElementData>& elements);
		float getTotalMinHeight(const std::vector<LayoutElementData>& elements);
		float getTotalPrefHeight(const std::vector<LayoutElementData>& elements);
	};
}
#endif