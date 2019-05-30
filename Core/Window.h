#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <glm/glm.hpp>

struct GLFWwindow;
namespace Core {

	class Window {
	public:
		Window(const char* title, int width, int height);
		Window();
		~Window();

		bool init();
		void clear() const;
		/* Swaps buffer */
		void update() const;
		void close();

		int getWidth();
		int getHeight();
		const glm::ivec2& getResolution();
		bool isActive() const;

		void setBackgroundColor(glm::vec3 color);

		GLFWwindow* getWindow();

	private:
		const char* title;
		glm::ivec2 resolution;
		glm::vec3 backgroundColor;

		bool active;

		GLFWwindow* window;
	};
}
#endif