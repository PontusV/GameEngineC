#ifndef BATCH_MANAGER_H
#define BATCH_MANAGER_H

#include "BatchRenderer2D.h"
#include "Renderable2D.h"
#include "Font.h"
#include "Transform.h"
#include "Layer.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace Core {

	static const std::size_t MAX_BATCH_AMOUNT = 100;

	class Window;

	class Renderer2D
	{
	public:
		Renderer2D(Window* window, std::size_t layerAmount);
		~Renderer2D();
		// Transformation Stack
		void push(glm::mat4 matrix, bool override = false);
		void pop();

		// Render sprite
		void submit(const Renderable2D& renderable, const unsigned short layerIndex);
		void flush();

		// Render text
		void renderText(std::string text, Transform transform, Font font, glm::vec4 color, bool clipEnabled, const glm::vec4 drawRect, const unsigned int layerIndex);
	private:
		std::vector<Layer> layers;

		std::vector<glm::mat4> transformationStack;
		glm::mat4* transformationStackBack;
	};
}
#endif