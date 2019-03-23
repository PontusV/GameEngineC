#ifndef BATCH_RENDERER_2D
#define BATCH_RENDERER_2D

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "BatchConfig.h"
#include "Renderable2D.h"
#include <vector>

namespace Core {

	class Window;

	struct VertexData
	{
		glm::vec2 vertex;
		//glm::vec4 color;
		GLuint color;
		glm::vec2 texture;
		float textureID;
	};

#define RENDERER_MAX_SPRITES	10000
#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_MAX_VERTICES	RENDERER_BUFFER_SIZE / RENDERER_VERTEX_SIZE
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6

#define SHADER_VERTEX_INDEX 0
#define SHADER_COLOR_INDEX 1
#define SHADER_TEXTURE_INDEX 2
#define SHADER_TEXTURE_ID_INDEX 3

	struct BatchSegment {
		GLsizei		startIndex;
		GLsizei		size;
		BatchConfig config;
	};

	class BatchRenderer2D
	{
	public:
		void startNewSegment(BatchConfig config);
	private:

	public:
		BatchRenderer2D(Window* window);
		~BatchRenderer2D();
		// Frees up memory
		void cleanUp();

		// Reuse VBO buffer
		BatchConfig& getConfig();
		inline bool hasRoom();
		inline bool hasBegun() { return begun; }

		void init();
		void begin();
		void submit(const RenderCopy2D& sprite);
		void end();
		void flush();
	private:
		bool						begun, initialized;
		GLuint						VAO, VBO, EBO;
		GLsizei						indexCount, verticiesCount;
		VertexData*					buffer;
		BatchConfig*				config;
		std::vector<BatchSegment>	segments;
		Window* window;
	};
}
#endif