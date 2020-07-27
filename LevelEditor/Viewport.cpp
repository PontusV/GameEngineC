#include "Viewport.h"
#include <iostream>
#include <glad/glad.h>

using namespace Editor;


Viewport::Viewport() {
}

Viewport::~Viewport() {
	// Delete buffers
	if (VAO != NULL) {
		glDeleteVertexArrays(1, &VAO);
	}
	if (VBO != NULL) {
		glDeleteBuffers(1, &VBO);
	}
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &textureID);
}

void Viewport::initialize(unsigned int width, unsigned int height) {
	// Generate texture/renderbuffer/framebuffer object
	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &RBO);
	glGenTextures(1, &textureID);

	// Creates texture and updates RBO settings
	setSize(width, height);

	// Initialize render data and uniforms
	init();
}

void Viewport::setSize(unsigned int width, unsigned int height) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0); // Attach texture to framebuffer as its color attachment

	// RBO
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

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

void Viewport::init() {
	// Configure VAO/VBO
	GLfloat vertices[] = {
		// Pos        // Tex
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)(2 * sizeof(GL_FLOAT)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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