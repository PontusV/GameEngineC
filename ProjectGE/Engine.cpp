#include "Engine.h"
#include "Level.h"
#include "FpsCounter.h"
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "ResourceManager.h"
#include "Window.h"

using namespace std;
using namespace Core;

/* Saves current level. */
void Engine::saveLevel() { //To be added: file location of level map (currently hard coded to the map Levels)
	ofstream file;
	file.open("Levels/level.dat", ios::out | ios::binary | ios::trunc);
	currentLevel->serialize(file);
	file.close();
}

/* Loads level from file. */
void Engine::loadLevel() { //To be added: file location of level map (currently hard coded to the map Levels)
	ifstream file;
	file.open("Levels/level.dat", ios::in | ios::binary);
	//
	LevelPtr level = std::make_shared<Level>();
	level->deserialize(file);
	//
	file.close();

	currentLevel = level; //Changes level (Might want to move this to a changeLevel function)
}

LevelPtr Engine::createLevel() {
	LevelPtr level = std::make_shared<Level>();
	currentLevel = level;
	return level;
}

/* Stops the gameloop. */
void Engine::terminate() {
	running = false;
}

/* Starts the gameloop. */
int Engine::initiate() {
	// Initialize systems
	// Graphics
	graphics->createWindow("Test", 800, 600);
	if (!graphics->initiate()) return -1;
	Window& window = graphics->getWindow();
	// Physics
	// Input
	glfwSetWindowUserPointer(window.getWindow(), input);
	glfwSetKeyCallback(window.getWindow(), Input::keyCallback);
	glfwSetCursorPosCallback(window.getWindow(), Input::cursorPositionCallback);
	glfwSetMouseButtonCallback(window.getWindow(), Input::mouseButtonCallback);
	// etc...
	//End of system initialization

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	FpsCounter fpsCounter;
	EntityHandle fpsDisplay = currentLevel->createEntity("FPS_Display",
		new Text("FPS: 0", "resources/fonts/cambriab.ttf", 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
		new Transform(5, 5, 0, TransformAnchor::TOP_LEFT)
	);

	// Game loop
	running = true;
	while (running && window.isActive())
	{
		// Calculate delta time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Fps
		Text* text = fpsDisplay.getComponent<Text>();
		fpsCounter.tick(deltaTime, text);
		//fpsCounter.tick(deltaTime);

		// Poll events
		glfwPollEvents();

		// Update systems
		input->update(deltaTime);
		graphics->update(deltaTime);

		// Render
		graphics->render();
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