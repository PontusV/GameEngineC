#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include "maths/Matrix4.h"
#include "maths/Vector2.h"
#include "maths/Vector3.h"

typedef void* (*GLADloadproc)(const char* name);
struct GLFWwindow; // Defined in IWindow
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

		void setBackgroundColor(float r, float g, float b);
		void setBackgroundColor(Vector3 color);

		Matrix4 getProjectionMatrix();
		void getProjectionMatrixData(float out[16]);

		GLFWwindow* getWindow();
		GLADloadproc getGLADloadproc();
		/* DO NOT USE. Used by Editor. */
		bool initGLAD(GLADloadproc glfwGetProcAddress);
		/* DO NOT USE. Used by Editor */
		void setActive(bool value);
		bool getActive();
		/* DO NOT USE. Used by Editor */
		void setResolution(float width, float height);
	private:
		const char* title;
		Vector2 resolution;
		Vector3 backgroundColor = Vector3(0,0,0);

		bool active;
		bool initialized = false;

		GLFWwindow* window;
	};
}
#endif