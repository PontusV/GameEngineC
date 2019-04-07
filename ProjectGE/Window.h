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
		bool isActive() const;

		void setBackgroundColor(glm::vec3 color);

		GLFWwindow* getWindow();

	private:
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	private:
		const char* title;
		int width, height;
		GLFWwindow* window;

		glm::vec3 backgroundColor;

		bool active;
	};
}
#endif