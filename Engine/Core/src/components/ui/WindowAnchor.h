#ifndef WINDOW_ANCHOR_H
#define WINDOW_ANCHOR_H
#include "UIBehaviour.h"
#include "utils/Anchor.h"
#include "maths/Vector2.h"
#include "WindowAnchor.generated.h"
namespace Core {

	/* Manages the position of a Transform component. The position depends on the anchor value, the size of the window and the offset. */
	CLASS() WindowAnchor : public UIBehaviour {
		GENERATED_BODY()
	public:
		WindowAnchor();
		WindowAnchor(Anchor anchor, float offsetX = 0, float offsetY = 0);
		~WindowAnchor();

		void initialize() override;
		void onWindowResize() override;

	public:
		PROPERTY()
		Vector2 anchor;
		PROPERTY()
		Vector2 offset;
	};
}
#endif