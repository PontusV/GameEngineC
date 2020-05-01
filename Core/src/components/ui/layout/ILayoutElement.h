#ifndef INTERFACE_LAYOUT_ELEMENT_H
#define INTERFACE_LAYOUT_ELEMENT_H
#include "maths/Vector2.h"

namespace Core {

	/* An interface for components that may be considered elements in a layout. */
	class ILayoutElement {
	protected:
		ILayoutElement() {}
	public:
		virtual ~ILayoutElement() {};

		/* Returns the minimum valid size for this element. */
		virtual Vector2 getMinSize() = 0;
		/* Returns the prefered size for this element. The returned size will always be equal to or more than the minimum size. */
		virtual Vector2 getPrefSize() = 0;
		/* Returns how much extra relative space this element may be allocated if there is additional available space. */
		virtual Vector2 getFlexibleSize() = 0;
	};
}
#endif