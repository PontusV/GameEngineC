#ifndef LAYOUT_ELEMENT_H
#define LAYOUT_ELEMENT_H
#include "Component.h"
#include "ILayoutElement.h"
#include <glm/glm.hpp>
#include "LayoutElement.LayoutElement.generated.h"

namespace Core {

	/* Manages the size and position of immediate children. */
	CLASS() LayoutElement : public Component, public ILayoutElement {
		GENERATED_BODY()
	public:
		LayoutElement();
		~LayoutElement();

		/* Returns the minimum valid size for this element. */
		glm::vec2 getMinSize();
		/* Returns the prefered size for this element. The returned size will always be equal to or more than the minimum size. */
		glm::vec2 getPrefSize();
		/* Returns how much extra relative space this element may be allocated if there is additional available space. */
		glm::vec2 getFlexibleSize();

		void setMinSize(glm::vec2 size);
		void setPrefSize(glm::vec2 size);
		void setFlexibleSize(glm::vec2 size);

		void setMinSizeEnabled(bool value);
		bool getMinSizeEnabled();
		void setPrefSizeEnabled(bool value);
		bool getPrefSizeEnabled();
		void setFlexibleSizeEnabled(bool value);
		bool getFlexibleSizeEnabled();

	private:
		PROPERTY()
		glm::vec2 minSize = glm::vec2(50,50);
		PROPERTY()
		glm::vec2 prefSize;
		PROPERTY()
		glm::vec2 flexibleSize; // Value between 0 - 1

		PROPERTY()
		bool minSizeEnabled = false;
		PROPERTY()
		bool prefSizeEnabled = false;
		PROPERTY()
		bool flexibleSizeEnabled = false;
	};
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