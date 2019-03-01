#ifndef BATCH_MANAGER_H
#define BATCH_MANAGER_H

#include "BatchRenderer2D.h"
#include "Renderable2D.h"
#include "Font.h"
#include "Transform.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace GameEngine {

	static const std::size_t MAX_BATCH_AMOUNT = 100;

	class Renderer2D
	{
	public:
		Renderer2D();
		~Renderer2D();
		// Transformation Stack
		void push(glm::mat4 matrix, bool override = false);
		void pop();

		// Render sprite
		void submit(const Renderable2D& renderable, const unsigned short layer);
		void flush();

		// Render text
		void renderText(std::string text, Transform transform, Font font, glm::vec4 color, const unsigned int layer);
	private:
		BatchRenderer2D batchStorage[MAX_BATCH_AMOUNT];	// Ensures cache hits
		std::vector<BatchRenderer2D*> batches;			// Points to batches in batchStorage

		std::vector<glm::mat4> transformationStack;
		glm::mat4* transformationStackBack;
		//Shader
		GLuint textShaderID;
	};
}
#endif