#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "Maths/Vector2.h"

namespace Core {
	typedef unsigned int GLuint;
	class Texture2D
	{
	public:
		Texture2D();
		~Texture2D();

		void bind() const;
		void generate(GLuint width, GLuint height, GLuint internalFormat, GLuint imageFormat, GLuint wrapS, GLuint wrapT, GLuint filterMin, GLuint filterMax, unsigned char* data);
		float getAlphaAtPoint(int x, int y) const;

		GLuint ID;
		GLuint format;
		int nrChannels;
		Vector2 size; // Size of destination draw area
		Vector2 sourceSize;
		Vector2 uvCoords[4];
	};
}
#endif