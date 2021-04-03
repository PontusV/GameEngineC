#ifndef VIEW_PORT_H
#define VIEW_PORT_H

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
	};
}
#endif