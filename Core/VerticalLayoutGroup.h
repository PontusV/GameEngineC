#ifndef VERTICAL_LAYOUT_GROUP_H
#define VERTICAL_LAYOUT_GROUP_H
#include "LayoutController.h"
#include "LayoutGroup.h"
#include <vector>
#include "VerticalLayoutGroup.VerticalLayoutGroup.generated.h"
namespace Core {

	CLASS() VerticalLayoutGroup : public LayoutGroup {
		GENERATED_BODY()
	public:
		VerticalLayoutGroup();
		~VerticalLayoutGroup();

		/* Returns the minimum valid size for this element. */
		glm::vec2 getMinSize();
		/* Returns the prefered size for this element. The returned size will always be equal to or more than the minimum size. */
		glm::vec2 getPrefSize();
		/* Returns how much extra relative space this element may be allocated if there is additional available space. */
		glm::vec2 getFlexibleSize();

		PROPERTY()
		bool shrinkableChildWidth = true; // Determines if the childrens width can be shrunk below their original width
		PROPERTY()
		bool shrinkableChildHeight = true; // Determines if the childrens height can be shrunk below their original height

		PROPERTY()
		bool childForceExpandWidth = true;
		PROPERTY()
		bool childForceExpandHeight = true;

	private:
		/* Attemps to stretch the height of all elements in the given vector so it fills all of the allocated height available to the elements. */
		void expandHeight(std::vector<LayoutElementData>& elements, float allocatedExpandHeight);
		void updateLayout() override;
	};
}
#endif