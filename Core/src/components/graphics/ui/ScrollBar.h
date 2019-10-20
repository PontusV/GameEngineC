#ifndef SCROLL_BAR_H
#define SCROLL_BAR_H
#include "UIBehaviour.h"
#include "graphics/data/Color.h"
#include "maths/Vector2.h"
#include "ScrollBar.generated.h"
namespace Core {
	CLASS() ScrollBar : public UIBehaviour {
		GENERATED_BODY()
	public:
		ScrollBar(EntityHandle target);
		ScrollBar();
		~ScrollBar();

		void start() override;
		void onDestroy() override;
		void onPreRender() override;
		void onMouseDrag(float mouseX, float mouseY);
		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonReleased(int buttoncode, int mods) override;
		void onScroll(float xOffset, float yOffset) override;

	public:
		PROPERTY()
		EntityHandle target;
		PROPERTY()
		Color backgroundColor = Color(0, 0, 0, 255);
		PROPERTY()
		Color handleColorDefault = Color(200, 200, 200, 255);
		PROPERTY()
		Color handleColorHighlight = Color(225, 225, 225, 255);
		PROPERTY()
		Color handleColorDragging = Color(255, 255, 255, 255);
		PROPERTY()
		float handlePadding = 2;
		PROPERTY()
		bool scrollY = true;

	private:
		EntityHandle handle;
		Vector2 handlePrevPos;
		bool handleDrag = false;
		bool hovering = false;
	};
}
#endif