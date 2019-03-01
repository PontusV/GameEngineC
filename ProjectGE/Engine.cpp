#include "Engine.h"
#include "Level.h"
#include <iostream>
#include <fstream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"

using namespace std;
using namespace GameEngine;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		//if (action == GLFW_PRESS)
		//	game.Keys[key] = GL_TRUE;
		//else if (action == GLFW_RELEASE)
		//	game.Keys[key] = GL_FALSE;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getInstance().updateShaders(projection);
}

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
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//glfwWindowHint(GLFW_SAMPLES, 16); //Anti-alias

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Test", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); //0 = DISABLES VSYNC, 1 = ENABLES VSYNC
	glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize game
	graphics.initiate(SCREEN_WIDTH, SCREEN_HEIGHT);

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// FPS counter variables
	uint32_t fpsCounter = 0;
	float fpsPrintTimer = 0;
	// Game loop
	running = true;
	while (running && !glfwWindowShouldClose(window))
	{
		// Calculate delta time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		fpsCounter++;
		fpsPrintTimer += deltaTime;
		if (fpsPrintTimer > 1.0f) {
			fpsPrintTimer -= 1.0f;
			std::cout << "Fps: " << fpsCounter << "\n";
			fpsCounter = 0;
		}

		glfwPollEvents();

		//deltaTime = 0.001f;
		// Manage user input
		//game.ProcessInput(deltaTime);

		// Update Game state
		//game.Update(deltaTime);

		// Render
		graphics.render();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
	/*running = true;

	uint32_t setFPS = 200; //Setting it over 1000 removes the CAP (FIX THIS!). At higher deltaTime the fps goes down even if it doesnt have to. Recalculate wait time to make it more accurate!
	float timePerFrame = (float)1000/setFPS;
	uint32_t fpsCounter = 0;
	float fpsPrintTimer = 0;
	//Gameloop
	auto lastTime = std::chrono::high_resolution_clock::now();
	while (running) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = float(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count());
		lastTime = currentTime;

		fpsCounter++;
		fpsPrintTimer += deltaTime;
		if (fpsPrintTimer > 1000) {
			fpsPrintTimer -= 1000;
			std::cout << "Fps: " << fpsCounter << "\n";
			fpsCounter = 0;
		}

		//Look for input events from SDL
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				terminate();
			}
			input.processInputEvent(event);
		}
		graphics.update(deltaTime);

		int sleepTime = int(timePerFrame*2 - deltaTime);
		if (sleepTime > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		}
	}*/
}//*/

Engine::Engine() : input(this) {
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

LevelPtr Engine::getCurrentLevel() {
	return currentLevel;
}