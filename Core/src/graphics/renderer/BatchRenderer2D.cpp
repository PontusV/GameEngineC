#include "BatchRenderer2D.h"
#include "graphics/data/Texture2D.h"
#include "maths/MatrixTransform.h"
#include "graphics/Window.h"

#include "engine/ResourceManager.h"

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
	maskShaderID = ResourceManager::getInstance().loadShader("resources/shaders/sprite").ID;
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

bool BatchRenderer2D::hasRoom() {
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
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // TODO??????
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


	const Vector2*	vertices	= renderable.vertices;
	const Vector2*	uvCoords	= renderable.uvCoords;
	const Color&	color		= renderable.color;

	const float& textureSlot = renderable.textureID == 0 ? 0 : (float)config->getTextureSlot(renderable.textureID);

	int r = (int)(color.r);
	int g = (int)(color.g);
	int b = (int)(color.b);
	int a = (int)(color.a);

	unsigned int c = a << 24 | b << 16 | g << 8 | r;

	std::size_t& i = verticiesCount;

	buffer[i].vertex = vertices[0].floor();
	buffer[i].color = c;
	buffer[i].texture = uvCoords[0];
	buffer[i].textureID = textureSlot;
	i++;

	buffer[i].vertex = vertices[1].floor();
	buffer[i].color = c;
	buffer[i].texture = uvCoords[1];
	buffer[i].textureID = textureSlot;
	i++;

	buffer[i].vertex = vertices[2].floor();
	buffer[i].color = c;
	buffer[i].texture = uvCoords[2];
	buffer[i].textureID = textureSlot;
	i++;

	buffer[i].vertex = vertices[3].floor();
	buffer[i].color = c;
	buffer[i].texture = uvCoords[3];
	buffer[i].textureID = textureSlot;
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

		glUseProgram(shaderID);

		//Draw
		glDrawElements(GL_TRIANGLES, segment.size, GL_UNSIGNED_SHORT, (void*)(segment.startIndex * sizeof(GLushort)));

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
}