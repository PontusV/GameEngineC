#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Entity.h"
#include "Image.h"
#include "Text.h"
#include "Transform.h"
#include "Texture2D.h"
#include "Rect.h"
#include "Panel.h"

#include "ComponentArrayManager.h"
#include "ComponentGroup.h"

#include "Window.h"
#include <vector>

namespace Core {
	class Renderer2D;

	struct RenderableRects : public ComponentGroup<Transform, Rect> {
		ComponentArray<Transform>&	transforms	= getComponentArray<Transform>();
		ComponentArray<Rect>&		rects		= getComponentArray<Rect>();
	};

	struct RenderableImages : public ComponentGroup<Transform, Image> {
		ComponentArray<Transform>&	transforms	= getComponentArray<Transform>();
		ComponentArray<Image>&		images		= getComponentArray<Image>();
	};

	struct RenderableTexts : public ComponentGroup<Transform, Text> {
		ComponentArray<Transform>&	transforms	= getComponentArray<Transform>();
		ComponentArray<Text>&		texts		= getComponentArray<Text>();
	};

	struct PanelGroup : public ComponentGroup<Transform, Panel> {
		ComponentArray<Transform>&	transforms	= getComponentArray<Transform>();
		ComponentArray<Panel>&		panels		= getComponentArray<Panel>();
	};

	class Graphics {
	public:
		Graphics();
		~Graphics();

		bool createWindow(const char* windowTitle, int width, int height);
		bool initiate();
		void update(float dt);
		void render();


		Window& getWindow();
	private:

		Window window;
		Renderer2D* renderer;

		RenderableRects renderableRects;
		RenderableImages renderableImages;
		RenderableTexts renderableTexts;

		PanelGroup panelGroup;
	};
}
#endif