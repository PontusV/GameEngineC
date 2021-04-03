#ifndef RENDERABLE_2D_H
#define RENDERABLE_2D_H
#include "graphics/data/Color.h"
#include "maths/Vector2.h"

namespace Core {

	struct Renderable2D {
		Renderable2D() {}
		unsigned int	textureID;
		Vector2			vertices[4];
		Vector2			uvCoords[4];
		float			z;
		unsigned char	shaderID;
		Color			color;
		unsigned char	sortingOrder;
	};
}
#endif