#include "Texture2D.h"
#include <iostream>

using namespace GameEngine;

Texture2D::Texture2D() : size(0, 0), uvPos{ glm::vec2(0,0), glm::vec2(0,1), glm::vec2(1,1), glm::vec2(1,0) }, ID(0) {
}


Texture2D::~Texture2D()
{
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2D::generate(GLuint width, GLuint height, GLuint internalFormat, GLuint imageFormat, GLuint wrapS, GLuint wrapT, GLuint filterMin, GLuint filterMax, unsigned char* data) {
	size = glm::vec2(width, height);
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