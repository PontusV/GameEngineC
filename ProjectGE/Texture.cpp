#include "Texture.h"
using namespace GameEngine;



Texture::Texture(SDL_Texture* tx, int width, int height) : tx(tx), width(width), height(height) {
}

Texture::~Texture() {
	//SDL_DestroyTexture(tx);
}

const int& Texture::getWidth() const {
	return width;
}

const int& Texture::getHeight() const {
	return height;
}

SDL_Texture* Texture::getTexture() const {
	return tx;
}

Texture* Texture::load(SDL_Surface* image, SDL_Renderer* ren) {
	/*if (image != nullptr) {
		int width = image->w;
		int height = image->h;
		SDL_Texture* tx = SDL_CreateTextureFromSurface(ren, image);
		SDL_FreeSurface(image);
		return new Texture(tx, width, height);
	}*/
	throw "Could not load texture! The provided image was a nullptr.";
}