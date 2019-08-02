#ifndef MOUSE_DRAG_H
#define MOUSE_DRAG_H
#include "Behaviour.h"
#include <glm/glm.hpp>
#include "MouseDrag.generated.h"
namespace Core {
	CLASS() MouseDrag : public Behaviour {
		GENERATED_BODY()
	public:
		MouseDrag();
		~MouseDrag();

		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonReleased(int buttoncode, int mods) override;
		void onMouseDrag(float mouseX, float mouseY) override;


		float padding = 0; // Padding around the area that triggers the drag event

	private:
		bool dragging = false;
		glm::vec2 offset;
	};
}
#endif