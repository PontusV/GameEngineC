#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

namespace Editor {

	class Viewport {
	public:
		Viewport();
		~Viewport();

		void initialize(unsigned int width, unsigned int height);

		/* Resizes the viewport texture. */
		void setSize(unsigned int width, unsigned int height);

		void begin();
		void end();

		unsigned int getTextureID();

	private:
		unsigned int textureID;

		unsigned int FBO;
		unsigned int RBO;
		unsigned int VAO, VBO;

		/* Initializes render data(VAO & VBO). */
		void init();
	};
}
#endif