#include "BatchRenderer2D.h"
#include "Texture2D.h"
#include "TransformMaths.h" //WIP not used atm
#include "Window.h"

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

BatchRenderer2D::BatchRenderer2D(Window* window) : initialized(false), window(window)
{
}


BatchRenderer2D::~BatchRenderer2D()
{
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
	initialized = false;
	begun = false;
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
	begun = false;
	initialized = true;
}

void BatchRenderer2D::begin() {
	begun = true;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void BatchRenderer2D::submit(const RenderCopy2D& renderable) {
	if (!hasRoom())
		throw std::length_error("Can not add anymore sprites to this batch!");


	const glm::vec2& position		= renderable.transform.getPosition();
	const glm::vec2& size			= renderable.size;
	const glm::vec4& color			= renderable.color;
	const glm::vec2* uvPos			= renderable.texture.uvPos;
	const glm::vec2& imageOffset	= renderable.imageOffset;

	const float& textureSlot = renderable.texture.ID == 0 ? 0 : (float)config->getTextureSlot(renderable.texture.ID);

	int r = (int)(color.x * 255.0f);
	int g = (int)(color.y * 255.0f);
	int b = (int)(color.z * 255.0f);
	int a = (int)(color.w * 255.0f);

	unsigned int c = a << 24 | b << 16 | g << 8 | r;

	GLsizei& i = verticiesCount;
	/*//
	glm::mat4 transformationStack;
	glm::vec4 pos(1, 1, 1, 1);
	transformationStack * pos;

	glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	//*/

	//position = renderable.transform.getTruePosition(size);
	//if (renderable.transform.getAnchor() == TransformAnchor::CENTER)
	//	position = glm::vec2(position.x - (float)size.x/2, position.y - (float)size.y/2); // Image offset
	//position += renderable.imagePosition;

	//glm::vec2 imgPos = glm::vec2(size.x, size.y);

	buffer[i].vertex = position + imageOffset;
	buffer[i].color = c;
	buffer[i].texture = uvPos[0];
	buffer[i].textureID = textureSlot;
	i++;

	buffer[i].vertex = glm::vec2(position.x, position.y + size.y) + imageOffset;
	buffer[i].color = c;
	buffer[i].texture = uvPos[1];
	buffer[i].textureID = textureSlot;
	i++;

	buffer[i].vertex = glm::vec2(position.x + size.x, position.y + size.y) + imageOffset;
	buffer[i].color = c;
	buffer[i].texture = uvPos[2];
	buffer[i].textureID = textureSlot;
	i++;

	buffer[i].vertex = glm::vec2(position.x + size.x, position.y) + imageOffset;
	buffer[i].color = c;
	buffer[i].texture = uvPos[3];
	buffer[i].textureID = textureSlot;
	i++;

	segments.back().size += 6;
	indexCount += 6;
}

void BatchRenderer2D::end() {
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	begun = false;
}

void BatchRenderer2D::flush() {
	// Is there anything to draw?
	if (indexCount == 0) return;

	for (BatchSegment& segment : segments) {
		BatchConfig& config = segment.config;

		const GLuint& shaderID = config.shaderID;
		const GLfloat& rotate = config.rotation;

		glUseProgram(shaderID);

		/*if (rotate != 0) { //Check for rotation, WORK IN PROGRESS
			glm::mat4 model;
			model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		}*/

		for (std::size_t i = 0; i < config.textureIDs.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, config.textureIDs[i]);
		}

		//-------------
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		if (config.clipEnabled) {
			glEnable(GL_SCISSOR_TEST);
			glScissor((GLint)config.drawRect.x,(GLint)(window->getHeight() - config.drawRect.y - config.drawRect.w),(GLsizei)config.drawRect.z,(GLsizei)config.drawRect.w);
		}

		glDrawElements(GL_TRIANGLES, segment.size, GL_UNSIGNED_SHORT, (void*)(segment.startIndex * sizeof(GLushort)));
		if (config.clipEnabled) {
			glDisable(GL_SCISSOR_TEST);
		}
		config.textureIDs.clear();

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	indexCount = 0;
	verticiesCount = 0;
	segments.clear();
}


void BatchRenderer2D::startNewSegment(BatchConfig config) {
	BatchSegment segment{indexCount, 0, config};
	segments.push_back(segment);
	this->config = &segments.back().config;

	if (!begun)	begin();
}