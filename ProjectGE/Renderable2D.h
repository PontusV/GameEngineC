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
		const glm::vec2		&imageOffset;

		bool				clipEnabled = false;
		const glm::vec4		&drawRect;
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
			imageOffset		= renderable.imageOffset;
			clipEnabled		= renderable.clipEnabled;
			drawRect		= renderable.drawRect;
		}

		Texture2D	texture;
		Transform	transform;
		glm::ivec2	size;
		GLuint		shaderID;
		glm::vec4	color;
		glm::vec2	imageOffset;

		bool		clipEnabled;
		glm::vec4	drawRect;

		RenderCopy2D& operator=(const Renderable2D& other) {
			texture			= other.texture;
			transform		= other.transform;
			size			= other.size;
			shaderID		= other.shaderID;
			color			= other.color;
			imageOffset		= other.imageOffset;
			clipEnabled		= other.clipEnabled;
			drawRect		= other.drawRect;

			return *this;
		}
	};
}
#endif