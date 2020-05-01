#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "entity/Entity.h"
#include "components/graphics/Image.h"
#include "components/graphics/Text.h"
#include "components/graphics/TexturedSprite.h"
#include "components/RectTransform.h"
#include "data/Texture2D.h"
#include "components/graphics/RectSprite.h"
#include "components/ui/graphics/Border.h"

#include "entity/component/ComponentArrayManager.h"
#include "entity/component/ComponentGroup.h"
#include "UISystem.h"

#include "Window.h"
#include "Camera.h"
#include <vector>

namespace Core {
	class Renderer2D;

	/* Manages the window, renderer and UISystem */
	class Graphics {
	public:
		Graphics();
		~Graphics();

		bool createWindow(const char* windowTitle, int width, int height);
		bool initiate();
		void update(float deltaTime);
		void render(float deltaTime);

		Window& getWindow();
		Camera& getCamera();
		Renderer2D& getRenderer();
		UISystem& getUISystem();

	private:
		UISystem userInterfaceSystem; // TODO: Move to Engine as its own System

		Window window;
		Camera camera;
		Renderer2D* renderer;

	private:
		ComponentGroup<RectTransform, Image> renderableImages;
		ComponentGroup<RectTransform, Text> renderableTexts;
		ComponentGroup<RectTransform, RectSprite> renderableRects;
		ComponentGroup<RectTransform, TexturedSprite> renderableTexturedSprites;
		ComponentGroup<RectTransform, Border> renderableBorders;
	};
}
#endif