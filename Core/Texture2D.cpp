#include "Texture2D.h"
#include <glad/glad.h>
#include <iostream>

using namespace Core;

Texture2D::Texture2D() : size(0, 0), uvCoords{ Vector2(0,0), Vector2(0,1), Vector2(1,1), Vector2(1,0) }, ID(0) {
}


Texture2D::~Texture2D()
{
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

float Texture2D::getAlphaAtPoint(int x, int y) const {
	if (format != GL_RGBA) return 1.0f;
	
	int xPos = (x * sourceSize.x) / size.x;
	int yPos = (y * sourceSize.y) / size.y;

	int index = (yPos * sourceSize.x + xPos) * 4;

	GLubyte* data = new GLubyte[sourceSize.x * sourceSize.y * 4];

	glBindTexture(GL_TEXTURE_2D, ID);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	float alpha = data[index];
	delete[] data;
	return alpha;
}

void Texture2D::generate(GLuint width, GLuint height, GLuint internalFormat, GLuint imageFormat, GLuint wrapS, GLuint wrapT, GLuint filterMin, GLuint filterMax, unsigned char* data) {
	size = Vector2(width, height);
	sourceSize = Vector2(width, height);
	format = imageFormat;
	// Create Texture
	glBindTexture(GL_TEXTURE_2D, ID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Makes sure all images get loaded properly
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, imageFormat, GL_UNSIGNED_BYTE, data);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}