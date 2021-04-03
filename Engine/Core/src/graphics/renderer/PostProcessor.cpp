#include "PostProcessor.h"
#include "graphics/Window.h"
#include "engine/ResourceManager.h"
#include <glad/glad.h>

using namespace Core;


PostProcessor::PostProcessor(Window* window) : window(window) {
	shader = ResourceManager::getInstance().loadShader("resources/shaders/postprocess");
	// Generate texture/renderbuffer/framebuffer object
	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &RBO);
	glGenTextures(1, &textureID);

	// Width & Height should be same as the window
	setSize(window->getWidth(), window->getHeight());

	// Initialize render data and uniforms
	init();
}

PostProcessor::~PostProcessor() {
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

void PostProcessor::setSize(unsigned int width, unsigned int height) {
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
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_UNDEFINED)
			std::cout << "ERROR::POSTPROCESSOR: FBO Undefined" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
			std::cout << "ERROR::POSTPROCESSOR: FBO incomplete attachment" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
			std::cout << "ERROR::POSTPROCESSOR: FBO incomplete missing attachment" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
			std::cout << "ERROR::POSTPROCESSOR: FBO incomplete draw buffer" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
			std::cout << "ERROR::POSTPROCESSOR: FBO incomplete read buffer" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_UNSUPPORTED)
			std::cout << "ERROR::POSTPROCESSOR: FBO unsupported" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
			std::cout << "ERROR::POSTPROCESSOR: FBO incomplete multisample" << std::endl;
		if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS)
			std::cout << "ERROR::POSTPROCESSOR: FBO incomplete layer targets" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::init() {
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

void PostProcessor::begin() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::end() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Binds both READ and WRITE framebuffer to default framebuffer
}

void PostProcessor::render(float time) {
	// Set uniforms/options
	shader.use();

	// Render textured quad
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// ---------------------------- EFFECTS --------------------------------

void PostProcessor::setGrayscale(bool value) {
	shader.setInteger("grayscale", value, true);
}
void PostProcessor::setInvert(bool value) {
	shader.setInteger("invert", value, true);
}