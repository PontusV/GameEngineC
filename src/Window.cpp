#include <SDL.h>
#include "Window.h"
using namespace GameEngine;

Window::Window(const char *title, int x, int y, int width, int height, Uint32 flags) {
	win = SDL_CreateWindow(title, x, y, width, height, flags);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
}

Window::~Window() {
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
}

SDL_Window* Window::getWindow() const {
	return win;
}

SDL_Renderer* Window::getRenderer() const {
	return ren;
}

void Window::update() {
	SDL_RenderPresent(ren);
	SDL_RenderClear(ren);
}