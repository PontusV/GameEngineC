#ifndef SPRITE_H
#define SPRITE_H
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "Transform.h"
#include "Shader.h"
namespace Core {
	/* Holds references to data required for rendering. */
	struct Renderable2D
	{
		const Texture2D		&texture;
		const Transform		&transform;
		const glm::ivec2	&size;
		const GLuint		&shaderID;
		const glm::vec4		&color;

		bool				clipEnabled = false;
		const std::vector<glm::vec2>	&clipMaskVertices;
	};

	/* Holds copied data required for rendering. */
	struct RenderCopy2D {
		RenderCopy2D() {}
		RenderCopy2D(const Renderable2D& renderable) {
			texture			= renderable.texture;
			transform		= renderable.transform;
			size			= renderable.size;
			shaderID		= renderable.shaderID;
			color			= renderable.color;
			clipEnabled		= renderable.clipEnabled;
			clipMaskVertices = renderable.clipMaskVertices;
		}

		Texture2D	texture;
		Transform	transform;
		glm::ivec2	size;
		GLuint		shaderID;
		glm::vec4	color;

		bool		clipEnabled;
		std::vector<glm::vec2>	clipMaskVertices;

		RenderCopy2D& operator=(const Renderable2D& other) {
			texture			= other.texture;
			transform		= other.transform;
			size			= other.size;
			shaderID		= other.shaderID;
			color			= other.color;
			clipEnabled		= other.clipEnabled;
			clipMaskVertices = other.clipMaskVertices;

			return *this;
		}
	};
}
#endif