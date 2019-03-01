#ifndef SPRITE_H
#define SPRITE_H
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "Transform.h"
#include "Shader.h"
namespace GameEngine {
	/* Holds references to data required for rendering. */
	struct Renderable2D
	{
		Texture2D&	texture;
		Transform&	transform;
		glm::ivec2&	size;
		GLuint&		shaderID;
		glm::vec4&	color;
	};

	/*class Sprite
	{
	public:
		Sprite(Transform transform, Texture2D texture, const GLuint shaderID, const glm::vec4 color = glm::vec4(1, 1, 1, 1));
		Sprite(Transform transform, float width, float height, Texture2D texture, const GLuint shaderID, const glm::vec4 color = glm::vec4(1, 1, 1, 1));
		~Sprite();

		const GLuint& getShaderID() const;
		const GLuint& getTextureID() const;
		const glm::vec2& getPosition() const;
		const glm::vec4& getColor() const;
		const glm::vec2& getSize() const;
		const Transform& getTransform() const;
	private:
		glm::vec4 color;
		glm::vec2 size;

		Texture2D texture;
		Transform transform;
		GLuint shaderID;
	};*/
}
#endif