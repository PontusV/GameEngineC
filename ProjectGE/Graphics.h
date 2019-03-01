#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Entity.h"
#include "Image.h"
#include "Text.h"
#include "Transform.h"
#include "Texture2D.h"
#include "ComponentArrayManager.h"
#include "ComponentGroup.h"
#include "Window.h"
#include <vector>

namespace GameEngine {
	class Renderer2D;
	struct RenderableImages : public ComponentGroup<Transform, Image> {
		ComponentArray<Transform>& transforms = getComponentArray<Transform>();
		ComponentArray<Image>& images = getComponentArray<Image>();
	};

	struct RenderableTexts : public ComponentGroup<Transform, Text> {
		ComponentArray<Transform>& transforms = getComponentArray<Transform>();
		ComponentArray<Text>& texts = getComponentArray<Text>();
	};

	class Graphics {
	public:
		Graphics();
		~Graphics();

		void initiate(int screenWidth, int screenHeight);
		void update(float dt);
		void render();
		void createWindow();
		void createWindow(const char *title, int x, int y, int width, int height);
	private:

		Window window;
		Renderer2D* renderer;
		RenderableImages renderableImages;
		RenderableTexts renderableTexts;
	};
}
#endif