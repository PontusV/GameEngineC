#ifndef HORIZONTAL_LAYOUT_GROUP_H
#define HORIZONTAL_LAYOUT_GROUP_H
#include "LayoutGroup.h"
#include <vector>
#include "HorizontalLayoutGroup.generated.h"

namespace Core {

	CLASS() HorizontalLayoutGroup : public LayoutGroup {
		GENERATED_BODY()
	public:
		HorizontalLayoutGroup();
		~HorizontalLayoutGroup();

		/* Returns the minimum valid size for this element. */
		Vector2 getMinSize();
		/* Returns the prefered size for this element. The returned size will always be equal to or more than the minimum size. */
		Vector2 getPrefSize();
		/* Returns how much extra relative space this element may be allocated if there is additional available space. */
		Vector2 getFlexibleSize();

		PROPERTY()
		bool shrinkableChildWidth = true; // Determines if the childrens width can be shrunk below their original width
		PROPERTY()
		bool shrinkableChildHeight = true; // Determines if the childrens height can be shrunk below their original height

		PROPERTY()
		bool childForceExpandWidth = true;
		PROPERTY()
		bool childForceExpandHeight = true;

	private:
		/* Attemps to stretch the width of all elements in the given vector so it fills all of the allocated width available to the elements. */
		void expandWidth(std::vector<LayoutElementData>& elements, float allocatedExpandWidth);
		void updateLayout() override;
	};
}
#endif