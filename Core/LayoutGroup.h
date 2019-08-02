#ifndef LAYOUT_GROUP_H
#define LAYOUT_GROUP_H
#include "LayoutController.h"
#include "ILayoutElement.h"
#include "Alignment.h"
#include <glm/glm.hpp>
#include "LayoutGroup.generated.h"

namespace Core {
	CLASS() LayoutGroup : public LayoutController, public ILayoutElement {
		GENERATED_BODY()
	protected:
		LayoutGroup();
	public:
		virtual ~LayoutGroup() = 0;

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
		glm::vec2 getAllocatedSpace(const std::vector<LayoutElementData>& elements);

		/* Returns LayoutElementData from all children with a RectTransform. */
		std::vector<LayoutElementData> getLayoutElementData(bool shrinkableChildWidth = true, bool shrinkableChildHeight = true);

		float getTotalMinWidth(const std::vector<LayoutElementData>& elements);
		float getTotalPrefWidth(const std::vector<LayoutElementData>& elements);
		float getTotalMinHeight(const std::vector<LayoutElementData>& elements);
		float getTotalPrefHeight(const std::vector<LayoutElementData>& elements);
	};
}
#endif