#ifndef MOUSE_DRAG_H
#define MOUSE_DRAG_H
#include "Script.h"
namespace Core {
	class MouseDrag : public Script {
	public:
		MouseDrag();
		~MouseDrag();

		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseDrag(float mouseX, float mouseY) override;

	private:
		float offsetX;
		float offsetY;
	};
}
#endif