#ifndef WINDOW_ANCHOR_H
#define WINDOW_ANCHOR_H
#include "UIBehaviour.h"
#include "Anchor.h"
#include <glm/glm.hpp>

namespace Core {

	/* Manages the position of a Transform component. The position depends on the anchor value, the size of the window and the offset. */
	class WindowAnchor : public UIBehaviour {
	public:
		WindowAnchor();
		WindowAnchor(Anchor anchor, float offsetX = 0, float offsetY = 0);
		~WindowAnchor();

		void awake() override;
		void onWindowResize() override;

	public:
		glm::vec2 anchor;
		glm::vec2 offset;
	};
}
#endif