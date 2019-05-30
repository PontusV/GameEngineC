#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include "Shader.h"

namespace Core {

	class Window;

	class PostProcessor {
	public:
		//PostProcessor(Shader shader, unsigned int width, unsigned int height);
		PostProcessor(Window* window);
		~PostProcessor();

		/* Resizes the area drawn on the screen. The size should be the same as the window size. */
		void setSize(unsigned int width, unsigned int height);

		void begin();
		void end();
		void render(float time);

	public:
		// Effects
		void setGrayscale(bool value);
		void setInvert(bool value);

	private:
		Shader shader;
		unsigned int textureID;
		unsigned int width, height;

		unsigned int FBO;
		unsigned int RBO;
		unsigned int VAO, VBO;

		/* Initializes render data(VAO & VBO). */
		void init();

		Window* window;
	};
}
#endif