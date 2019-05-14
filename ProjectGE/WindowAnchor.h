#ifndef WINDOW_ANCHOR_H
#define WINDOW_ANCHOR_H
#include "UIBehaviour.h"
#include "Anchor.h"
#include <glm/glm.hpp>

namespace Core {

	class WindowAnchor : public UIBehaviour {
	public:
		WindowAnchor();
		WindowAnchor(Anchor anchor, float offsetX = 0, float offsetY = 0);
		~WindowAnchor();

		void start() override;
		void onWindowResize() override;

	public:
		glm::vec2 anchor;
		glm::vec2 offset;
	};
}
#endif