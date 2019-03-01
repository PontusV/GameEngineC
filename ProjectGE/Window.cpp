#include "Window.h"
using namespace GameEngine;

Window::Window(const char *title, int x, int y, int width, int height) {
	create(title, x, y, width, height);
}
Window::Window() : active(false) {

}

Window::~Window() {
	//if (ren) SDL_DestroyRenderer(ren);
	//if (win) SDL_DestroyWindow(win);
}

/* Destroys current window if there is any. Creates new one with the given parameters. */
void Window::create(const char *title, int x, int y, int width, int height) {


	active = true;
}

bool Window::isActive() {
	return active;
}

/*SDL_Window* Window::getWindow() const {
	return win;
}

SDL_Renderer* Window::getRenderer() const {
	return ren;
}*/

void Window::update() {
	//SDL_RenderPresent(ren);
	//SDL_RenderClear(ren);
}