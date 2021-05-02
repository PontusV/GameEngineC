#include "Viewport.h"
#include <iostream>
#include <glad/glad.h>

using namespace Editor;


Viewport::Viewport() {
}

Viewport::~Viewport() {
	// Delete buffers
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &textureID);
}

void Viewport::initialize(unsigned int width, unsigned int height, bool background) {
	// Generate texture/renderbuffer/framebuffer object
	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &textureID);

	// Creates texture and updates FBO settings
	setSize(width, height, background);
}

void Viewport::setSize(unsigned int width, unsigned int height, bool background) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (background) {
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0); // Attach texture to framebuffer as its color attachment

	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::VIEWPORT: Failed to initialize FBO" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_UNDEFINED)
			std::cout << "ERROR::VIEWPORT: FBO Undefined" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
			std::cout << "ERROR::VIEWPORT: FBO incomplete attachment" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
			std::cout << "ERROR::VIEWPORT: FBO incomplete missing attachment" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
			std::cout << "ERROR::VIEWPORT: FBO incomplete draw buffer" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
			std::cout << "ERROR::VIEWPORT: FBO incomplete read buffer" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_UNSUPPORTED)
			std::cout << "ERROR::VIEWPORT: FBO unsupported" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
			std::cout << "ERROR::VIEWPORT: FBO incomplete multisample" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS)
			std::cout << "ERROR::VIEWPORT: FBO incomplete layer targets" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Viewport::begin() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Viewport::end() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Binds both READ and WRITE framebuffer to default framebuffer
}

unsigned int Viewport::getTextureID() {
	return textureID;
}