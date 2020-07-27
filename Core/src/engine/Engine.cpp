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

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "maths/Vector2.h"
#include "maths/MatrixTransform.h"

using namespace std;
using namespace Core;

// Register all component types
#include "entity/component/ComponentRegistry.h"
static bool registered = Core::ComponentRegistry::registerComponentTypes();

void Engine::resizeViewport(unsigned int width, unsigned int height) {
	// Viewport
	glViewport(0, 0, width, height);
	// Updates projection matrix of all shaders
	Matrix4 projection = maths::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getInstance().updateShaders(projection);
	// Resize draw area
	graphics.getRenderer().updateSize(width, height);
	// Notify UIBehaviours
	guiSystem.onWindowResize(); // TODO: onViewportResize?
}

// -------------------------------Callbacks----------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

	engine->resizeViewport(width, height);
}

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


// ------------------------------- Input callbacks -----------------------------------

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key >= 0 && key < 1024) {
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

		InputEvent event;
		event.type = INPUT_EVENT_KEY;
		event.key.keycode = key;
		event.key.scancode = scancode;
		event.key.action = action;
		event.key.mods = mods;

		engine->getInput().addInputEvent(event);
	}
}

void characterCallback(GLFWwindow* window, unsigned int codepoint) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

	InputEvent event;
	event.type = INPUT_EVENT_CHARACTER;
	event.chr.codepoint = codepoint;

	engine->getInput().addInputEvent(event);
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->getInput().setMousePosition(Vector2(xpos, ypos));
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

	InputEvent event;
	event.type = INPUT_EVENT_MOUSEBUTTON;
	event.mouseButton.buttoncode = button;
	event.mouseButton.action = action;
	event.mouseButton.mods = mods;

	engine->getInput().addInputEvent(event);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

	InputEvent event;
	event.type = INPUT_EVENT_SCROLL;
	event.scroll.xoffset = (float)xoffset;
	event.scroll.yoffset = (float)yoffset;

	engine->getInput().addInputEvent(event);
}

// -------------------------------End of Callbacks----------------------------------

/* Stops the gameloop. */
void Engine::terminate() {
	running = false;
}

/* Prepares for the gameloop. */
int Engine::initiate(bool skipCallbacks) {
	glfwSetErrorCallback(glfw_error_callback);

	// Initialize systems
	// Graphics
	graphics.createWindow("Test", 800, 600);
	if (!graphics.initiate()) return -1;
	Window& window = graphics.getWindow();
	// Physics
	// Input
	// End of system initialization

	//Callbacks
	if (!skipCallbacks) {
		// Input
		glfwSetWindowUserPointer(window.getWindow(), this);
		glfwSetKeyCallback(window.getWindow(), keyCallback);
		glfwSetCharCallback(window.getWindow(), characterCallback);
		glfwSetCursorPosCallback(window.getWindow(), cursorPositionCallback);
		glfwSetMouseButtonCallback(window.getWindow(), mouseButtonCallback);
		glfwSetScrollCallback(window.getWindow(), scrollCallback);
		// Graphics
		glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);
	}

	return 0;
}

int Engine::start() {
	Window& window = graphics.getWindow();

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = (GLfloat)glfwGetTime();

	// Game loop
	running = true;
	while (running && window.isActive()) {
		// Calculate delta time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		tick(deltaTime);

		window.update();

		// 1ms sleep
		Sleep(1);
	}

	glfwTerminate();
	return 0;
}

void Engine::tick(float deltaTime) {
	// Poll events
	glfwPollEvents();

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