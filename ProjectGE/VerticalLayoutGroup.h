#ifndef VERTICAL_LAYOUT_GROUP_H
#define VERTICAL_LAYOUT_GROUP_H
#include "LayoutController.h"
#include "LayoutGroup.h"
#include <vector>
namespace Core {

	class VerticalLayoutGroup : public LayoutGroup {
	public:
		VerticalLayoutGroup();
		~VerticalLayoutGroup();


		bool shrinkableChildWidth = true; // Determines if the childrens width can be shrunk below their original width
		bool shrinkableChildHeight = true; // Determines if the childrens height can be shrunk below their original height

		bool childForceExpandWidth = true;
		bool childForceExpandHeight = true;

	private:
		/* Attemps to stretch the height of all elements in the given vector so it fills all of the allocated height available to the elements. */
		void expandHeight(std::vector<LayoutElementData>& elements, float allocatedExpandHeight);
		void updateLayout() override;
	};
}
#endif