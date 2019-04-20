#include "Engine.h"
#include "Level.h"
#include "Window.h"
#include "FpsCounter.h"
#include "ResourceManager.h"
#include "Renderer2D.h"

#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace Core;

// Register all component types
#include "ComponentRegistry.h"
static bool registered = Core::ComponentRegistry::registerComponentTypes();

// -------------------------------Callbacks----------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

	// Viewport
	glViewport(0, 0, width, height);
	// Updates projection matrix of all shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getInstance().updateShaders(projection);
	// Resize draw area
	engine->getGraphics().getRenderer().updateSize(width, height);
}



// ------------------------------- Input callbacks -----------------------------------

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key >= 0 && key < 1024)
	{
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

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->getInput().setMousePos(glm::vec2(xpos, ypos));
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


/* Saves current level. */
void Engine::saveLevel(const char* fileName) { //To be added: file location of level map (currently hard coded to the map Levels)
	if (!currentLevel) {
		std::cout << "Engine::saveLevel::ERROR There is not any currently active level to save!\n";
		throw std::invalid_argument("Engine::saveLevel::ERROR There is not any currently active level to save!");
	}
	ofstream file;
	std::string levelDir("Levels/");
	levelDir.append(fileName);
	file.open(levelDir, ios::out | ios::binary | ios::trunc);
	currentLevel->serialize(file);
	file.close();

	std::cout << "Saved level to: " << levelDir << "\n";
}

/* Loads level from file. */
LevelPtr Engine::loadLevel(const char* fileName) { //To be added: file location of level map (currently hard coded to the map Levels)
	ifstream file;
	std::string levelDir("Levels/");
	levelDir.append(fileName);
	std::cout << "Loading level: " << levelDir << "\n";
	file.open(levelDir, ios::in | ios::binary);
	//
	LevelPtr level = std::make_shared<Level>();
	level->deserialize(file);
	//
	file.close();

	return level;
}

LevelPtr Engine::createLevel() {
	LevelPtr level = std::make_shared<Level>();
	return level;
}

void Engine::setCurrentLevel(LevelPtr level) {
	std::cout << "Changing level...\n";
	if (currentLevel)
		currentLevel->clear();

	currentLevel = level;
}

/* Stops the gameloop. */
void Engine::terminate() {
	running = false;
}

/* Starts the gameloop. */
int Engine::initiate() {
	// Create Empty Debug level
	debugLevel = createLevel();

	// Initialize systems
	// Graphics
	graphics.createWindow("Test", 800, 600);
	if (!graphics.initiate()) return -1;
	Window& window = graphics.getWindow();
	// Physics
	// Input
	glfwSetWindowUserPointer(window.getWindow(), this); // TODO: Change user to Engine
	glfwSetKeyCallback(window.getWindow(), keyCallback);
	glfwSetCursorPosCallback(window.getWindow(), cursorPositionCallback);
	glfwSetMouseButtonCallback(window.getWindow(), mouseButtonCallback);
	// End of system initialization

	return 0;
}
int Engine::start() {
	Window& window = graphics.getWindow();

	//Callbacks
	glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);

	FpsCounter fpsCounter;
	unsigned char debugLayer = graphics.createLayer();
	EntityHandle fpsDisplay = debugLevel->createEntity("FPS_Display",
		Text("Fps: 0", "resources/fonts/cambriab.ttf", 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), debugLayer),
		Transform(500, 5, 0, TransformAnchor::TOP_LEFT)
	);

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

		// Fps
		Text* text = fpsDisplay.getComponent<Text>();
		fpsCounter.tick(deltaTime, text);

		// Poll events
		glfwPollEvents();

		// Update systems
		input.update(deltaTime);
		scriptManager.update(deltaTime);
		physics.update(deltaTime);
		graphics.update(deltaTime);

		// Render
		graphics.render(deltaTime);

		// 1ms sleep
		Sleep(1);
	}

	glfwTerminate();
	return 0;
}

Engine::Engine() : graphics(), input(this), physics() {
}
Engine::~Engine() {
	//Unload all entities and components
	if (currentLevel)
		currentLevel->clear();
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

ScriptManager& Engine::getScriptManager() {
	return scriptManager;
}

LevelPtr Engine::getCurrentLevel() {
	return currentLevel;
}
LevelPtr Engine::getDebugLevel() {
	return debugLevel;
}