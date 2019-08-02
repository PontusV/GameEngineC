#ifndef WINDOW_ANCHOR_H
#define WINDOW_ANCHOR_H
#include "UIBehaviour.h"
#include "Anchor.h"
#include <glm/glm.hpp>
#include "WindowAnchor.generated.h"
namespace Core {

	/* Manages the position of a Transform component. The position depends on the anchor value, the size of the window and the offset. */
	CLASS() WindowAnchor : public UIBehaviour {
		GENERATED_BODY()
	public:
		WindowAnchor();
		WindowAnchor(Anchor anchor, float offsetX = 0, float offsetY = 0);
		~WindowAnchor();

		void awake() override;
		void onWindowResize() override;

	public:
		PROPERTY()
		glm::vec2 anchor;
		PROPERTY()
		glm::vec2 offset;
	};
}
#endif