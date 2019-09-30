#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Entity.h"
#include "Image.h"
#include "Text.h"
#include "TexturedSprite.h"
#include "RectTransform.h"
#include "Texture2D.h"
#include "RectSprite.h"
#include "Border.h"

#include "ComponentArrayManager.h"
#include "ComponentGroup.h"
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
		void update(float dt);
		void render(float deltaTime);

		Window& getWindow();
		Camera& getCamera();
		Renderer2D& getRenderer();
		UISystem& getUISystem();

	private:
		UISystem userInterfaceSystem;

		Window window;
		Camera camera;
		Renderer2D* renderer;

	private:
		ComponentGroup<RectTransform, RectSprite> renderableRects;
		ComponentGroup<RectTransform, Image> renderableImages;
		ComponentGroup<RectTransform, Text> renderableTexts;
		ComponentGroup<RectTransform, TexturedSprite> renderableTexturedSprites;
		ComponentGroup<RectTransform, Border> renderableBorders;
	};
}
#endif