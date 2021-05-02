#ifndef GRID_H
#define GRID_H

#include "Shader.h"

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
		void initialize(unsigned int width, unsigned int height, float spacing, float zoom = 1.0f);
		/* Renders the grid */
		void render(float x, float y);

	private:
		void begin();
		void end();

		void cleanup();

	private:
		Shader shader;
		unsigned int width, height; // Viewport size
		unsigned int wCount, hCount; // Number of lines vertically and horizontally
		float zoom; // Amount of zoom
		float spacing; // Spacing between grid lines

		unsigned int VAO, VBO;

		GridVertexData* buffer;
	};
}
#endif