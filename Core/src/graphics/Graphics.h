#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "entity/Entity.h"
#include "components/graphics/Image.h"
#include "components/graphics/Text.h"
#include "components/graphics/TexturedSprite.h"
#include "components/RectTransform.h"
#include "data/Texture2D.h"
#include "components/graphics/RectSprite.h"
#include "components/graphics/ui/Border.h"
#include "components/engine/UIObjectData.h"
#include "components/engine/GameObjectData.h"

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
		void update(float dt);
		void render(float deltaTime);

		Window& getWindow();
		Camera& getCamera();
		Renderer2D& getRenderer();
		Renderer2D& getRendererUI();
		UISystem& getUISystem();

	private:
		UISystem userInterfaceSystem;

		Window window;
		Camera camera;
		Renderer2D* renderer;
		Renderer2D* rendererUI;

	private:
		ComponentGroup<RectTransform, Image> renderableImages;
		ComponentGroup<RectTransform, Text, GameObjectData> renderableTexts;
		ComponentGroup<RectTransform, RectSprite, GameObjectData> renderableRects;
		ComponentGroup<RectTransform, TexturedSprite, GameObjectData> renderableTexturedSprites;
		ComponentGroup<RectTransform, Border, GameObjectData> renderableBorders;

		ComponentGroup<RectTransform, Text, UIObjectData> renderableTextsUI;
		ComponentGroup<RectTransform, RectSprite, UIObjectData> renderableRectsUI;
		ComponentGroup<RectTransform, TexturedSprite, UIObjectData> renderableTexturedSpritesUI;
		ComponentGroup<RectTransform, Border, UIObjectData> renderableBordersUI;
	};
}
#endif