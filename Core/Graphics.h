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
#include <vector>

namespace Core {
	class Renderer2D;

	struct RenderableRects : public ComponentGroup<RectTransform, RectSprite> {
		ComponentArray<RectTransform>&	transforms	= getComponentArray<RectTransform>();
		ComponentArray<RectSprite>&		rects		= getComponentArray<RectSprite>();
	};

	struct RenderableImages : public ComponentGroup<RectTransform, Image> {
		ComponentArray<RectTransform>&	transforms	= getComponentArray<RectTransform>();
		ComponentArray<Image>&			images		= getComponentArray<Image>();
	};
	struct RenderableTexts : public ComponentGroup<RectTransform, Text> {
		ComponentArray<RectTransform>&	transforms	= getComponentArray<RectTransform>();
		ComponentArray<Text>&			texts		= getComponentArray<Text>();
	};

	struct RenderableTexturedSprites : public ComponentGroup<RectTransform, TexturedSprite> {
		ComponentArray<RectTransform>&	transforms	= getComponentArray<RectTransform>();
		ComponentArray<TexturedSprite>&	sprites		= getComponentArray<TexturedSprite>();
	};

	struct RenderableBorders : public ComponentGroup<RectTransform, Border> {
		ComponentArray<RectTransform>&	transforms	= getComponentArray<RectTransform>();
		ComponentArray<Border>&			borders		= getComponentArray<Border>();
	};

	/* Manages the window, renderer and UISystem */
	class Graphics {
	public:
		Graphics();
		~Graphics();

		bool createWindow(const char* windowTitle, int width, int height);
		bool initiate();
		void update(float dt);
		void render(float deltaTime);

		unsigned char createLayer();

		Window& getWindow();
		Renderer2D& getRenderer();
		UISystem& getUISystem();

	private:
		UISystem userInterfaceSystem;

		Window window;
		Renderer2D* renderer;

	private:
		RenderableRects renderableRects;
		RenderableImages renderableImages;
		RenderableTexts renderableTexts;
		RenderableTexturedSprites renderableTexturedSprites;
		RenderableBorders renderableBorders;
	};
}
#endif