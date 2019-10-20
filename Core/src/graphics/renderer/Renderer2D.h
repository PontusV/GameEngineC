#ifndef BATCH_MANAGER_H
#define BATCH_MANAGER_H

#include "BatchRenderer2D.h"
#include "Renderable2D.h"
#include "graphics/data/Color.h"
#include "graphics/data/Texture2D.h"
#include "graphics/font/Font.h"
#include "components/RectTransform.h"
#include "PostProcessor.h"
#include "maths/Vector2.h"

#include <vector>
#include <string>
#include <array>

namespace Core {

	#define MAX_RENDERABLES 100000

	class Camera;
	class Window;

	class Renderer2D {
	public:
		Renderer2D(Camera* camera, Window* window);
		~Renderer2D();

		/* Render sprite. Submits a Renderable2D to a batchRenderer2D. */
		void submit(const Texture2D& texture, const RectTransform& transform, const unsigned int& shaderID, const Color& color, const std::vector<std::array<Vector2, 4>>& clipMaskVertices, const unsigned char& sortingLayer);

		/* Render text. Submits text to a batchRenderer2D. */
		void submitText(const std::wstring& text, const RectTransform& transform, const Font& font, const Color& color, const std::vector<std::array<Vector2, 4>>& clipMaskVertices, const unsigned int& sortingLayer);

		/* Draws everything submitted to this renderer since the last render() call. */
		void render(float deltaTime);

		/* Updates the size of the area to be rendered. Resizes the Frame Buffer Object(FBO). */
		void updateSize(unsigned int width, unsigned int height);

	private:
		/* Draws and clears batch renderer. */
		void flush();

	private:
		Renderable2D renderableBuffer[MAX_RENDERABLES];
		std::size_t renderablesSize = 0;

		BatchRenderer2D	batch;
		PostProcessor postProcessor;
		Camera* camera;

		// Shader
		GLuint textShaderID;
	};
}
#endif