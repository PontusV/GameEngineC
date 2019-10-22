#include "Window.h"
#include "engine/ResourceManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "maths/MatrixTransform.h"
using namespace Core;


Window::Window(const char* title, int width, int height) : title(title), resolution(width, height) {
}
Window::Window() : active(false) {

}

Window::~Window() {
	glfwTerminate();
}

/* Destroys current window if there is any. Creates new one with the given parameters. */
bool Window::init() {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW!" << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//glfwWindowHint(GLFW_SAMPLES, 16); // Anti-alias (Makes Sprites blurry)

	window = glfwCreateWindow((int)resolution.x, (int)resolution.y, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		std::cout << "Failed to create GLFW window!" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); //0 = DISABLES VSYNC, 1 = ENABLES VSYNC

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// OpenGL configuration
	glViewport(0, 0, (int)resolution.x, (int)resolution.y);
	// Enable transparently
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Stencil (Test)
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glStencilMask(0x00); // Disable stencil writes
	// End of config

	active = true;
	return true;
}

void Window::clear() const {
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::update() const {
	glfwSwapBuffers(window);
}

bool Window::isActive() const {
	return active && !glfwWindowShouldClose(window);
}

void Window::setBackgroundColor(Vector3 color) {
	backgroundColor = color;
}

GLFWwindow* Window::getWindow() {
	return window;
}

void Window::close() {
	active = false;
}

int Window::getWidth() {
	getResolution();
	return (int)resolution.x;
}

int Window::getHeight() {
	getResolution();
	return (int)resolution.y;
}

const Vector2& Window::getResolution() {
	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);
	resolution = Vector2(width, height);
	return resolution;
}

Matrix4 Window::getProjectionMatrix() {
	getResolution();
	return maths::ortho(0.0f, static_cast<GLfloat>(resolution.x), static_cast<GLfloat>(resolution.y), 0.0f, -1.0f, 1.0f);
}