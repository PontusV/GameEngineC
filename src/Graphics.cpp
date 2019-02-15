#include "Graphics.h"
#include "Window.h"
#include "Image.h"
#include "Text.h"
#include "HandleManager.h"
#include "Handle.h"
using namespace GameEngine;


Graphics::Graphics(HandleManager& handleManager) : handleManager(handleManager) {}


Graphics::~Graphics() {
	delete window;
}

void Graphics::initiate() {
	
}

/* Loops through Renderable vector and draws them.*/
void Graphics::update(float dt) {
	std::size_t size = renderables.images.size();

	for (std::size_t i = 0; i < size; i++) {
		Image& image = renderables.images[i];
		Transform& transform = renderables.transforms[i];
		image.reload(window->getRenderer()); //Make sure image is loaded

		//Get draw variables
		int x = int(transform.getX());
		int y = int(transform.getY());
		int width = image.getTexture()->getWidth();
		int height = image.getTexture()->getHeight();

		//Draw
		SDL_Rect dest{ x, y, width, height };
		SDL_RenderCopy(window->getRenderer(), image.getTexture()->getTexture(), NULL, &dest);
	}
	window->update();
}

/* Destroys any existing window and renderer and creates new ones */
void Graphics::createWindow() {
	//Destroy old window
	if (window != nullptr)
		delete window;

	//Create new window
	window = new Window("Hello", 0, 0, 1680, 1050, 0);
}