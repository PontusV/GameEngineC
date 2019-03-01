#ifndef WINDOW_H
#define WINDOW_H

#include <vector>

struct SDL_Window;
struct SDL_Renderer;
namespace GameEngine {

	class Window {
	public:
		Window(const char *title, int x, int y, int width, int height);
		Window();
		~Window();
		void create(const char *title, int x, int y, int width, int height);

		bool isActive();
		SDL_Window* getWindow() const;
		SDL_Renderer* getRenderer() const;
		void update();
	private:
		class Component;
		SDL_Window* win;
		SDL_Renderer* ren;
		bool active;
	};
}
#endif