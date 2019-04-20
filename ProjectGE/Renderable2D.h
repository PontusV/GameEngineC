#ifndef SPRITE_H
#define SPRITE_H
#include <glm/glm.hpp>
#include <vector>

namespace Core {

	/* Holds color values between 0-255 */
	struct Color {
		Color() {}
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
		Color(float r, float g, float b, float a = 1.0f) : r((unsigned char)(r*255)), g((unsigned char)(g*255)), b((unsigned char)(b*255)), a((unsigned char)(a*255)) {}
		unsigned char	r,g,b,a;
	};

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