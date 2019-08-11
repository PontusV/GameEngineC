#ifndef LAYOUT_ELEMENT_H
#define LAYOUT_ELEMENT_H
#include "Component.h"
#include "ILayoutElement.h"
#include "Maths/Vector2.h"
#include "LayoutElement.generated.h"

namespace Core {

	/* Manages the size and position of immediate children. */
	CLASS() LayoutElement : public Component, public ILayoutElement {
		GENERATED_BODY()
	public:
		LayoutElement();
		~LayoutElement();

		/* Returns the minimum valid size for this element. */
		Vector2 getMinSize();
		/* Returns the prefered size for this element. The returned size will always be equal to or more than the minimum size. */
		Vector2 getPrefSize();
		/* Returns how much extra relative space this element may be allocated if there is additional available space. */
		Vector2 getFlexibleSize();

		void setMinSize(Vector2 size);
		void setPrefSize(Vector2 size);
		void setFlexibleSize(Vector2 size);

		void setMinSizeEnabled(bool value);
		bool getMinSizeEnabled();
		void setPrefSizeEnabled(bool value);
		bool getPrefSizeEnabled();
		void setFlexibleSizeEnabled(bool value);
		bool getFlexibleSizeEnabled();

	private:
		PROPERTY()
		Vector2 minSize = Vector2(1, 1);
		PROPERTY()
		Vector2 prefSize = Vector2(1, 1);
		PROPERTY()
		Vector2 flexibleSize = Vector2(1, 1); // Value between 0 - 1

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