#include "BatchRenderer2D.h"
#include "Texture2D.h"
#include "TransformMaths.h"
#include "Window.h"

#include "ResourceManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cstddef>

using namespace Core;


static GLushort indices[RENDERER_INDICES_SIZE];

void initIndices() {
	GLushort offset = 0;
	for (std::size_t i = 0; i < RENDERER_INDICES_SIZE; i += 6) {
		indices[i] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}
}

BatchRenderer2D::BatchRenderer2D(Window* window) : window(window) {
	maskShaderID = ResourceManager::getInstance().loadShader("resources/shaders/sprite.vert", "resources/shaders/sprite.frag").ID;
}


BatchRenderer2D::~BatchRenderer2D() {
	cleanUp();
}

void BatchRenderer2D::cleanUp() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	if (EBO != NULL) {
		glDeleteBuffers(1, &EBO);
		EBO = NULL;
	}
	if (VBO != NULL) {
		glDeleteBuffers(1, &VBO);
		VBO = NULL;
	}
	if (VAO != NULL) {
		glDeleteVertexArrays(1, &VAO);
		VAO = NULL;
	}
	indexCount = 0;
	verticiesCount = 0;
}

BatchConfig& BatchRenderer2D::getConfig() {
	return *config;
}

inline bool BatchRenderer2D::hasRoom() {
	static int vertices = 4;
	return (verticiesCount + vertices) <= RENDERER_MAX_VERTICES;
}

void BatchRenderer2D::init() {
	indexCount = 0;
	verticiesCount = 0;
	glGetError(); //Clear errors

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
	glEnableVertexAttribArray(SHADER_COLOR_INDEX);
	glEnableVertexAttribArray(SHADER_TEXTURE_INDEX);
	glEnableVertexAttribArray(SHADER_TEXTURE_ID_INDEX);
	glVertexAttribPointer(SHADER_VERTEX_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)0);
	glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::color)));
	glVertexAttribPointer(SHADER_TEXTURE_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::texture)));
	glVertexAttribPointer(SHADER_TEXTURE_ID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::textureID)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	static bool indicesInitialized;
	if (!indicesInitialized) {
		initIndices();
		indicesInitialized = true;
	}

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_INDICES_SIZE * sizeof(GLushort), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	if (GL_NO_ERROR != glGetError()) {
		std::cout << "Failed to create batch\n";
	}
}

void BatchRenderer2D::begin() {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void BatchRenderer2D::submit(const Renderable2D& renderable) {
	if (!hasRoom())
		throw std::length_error("Can not add anymore sprites to this batch!");


	const glm::vec2*	vertices	= renderable.vertices;
	const glm::vec2*	uvCoords	= renderable.uvCoords;
	const Color&		color		 = renderable.color;

	const float& textureSlot = renderable.textureID == 0 ? 0 : (float)config->getTextureSlot(renderable.textureID);

	int r = (int)(color.r);
	int g = (int)(color.g);
	int b = (int)(color.b);
	int a = (int)(color.a);

	unsigned int c = a << 24 | b << 16 | g << 8 | r;

	std::size_t& i = verticiesCount;

	buffer[i].vertex = vertices[0];
	buffer[i].color = c;
	buffer[i].texture = uvCoords[0];
	buffer[i].textureID = textureSlot;
	i++;

	buffer[i].vertex = vertices[1];
	buffer[i].color = c;
	buffer[i].texture = uvCoords[1];
	buffer[i].textureID = textureSlot;
	i++;

	buffer[i].vertex = vertices[2];
	buffer[i].color = c;
	buffer[i].texture = uvCoords[2];
	buffer[i].textureID = textureSlot;
	i++;

	buffer[i].vertex = vertices[3];
	buffer[i].color = c;
	buffer[i].texture = uvCoords[3];
	buffer[i].textureID = textureSlot;
	i++;

	segmentBack->size += 6;
	indexCount += 6;
}

void BatchRenderer2D::submitMask(glm::vec2 vertex1, glm::vec2 vertex2, glm::vec2 vertex3, glm::vec2 vertex4) {
	if (!hasRoom())
		throw std::length_error("Can not add anymore sprites to this batch!");

	int r = 255;
	int g = 255;
	int b = 255;
	int a = 255;
	unsigned int c = a << 24 | b << 16 | g << 8 | r;

	std::size_t& i = verticiesCount;

	buffer[i].vertex = vertex1;
	buffer[i].color = c;
	buffer[i].textureID = 0;
	i++;

	buffer[i].vertex = vertex2;
	buffer[i].color = c;
	buffer[i].textureID = 0;
	i++;

	buffer[i].vertex = vertex3;
	buffer[i].color = c;
	buffer[i].textureID = 0;
	i++;

	buffer[i].vertex = vertex4;
	buffer[i].color = c;
	buffer[i].textureID = 0;
	i++;

	segmentBack->size += 6;
	indexCount += 6;
}

void BatchRenderer2D::end() {
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BatchRenderer2D::flush() {
	// Is there anything to draw?
	if (indexCount == 0) return;

	for (BatchSegment& segment : segments) {
		// Is there anything to draw in this segment?
		if (segment.size == 0) return;
		BatchConfig& config = segment.config;
		const GLuint& shaderID = config.shaderID;

		for (std::size_t i = 0; i < config.textureIDs.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, config.textureIDs[i]);
		}

		//-------------
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		if (config.clipEnabled) {
			glStencilMask(0xFF);				// Enable stencil writes
			glClear(GL_STENCIL_BUFFER_BIT);		// Clear old masks
			glStencilFunc(GL_ALWAYS, 1, 0xFF);	// Always draw

			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Disables drawing to color buffer
			// Draw mask
			glUseProgram(maskShaderID);
			for (std::size_t i = 1; i <= segment.clipMaskCount; i++) {
				std::size_t maskIndex = segment.startIndex - (6 * i);
				if (maskIndex > 0)
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(maskIndex * sizeof(GLushort)));
			}
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);	// Enable drawing to color buffer

			glStencilMask(0x00);				// Disable stencil writes
			glStencilFunc(GL_LEQUAL, 1, 0xFF);	// Only draw on mask
		}
		glUseProgram(shaderID);

		//Draw
		glDrawElements(GL_TRIANGLES, segment.size, GL_UNSIGNED_SHORT, (void*)(segment.startIndex * sizeof(GLushort)));
		if (config.clipEnabled) {
			glStencilFunc(GL_ALWAYS, 1, 0xFF);	// Always draw
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	indexCount = 0;
	verticiesCount = 0;
	segments.clear();
}


void BatchRenderer2D::startNewSegment(BatchConfig config) {
	// Start segment
	BatchSegment segment{ indexCount, 0, 0, config };
	segments.push_back(segment);
	segmentBack = &segments.back();
	this->config = &segmentBack->config;

	// Add clip mask for segments with clipping enabled
	if (config.clipEnabled) {

		for (std::size_t i = 0; i < config.clipMaskVertices.size(); i += 4) {
			submitMask(config.clipMaskVertices[i], config.clipMaskVertices[i+1], config.clipMaskVertices[i+2], config.clipMaskVertices[i+3]);

			// Shift segment start (Puts this mask right before segment start)
			segmentBack->startIndex += 6;
			segmentBack->size -= 6;
			segmentBack->clipMaskCount++;
		}
	}
}