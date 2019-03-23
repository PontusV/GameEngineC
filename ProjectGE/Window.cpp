#include "Window.h"
#include "ResourceManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace Core;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getInstance().updateShaders(projection);
}

Window::Window(const char* title, int width, int height) : title(title), width(width), height(height) {
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
	//glfwWindowHint(GLFW_SAMPLES, 16); //Anti-alias

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		std::cout << "Failed to create GLFW window!" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); //0 = DISABLES VSYNC, 1 = ENABLES VSYNC
	//
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// OpenGL configuration
	glViewport(0, 0, width, height);
	//Enable transparently
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Enable scissor for panels
	//glEnable(GL_SCISSOR_TEST);
	// End of config

	active = true;
	return true;
}

void Window::update() const {
	glfwSwapBuffers(window);
}

bool Window::isActive() const {
	return active && !glfwWindowShouldClose(window);
}


GLFWwindow* Window::getWindow() {
	return window;
}

void Window::close() {
	active = false;
}

int Window::getWidth() {
	return width;
}

int Window::getHeight() {
	return height;
}
/*void Window::setKeyCallBack(GLFWkeyfun fun) {
	glfwSetKeyCallback(window, fun);
}

void Window::setFramebufferSizeCallback(GLFWframebuffersizefun fun) {
	glfwSetFramebufferSizeCallback(window, fun);
}*/