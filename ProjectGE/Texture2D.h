#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
namespace Core {
	class Texture2D
	{
	public:
		Texture2D();
		~Texture2D();

		void bind() const;
		void generate(GLuint width, GLuint height, GLuint internalFormat, GLuint imageFormat, GLuint wrapS, GLuint wrapT, GLuint filterMin, GLuint filterMax, unsigned char* data);
		float getAlphaAtPoint(int x, int y) const;

		GLuint		ID;
		GLuint		format;
		int			nrChannels;
		glm::ivec2	size; // Size of destination draw area
		glm::ivec2	sourceSize;
		glm::vec2	uvPos[4];
	};
}
#endif