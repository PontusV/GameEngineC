#ifndef LAYER_H
#define LAYER_H

#include "BatchRenderer2D.h"
#include "Renderable2D.h"
#include "Font.h"
#include <cstddef>
//#include <vector>

#define MAX_RENDERABLES_PER_LAYER 10000

namespace Core {
	class Window;

	class Layer {
	public:
		Layer(int layerIndex, Window* window);
		~Layer();

		void renderText(std::string text, Transform transform, Font font, glm::vec4 color, bool clipEnabled, const glm::vec4 drawRect);
		void submit(const Renderable2D& renderable);
		void flush();
	private:
		BatchRenderer2D				batch;
		std::size_t					index;
		RenderCopy2D				renderables[MAX_RENDERABLES_PER_LAYER];
		std::size_t					renderablesSize;

		//Shader
		GLuint textShaderID;
	};
}
#endif