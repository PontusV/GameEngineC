#include "Engine.h"
#include "Level.h"
#include "Window.h"
#include "FpsCounter.h"
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace Core;

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
	graphics->createWindow("Test", 800, 600);
	if (!graphics->initiate()) return -1;
	Window& window = graphics->getWindow();
	// Physics
	// Input
	glfwSetWindowUserPointer(window.getWindow(), input); // TODO: Change user to Engine
	glfwSetKeyCallback(window.getWindow(), Input::keyCallback);
	glfwSetCursorPosCallback(window.getWindow(), Input::cursorPositionCallback);
	glfwSetMouseButtonCallback(window.getWindow(), Input::mouseButtonCallback);
	// End of system initialization

	return 0;
}
int Engine::start() {
	Window& window = graphics->getWindow();

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	FpsCounter fpsCounter;
	unsigned short debugLayer = graphics->createLayer();
	EntityHandle fpsDisplay = debugLevel->createEntity("FPS_Display",
		new Text("FPS: 0", "resources/fonts/cambriab.ttf", 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), debugLayer),
		new Transform(500, 5, 0, TransformAnchor::TOP_LEFT)
	);

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
		input->update(deltaTime);
		graphics->update(deltaTime);
		physics->update(deltaTime);

		// Render
		graphics->render();

		// 1ms sleep
		Sleep(1);
	}

	glfwTerminate();
	return 0;
}

Engine::Engine() {
	graphics = new Graphics();
	input = new Input(this);
	physics = new Physics();
}


Engine::~Engine() {
	delete input;
	delete physics;
	delete graphics;

	//Unload all entities and components
	if (currentLevel)
		currentLevel->clear();
}

Input& Engine::getInput() {
	return *input;
}

Graphics& Engine::getGraphics() {
	return *graphics;
}

LevelPtr Engine::getCurrentLevel() {
	return currentLevel;
}
LevelPtr Engine::getDebugLevel() {
	return debugLevel;
}