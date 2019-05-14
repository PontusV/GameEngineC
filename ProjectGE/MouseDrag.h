#ifndef MOUSE_DRAG_H
#define MOUSE_DRAG_H
#include "Behaviour.h"
#include <glm/glm.hpp>
namespace Core {
	class MouseDrag : public Behaviour {
	public:
		MouseDrag();
		~MouseDrag();

		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseDrag(float mouseX, float mouseY) override;

	private:
		glm::vec2 offset;
	};
}
#endif