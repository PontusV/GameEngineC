#include "GridRenderer.h"
#include "ResourceManager.h"
#include <glad/glad.h>
#include <iostream>

using namespace Editor;


GridRenderer::GridRenderer() {
}

GridRenderer::~GridRenderer() {
	cleanup();
}

void GridRenderer::cleanup() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	if (VBO != NULL) {
		glDeleteBuffers(1, &VBO);
		VBO = NULL;
	}
	if (VAO != NULL) {
		glDeleteVertexArrays(1, &VAO);
		VAO = NULL;
	}
}

void GridRenderer::initialize(unsigned int width, unsigned int height, float spacing) {
	cleanup(); // Cleans up old OpenGL resources
	shader = ResourceManager::getInstance().loadShader("resources/shaders/grid");
	this->width = width;
	this->height = height;
	this->spacing = spacing;
	this->wCount = 0;
	this->hCount = 0;
	if (width == 0 || height == 0 || spacing == 0 || width * zoom > 10000 || height * zoom > 10000 || spacing > 10000) return;

	this->hCount = (height * zoom / spacing) + 2;
	this->wCount = (width * zoom / spacing) + 2;
	unsigned int vertexCount = (hCount * 2) + (wCount * 2);

	glGetError(); //Clear errors

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GridVertexData), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GridVertexData), (const GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	if (GL_NO_ERROR != glGetError()) {
		std::cout << "Failed to initalize grid renderer\n";
	}
}

void GridRenderer::begin() {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	buffer = (GridVertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void GridRenderer::end() {
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GridRenderer::render(float x, float y) {
	if (width == 0 || height == 0 || spacing == 0 || width > 10000 || height > 10000 || spacing > 10000) return;
	float width = this->width * zoom;
	float height = this->height * zoom;

	float offsetX = (std::fmodf(-x + width/2, spacing) * 2) / width;
	float offsetY = (std::fmodf(y - height/2, spacing) * 2) / height;

	begin();
	for (std::size_t i = 0; i < hCount * 2; i += 2) {
		// Add horizontal line
		float y = (spacing * i) / height;
		float posY = y - offsetY;
		buffer[i] = { -1.0f, 1 - posY };
		buffer[i+1] = { 1.0f, 1 - posY };
	}
	for (std::size_t i = 0; i < wCount * 2; i += 2) {
		std::size_t index = hCount * 2 + i;
		// Add vertical line
		float x = (spacing * i) / width;
		float posX = x + offsetX;
		buffer[index] = { posX - 1, -1.0f };
		buffer[index + 1] = { posX - 1, 1.0f };
	}
	end();
	unsigned int vertexCount = (hCount * 2) + (wCount * 2);

	shader.use();

	glBindVertexArray(VAO);

	// Draw
	glLineWidth(1.0f);
	glDrawArrays(GL_LINES, 0, vertexCount);

	glBindVertexArray(0);
}

void GridRenderer::setZoom(float value) {
	zoom = value;
	initialize(width, height, spacing);
}