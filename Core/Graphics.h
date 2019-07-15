#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Entity.h"
#include "Image.h"
#include "Text.h"
#include "RectTransform.h"
#include "Texture2D.h"
#include "RectSprite.h"
#include "Panel.h"
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

	struct RenderableBorders : public ComponentGroup<RectTransform, Border> {
		ComponentArray<RectTransform>&	transforms	= getComponentArray<RectTransform>();
		ComponentArray<Border>&			borders		= getComponentArray<Border>();
	};

	struct PanelGroup : public ComponentGroup<RectTransform, Panel> {
		ComponentArray<RectTransform>&	transforms	= getComponentArray<RectTransform>();
		ComponentArray<Panel>&			panels		= getComponentArray<Panel>();
	};
	struct SpriteCollection : public ComponentGroup<Sprite> {
		ComponentArray<Sprite>& sprites = getComponentArray<Sprite>();
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
		RenderableBorders renderableBorders;
		PanelGroup panelGroup;
		SpriteCollection spriteGroup;
	};
}
#endif