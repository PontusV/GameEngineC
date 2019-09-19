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

		PROPERTY()
		bool shrinkableChildWidth = false; // Determines if the childrens width can be shrunk below their original width
		PROPERTY()
		bool shrinkableChildHeight = false; // Determines if the childrens height can be shrunk below their original height

		PROPERTY()
		bool childForceExpandWidth = false;
		PROPERTY()
		bool childForceExpandHeight = false;

	private:
		/* Attemps to stretch the width of all elements in the given vector so it fills all of the allocated width available to the elements. */
		void expandWidth(std::vector<LayoutElementData>& elements, float allocatedExpandWidth);
		void updateLayout() override;
		void updateLayoutSizes() override;
	};
}
#endif