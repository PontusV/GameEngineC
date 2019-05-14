#ifndef BATCH_MANAGER_H
#define BATCH_MANAGER_H

#include "BatchRenderer2D.h"
#include "Renderable2D.h"
#include "Color.h"
#include "Font.h"
#include "RectTransform.h"
#include "Texture2D.h"
#include "PostProcessor.h"

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace Core {

	#define MAX_RENDERABLES 100000

	class Window;

	class Renderer2D {
	public:
		Renderer2D(Window* window);
		~Renderer2D();

		/* Returns unique ID for a layer */
		unsigned char createLayer();

		/* Render sprite. Submits a Renderable2D to a batchRenderer2D. */
		void submit(const Texture2D& texture, const RectTransform& transform, const unsigned int& shaderID, const Color& color, const bool& clipEnabled, const std::vector<glm::vec2>& clipMaskVertices, const unsigned char& layerIndex);

		/* Render text. Submits text to a batchRenderer2D. */
		void submitText(const std::string& text, const RectTransform& transform, const Font& font, const Color& color, const bool& clipEnabled, const std::vector<glm::vec2>& clipMaskVertices, const unsigned int& layerIndex);

		/* Draws everything submitted to this renderer since the last render() call. */
		void render(float deltaTime);

		/* Updates the size of the area to be rendered. Resizes the Frame Buffer Object(FBO). */
		void updateSize(unsigned int width, unsigned int height);

	private:
		/* Draws and clears batch renderer. */
		void flush();

	private:
		Renderable2D renderableBuffer[MAX_RENDERABLES];
		std::size_t renderablesSize;
		BatchRenderer2D	batch;

		unsigned char layerAmount; // To keep track of next LayerID
		PostProcessor postProcessor;

		// Shader
		GLuint textShaderID;
	};
}
#endif