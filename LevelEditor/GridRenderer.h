#ifndef GRID_H
#define GRID_H

#include "graphics/data/Shader.h"

namespace Editor {

	struct GridVertexData {
		float x;
		float y;
	};

	class GridRenderer {
	public:
		GridRenderer();
		~GridRenderer();

		/* Used for initialization and resizing. Calls cleanup before initializing to prevent memory leak */
		void initialize(unsigned int width, unsigned int height, float spacing);

		/* Resizes the area drawn on the screen. The size should be the same as the window size. */
		void setSize(unsigned int width, unsigned int height);

		void render(float x, float y);

	private:
		/* Binds the FBO and clears the texture */
		void begin();
		/* Unbinds the FBO */
		void end();

		void cleanup();

	private:
		Core::Shader shader;
		unsigned int width, height; // Viewport size
		unsigned int wCount, hCount; // Number of lines vertically and horizontally
		float spacing; // Spacing between grid lines

		unsigned int VAO, VBO;

		GridVertexData* buffer;
	};
}
#endif