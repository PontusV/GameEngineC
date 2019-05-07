#ifndef LAYOUT_ELEMENT_H
#define LAYOUT_ELEMENT_H
#include "Component.h"
#include "Alignment.h"
#include <glm/glm.hpp>

namespace Core {
	/* Manages the size and position of immediate children. */
	class LayoutElement : public Component {
	public:
		LayoutElement();
		~LayoutElement();

		/* Sets size of all BoxComponents attached to the owner. */
		void setSize(int width, int height); // Children may have to be resized as well though
		glm::ivec2 getSize();

	private:
		glm::ivec2 minSize;
		glm::ivec2 prefSize;
		glm::ivec2 maxSize;
	};
	class ContentSizeFitter; // Controlls only its own LayoutElement, gets Size of parent and stretches out
	// Padding
	// Horizontal Fit - Which value will the sizefitter aim to stretch the LayoutElement? Unconstrained, Min Size, Preferred Size
	// Vertical Fit - Same as above
	class AspectRatioFitter; // Same as ContentSizeFitter
	// Padding
	// Aspect Mode
	//	- Width controls Height
	//	- Height controls Width
	//	- Fit in Parent
	//	- Envelope Parent
	// Aspect Ratio
}
#endif