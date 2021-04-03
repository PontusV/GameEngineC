#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Core/Graphics.h>
#include "entity/Entity.h"
#include "components/graphics/SpriteRenderer.h"
#include "components/graphics/CanvasRenderer.h"
#include "components/graphics/Text.h"
#include "components/graphics/Image.h"
#include "components/graphics/RectSprite.h"
#include "components/graphics/Border.h"
#include "components/RectTransform.h"

#include "entity/component/ComponentGroup.h"

#include "Window.h"
#include "Camera.h"
#include <vector>

namespace Core {
	class Renderer2D;

	/* Manages the window, renderer and UISystem */
	class Graphics: public IGraphics {
	public:
		Graphics();
		~Graphics();

		/* Creates a new Window and initializes it. Stores the window in this instance. */
		bool createWindow(const char* windowTitle, int width, int height);
		bool initiate();
		void update(float deltaTime);
		/* Renders the world and the GUI */
		void render(float deltaTime);

		Window& getWindow();
		Camera& getCamera();
		Renderer2D& getRenderer();

		IWindow* getWindowInterface() override;
		ICamera* getCameraInterface() override;

	private:
		void renderWorld(float deltaTime);
		void renderGUI(float deltaTime);

		std::vector<TextSprite> createTextSprites(const Text& text, const RectTransform& transform) const;
		std::vector<RectTransform> createBorder(const Border& border, const RectTransform& transform) const;

	private:
		Window window;
		Camera camera;
		Renderer2D* renderer;

	private:
		ComponentGroup<RectTransform, Image, SpriteRenderer> renderableImages;
		ComponentGroup<RectTransform, RectSprite, SpriteRenderer> renderableRects;
		ComponentGroup<RectTransform, Text, SpriteRenderer> renderableTexts;
		ComponentGroup<RectTransform, Border, SpriteRenderer> renderableBorders;
		ComponentGroup<RectTransform, Image, CanvasRenderer> canvasImages;
		ComponentGroup<RectTransform, RectSprite, CanvasRenderer> canvasRects;
		ComponentGroup<RectTransform, Text, CanvasRenderer> canvasTexts;
		ComponentGroup<RectTransform, Border, CanvasRenderer> canvasBorders;
	};
}
#endif