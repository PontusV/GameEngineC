#include "Engine.h"
#include "ResourceManager.h"
#include "FpsCounter.h"
#include "scene/Scene.h"
#include "graphics/Window.h"
#include "graphics/renderer/Renderer2D.h"
#include "components/ui/WindowAnchor.h"
#include "components/graphics/Text.h"

#include <iostream>
#include <fstream>

#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "maths/Vector2.h"
#include "maths/MatrixTransform.h"

using namespace std;
using namespace Core;

// Register all component types
#include "entity/component/ComponentRegistry.h"
static bool registered = Core::ComponentRegistry::registerComponentTypes();

// -------------------------------GLFW Callbacks----------------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->framebufferSizeCallback(width, height);
}

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Engine Glfw Error %d: %s\n", error, description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->keyCallback(key, scancode, action, mods);
}

void character_callback(GLFWwindow* window, unsigned int codepoint) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->characterCallback(codepoint);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->cursorPositionCallback(xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->mouseButtonCallback(button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->scrollCallback(xoffset, yoffset);
}

// -------------------------------Callbacks----------------------------------

void Engine::framebufferSizeCallback(int width, int height) {
	resizeViewport(width, height);
}
void Engine::keyCallback(int key, int scancode, int action, int mods) {
	if (key >= 0 && key < 1024) {
		InputEvent event;
		event.type = INPUT_EVENT_KEY;
		event.key.keycode = key;
		event.key.scancode = scancode;
		event.key.action = action;
		event.key.mods = mods;

		input.addInputEvent(event);
	}
}
void Engine::characterCallback(unsigned int codepoint) {
	InputEvent event;
	event.type = INPUT_EVENT_CHARACTER;
	event.chr.codepoint = codepoint;

	input.addInputEvent(event);
}
void Engine::cursorPositionCallback(double xpos, double ypos) {
	input.setMousePosition(Vector2(xpos, ypos));
}
void Engine::mouseButtonCallback(int button, int action, int mods) {
	InputEvent event;
	event.type = INPUT_EVENT_MOUSEBUTTON;
	event.mouseButton.buttoncode = button;
	event.mouseButton.action = action;
	event.mouseButton.mods = mods;

	input.addInputEvent(event);
}
void Engine::scrollCallback(double xoffset, double yoffset) {
	InputEvent event;
	event.type = INPUT_EVENT_SCROLL;
	event.scroll.xoffset = (float)xoffset;
	event.scroll.yoffset = (float)yoffset;

	input.addInputEvent(event);
}
// -------------------------------End of Callbacks----------------------------------

void Engine::resizeViewport(unsigned int width, unsigned int height) {
	// Viewport
	glViewport(0, 0, width, height);
	// Updates projection matrix of all shaders
	Matrix4 projection = maths::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getInstance().updateShaders(projection);
	// Resize draw area
	graphics.getWindow().setResolution(width, height);
	graphics.getRenderer().updateSize(width, height);
	graphics.getCamera().updateViewMatrix();
	// Notify UIBehaviours
	guiSystem.onWindowResize(); // TODO: onViewportResize?
}

/* Stops the gameloop. */
void Engine::terminate() {
	running = false;
}

/* Prepares for the gameloop. */
int Engine::initiate(bool skipGLFW) {
	glfwSetErrorCallback(glfw_error_callback);

	// Initialize systems
	// Graphics
	if (!skipGLFW) {
		graphics.createWindow("Test", 800, 600);
		Window& window = graphics.getWindow();
		// Callbacks
		// Input
		glfwSetWindowUserPointer(window.getWindow(), this);
		glfwSetKeyCallback(window.getWindow(), key_callback);
		glfwSetCharCallback(window.getWindow(), character_callback);
		glfwSetCursorPosCallback(window.getWindow(), cursor_position_callback);
		glfwSetMouseButtonCallback(window.getWindow(), mouse_button_callback);
		glfwSetScrollCallback(window.getWindow(), scroll_callback);
		// Graphics
		glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);
	}
	if (!graphics.initiate()) return -1;
	// Physics
	// Input
	// End of system initialization

	return 0;
}

int Engine::start() {
	Window& window = graphics.getWindow();

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	std::chrono::time_point<std::chrono::system_clock> lastFrame = std::chrono::system_clock::now();

	// Game loop
	running = true;
	while (running && window.isActive()) {
		// Calculate delta time
		std::chrono::time_point<std::chrono::system_clock> currentFrame = std::chrono::system_clock::now();
		deltaTime = static_cast<GLfloat>(std::chrono::duration_cast<std::chrono::milliseconds>(currentFrame - lastFrame).count()) / 1000;
		lastFrame = currentFrame;

		// Poll events
		glfwPollEvents();

		window.clear();

		tick(deltaTime);

		window.update();

		// 1ms sleep
		Sleep(1);
	}

	glfwTerminate();
	return 0;
}

void Engine::tick(float deltaTime) {
	// Update systems
	sceneManager.update();
	input.update(deltaTime);
	behaviourManager.update(deltaTime);
	guiSystem.update();
	graphics.update(deltaTime);
	physics.update(deltaTime);

	// Render
	behaviourManager.onPreRender(deltaTime);
	graphics.render(deltaTime);
	behaviourManager.onPostRender(deltaTime);
}

Engine::Engine() : sceneManager(&entityManager), graphics(), input(this), physics(), behaviourManager(this) {
}

Engine::~Engine() {
}

EntityManager& Engine::getEntityManager() {
	return entityManager;
}

SceneManager& Engine::getSceneManager() {
	return sceneManager;
}

Input& Engine::getInput() {
	return input;
}

Graphics& Engine::getGraphics() {
	return graphics;
}

Physics& Engine::getPhysics() {
	return physics;
}

BehaviourManager& Engine::getBehaviourManager() {
	return behaviourManager;
}