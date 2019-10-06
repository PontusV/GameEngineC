#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include "Maths/Matrix4.h"
#include "Maths/Vector2.h"
#include "Maths/Vector3.h"

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
		const Vector2& getResolution();
		bool isActive() const;

		void setBackgroundColor(Vector3 color);

		GLFWwindow* getWindow();
		Matrix4 getProjectionMatrix();

	private:
		const char* title;
		Vector2 resolution;
		Vector3 backgroundColor = Vector3(0,0,0);

		bool active;

		GLFWwindow* window;
	};
}
#endif