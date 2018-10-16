#ifndef WINDOW_H
#define WINDOW_H

#include <vector>

class RenderComponent;
struct SDL_Window;
struct SDL_Renderer;

class Window {
public:
	Window(const char *title, int x, int y, int width, int height, Uint32 flags);
	~Window();
	SDL_Window* getWindow() const;
	SDL_Renderer* getRenderer() const;
	void addGraphics(RenderComponent* g);
	void update();
private:
	std::vector<RenderComponent*> drawList;
	class Component;
	SDL_Window* win;
	SDL_Renderer* ren;
};
#endif