#ifndef LAYOUT_ELEMENT_H
#define LAYOUT_ELEMENT_H
#include "Component.h"
#include "Bounds.h"
#include <glm/glm.hpp>

namespace Core {
	class LayoutElement : public Component {
	public:
		LayoutElement();
		~LayoutElement();

		/* Creates an Axis Aligned Bounding Box surrounding the Entity this component is attached to. */
		Bounds getBounds();

	private:

	};
}
#endif