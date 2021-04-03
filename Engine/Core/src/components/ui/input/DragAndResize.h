#ifndef DRAG_AND_RESIZE_H
#define DRAG_AND_RESIZE_H
#include "components/Behaviour.h"
#include "maths/Vector2.h"
#include "DragAndResize.generated.h"

namespace Core {

	/* Allows resizing of a RectTransform by pulling at its edges and corners */
	CLASS() DragAndResize : public Behaviour {
		GENERATED_BODY()
	public:
		DragAndResize();
		~DragAndResize();

		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseDrag(float mouseX, float mouseY) override;
		void onMouseButtonReleased(int buttoncode, int mods) override;

		PROPERTY()
		bool top = true;
		PROPERTY()
		bool right = true;
		PROPERTY()
		bool bottom = true;
		PROPERTY()
		bool left = true;
		PROPERTY()
		float edgeSize = 0;

	private:
		Vector2 mouseOffset; // Offset from top left vertex
		bool draggingTop = false;
		bool draggingRight = false;
		bool draggingBottom = false;
		bool draggingLeft = false;
	};
}
#endif