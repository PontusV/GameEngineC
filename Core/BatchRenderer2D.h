#ifndef BATCH_RENDERER_2D
#define BATCH_RENDERER_2D

#include "BatchConfig.h"
#include "Renderable2D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace Core {

	class Window;

	struct VertexData
	{
		glm::vec2		vertex;
		unsigned int	color;
		glm::vec2		texture;
		float			textureID;

		//float x, y; // Position
		//float u, v; // Texture
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
		std::size_t	startIndex;
		std::size_t size;
		std::size_t clipMaskCount;
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

		void init();
		void begin();
		void submit(const Renderable2D& sprite);
		void end();
		void flush();

		void submitMask(glm::vec2 vertex1, glm::vec2 vertex2, glm::vec2 vertex3, glm::vec2 vertex4);

	private:
		unsigned int maskShaderID;

	private:
		unsigned int				VAO, VBO, EBO;
		std::size_t					indexCount, verticiesCount;
		VertexData*					buffer;
		BatchConfig*				config;
		std::vector<BatchSegment>	segments;
		BatchSegment*				segmentBack;
		Window* window;
	};
}
#endif