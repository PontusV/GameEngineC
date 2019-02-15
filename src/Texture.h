#ifndef TEXTURE_H
#define TEXTURE_H

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;

namespace GameEngine {

	class Texture {
	public:
		~Texture();
		const int& getWidth() const;
		const int& getHeight() const;
		SDL_Texture* getTexture() const;

		static Texture* load(SDL_Surface* image, SDL_Renderer* ren);
	protected:
		Texture(SDL_Texture* tx, int width, int height);

		int width, height;
		SDL_Texture* tx;
	};
}
#endif