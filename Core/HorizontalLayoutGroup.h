#ifndef HORIZONTAL_LAYOUT_GROUP_H
#define HORIZONTAL_LAYOUT_GROUP_H
#include "LayoutGroup.h"
#include <vector>
namespace Core {

	class HorizontalLayoutGroup : public LayoutGroup {
	public:
		HorizontalLayoutGroup();
		~HorizontalLayoutGroup();


		bool shrinkableChildWidth = true; // Determines if the childrens width can be shrunk below their original width
		bool shrinkableChildHeight = true; // Determines if the childrens height can be shrunk below their original height

		bool childForceExpandWidth = true;
		bool childForceExpandHeight = true;

	private:
		/* Attemps to stretch the width of all elements in the given vector so it fills all of the allocated width available to the elements. */
		void expandWidth(std::vector<LayoutElementData>& elements, float allocatedExpandWidth);
		void updateLayout() override;
	};
}
#endif