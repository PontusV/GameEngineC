#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <GLFW/glfw3.h>

namespace Core {

	class Window {
	public:
		Window(const char* title, int width, int height);
		Window();
		~Window();

		bool init();
		void update() const;
		void close();

		int getWidth();
		int getHeight();
		bool isActive() const;

		GLFWwindow* getWindow();
		//void setKeyCallBack(GLFWkeyfun fun);
		//void setFramebufferSizeCallback(GLFWframebuffersizefun fun);
	private:
		const char* title;
		int width, height;
		GLFWwindow* window;

		bool active;
	};
}
#endif