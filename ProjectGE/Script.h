#ifndef SCRIPT_H
#define SCRIPT_H
#include "Component.h"

namespace Core {
	class Script : public Component {
	public:
		virtual ~Script() = 0; // Abstract

		/* Called when the Script is enabled. */
		virtual void	start() {};
		/* Called every frame. */
		virtual void	update(float deltaTime) {};
		/* Called when this component is destroyed. */
		virtual void	onDestroy() {};

		// Interact
		virtual void	onMouseButtonPressed(int buttoncode, int mods) {}
		virtual void	onMouseButtonReleased(int buttoncode, int mods) {}
		virtual void	onMouseDrag(float mouseX, float mouseY) {}
		virtual void	onDoubleClick() {}
		virtual void	onHoverover() {}
		virtual void	onHoverout() {}
		virtual void	onScroll(float xoffset, float yoffset) {}

		bool active;
		bool enabled;
	protected:
		Script();
	};
}
#endif