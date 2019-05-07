#ifndef WINDOW_ALIGNMENT_H
#define WINDOW_ALIGNMENT_H
#include "Script.h"
#include "Anchor.h"
#include <glm/glm.hpp>

namespace Core {

	class WindowAlignment : public Script {
	public:
		WindowAlignment();
		WindowAlignment(Anchor anchor, float offsetX = 0, float offsetY = 0);
		~WindowAlignment();

		void update(float deltaTime) override;

	public:
		glm::vec2 anchor;
		glm::vec2 offset;
		glm::vec2 resolution;
	};
}
#endif