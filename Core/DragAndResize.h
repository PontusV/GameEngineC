#ifndef DRAG_AND_RESIZE_H
#define DRAG_AND_RESIZE_H
#include "Behaviour.h"
#include <glm/glm.hpp>
namespace Core {

	/* Allows resizing of a RectTransform by pulling at its edges and corners */
	class DragAndResize : public Behaviour {
	public:
		DragAndResize();
		~DragAndResize();

		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseDrag(float mouseX, float mouseY) override;
		void onMouseButtonReleased(int buttoncode, int mods) override;

		bool top = true;
		bool right = true;
		bool bottom = true;
		bool left = true;
		float edgeSize = 0;

	private:
		glm::vec2 mouseOffset; // Offset from top left vertex
		bool draggingTop = false;
		bool draggingRight = false;
		bool draggingBottom = false;
		bool draggingLeft = false;
	};
}
#endif