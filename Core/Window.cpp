#include "Window.h"
#include "ResourceManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

	window = glfwCreateWindow(resolution.x, resolution.y, title, NULL, NULL);
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
	glViewport(0, 0, resolution.x, resolution.y);
	// Enable transparently
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Stencil (Test)
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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

void Window::setBackgroundColor(glm::vec3 color) {
	backgroundColor = color;
}

GLFWwindow* Window::getWindow() {
	return window;
}

void Window::close() {
	active = false;
}

int Window::getWidth() {
	glfwGetWindowSize(window, &resolution.x, &resolution.y);
	return resolution.x;
}

int Window::getHeight() {
	glfwGetWindowSize(window, &resolution.x, &resolution.y);
	return resolution.y;
}

const glm::ivec2& Window::getResolution() {
	glfwGetWindowSize(window, &resolution.x, &resolution.y);
	return resolution;
}