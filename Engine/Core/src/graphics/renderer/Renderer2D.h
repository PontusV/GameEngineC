#ifndef BATCH_MANAGER_H
#define BATCH_MANAGER_H

#include "BatchRenderer2D.h"
#include "Renderable2D.h"
#include "PostProcessor.h"
#include "graphics/data/Color.h"
#include "graphics/data/Texture2D.h"
#include "graphics/font/Font.h"
#include "components/RectTransform.h"
#include "maths/Vector2.h"

#include <vector>
#include <string>
#include <array>

namespace Core {

	#define MAX_RENDERABLES 100000

	class Window;

	/* 2D renderer for a specified window */
	class Renderer2D {
	public:
		Renderer2D(Window* window);
		~Renderer2D();

		/* Render sprite. Submits a Renderable2D to a batchRenderer2D. */
		void submit(const Texture2D& texture, const RectTransform& transform, const unsigned int& shaderID, const Color& color, const unsigned char& sortingOrder);

		/* Draws everything submitted to this renderer since the last render call. */
		void render(float deltaTime, Matrix4 viewMatrix = Matrix4());

		/* Updates the size of the area to be rendered. Resizes the Frame Buffer Object(FBO). */
		void updateSize(unsigned int width, unsigned int height);

	private:
		/* Draws and clears the whole batch renderer through the view of the given camera. */
		void flushAll();

		/* Draws and clears batch renderer. The specified index defines which renderables in the Renderable buffer to render. */
		void flush(std::size_t startIndex, std::size_t endIndex);

	private:
		std::array<Renderable2D, MAX_RENDERABLES> renderableBuffer;
		std::size_t renderablesSize = 0;

		BatchRenderer2D	batch;
		PostProcessor postProcessor;

		// Shader
		GLuint textShaderID;
	};
}
#endif