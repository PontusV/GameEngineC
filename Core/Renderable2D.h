#ifndef RENDERABLE_2D_H
#define RENDERABLE_2D_H
#include "Color.h"
#include <glm/glm.hpp>
#include <vector>

namespace Core {

	struct Renderable2D {
		Renderable2D() {}
		unsigned int	textureID;
		glm::vec2		vertices[4];
		glm::vec2		uvCoords[4];
		float			z;
		unsigned char	shaderID;
		Color			color;
		unsigned char	layerIndex;

		// Clipping with Stencil Buffer
		bool			clipEnabled;
		std::vector<glm::vec2> clipMaskVertices;
	};
}
#endif