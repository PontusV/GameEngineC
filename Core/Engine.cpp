#include "Engine.h"
#include "Scene.h"
#include "Window.h"
#include "FpsCounter.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "WindowAnchor.h"
#include "Text.h"

#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Maths/Vector2.h"
#include "Maths/MatrixTransform.h"

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
	Matrix4 projection = maths::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getInstance().updateShaders(projection);
	// Resize draw area
	engine->getGraphics().getRenderer().updateSize(width, height);
	// Notify UIBehaviours
	engine->getGraphics().getUISystem().onWindowResize();
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


void Engine::saveScene(ScenePtr scene, const char* fileName) { //To be added: file location of scene map (currently hard coded to the map Scenes)
	ofstream file;
	std::string sceneDir("Scenes/");
	sceneDir.append(fileName);
	file.open(sceneDir, ios::out | ios::binary | ios::trunc);
	scene->serialize(file);
	file.close();

	std::cout << "Saved scene to: " << sceneDir << "\n";
}

ScenePtr Engine::loadScene(const char* fileName) { //To be added: file location of scene map (currently hard coded to the map Scenes)
	ifstream file;
	std::string sceneDir("sceneDir/");
	sceneDir.append(fileName);
	std::cout << "Loading Scene: " << sceneDir << "\n";
	file.open(sceneDir, ios::in | ios::binary);
	//
	ScenePtr scene = std::make_shared<Scene>(&entityManager, ObjectType::World); // TODO: Save & Load type
	scene->deserialize(file);
	//
	file.close();
	scene->awake();

	return scene;
}

ScenePtr Engine::createScene(std::string name, ObjectType type) { // TODO: Link name to Scene
	ScenePtr scene = std::make_shared<Scene>(&entityManager, type);
	scenes.push_back(scene);
	return scene;
}

/* Stops the gameloop. */
void Engine::terminate() {
	running = false;
}

/* Starts the gameloop. */
int Engine::initiate() {
	// Create Empty Debug Scene
	debugScene = createScene("Debug", ObjectType::UI);

	// Initialize systems
	// Graphics
	graphics.createWindow("Test", 800, 600);
	if (!graphics.initiate()) return -1;
	Window& window = graphics.getWindow();
	// Physics
	// Input
	glfwSetWindowUserPointer(window.getWindow(), this);
	glfwSetKeyCallback(window.getWindow(), keyCallback);
	glfwSetCharCallback(window.getWindow(), characterCallback);
	glfwSetCursorPosCallback(window.getWindow(), cursorPositionCallback);
	glfwSetMouseButtonCallback(window.getWindow(), mouseButtonCallback);
	glfwSetScrollCallback(window.getWindow(), scrollCallback);
	// End of system initialization

	return 0;
}
int Engine::start() {
	Window& window = graphics.getWindow();

	//Callbacks
	glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);

	FpsCounter fpsCounter;
	//unsigned char debugLayer = graphics.createLayer();
	EntityHandle fpsDisplay = debugScene->createEntity("FPS_Display",
		Text("Fps: 0", "resources/fonts/cambriab.ttf", 20, Color(255, 255, 255, 255)),
		WindowAnchor(Alignment::BOTTOM_LEFT, 5, -10),
		RectTransform(500, 5, 0, 0, 30, Alignment::BOTTOM_LEFT)
	);
	debugScene->awake();

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
		behaviourManager.update(deltaTime);
		for (ScenePtr& scene : scenes) {
			scene->processQueue();
		}
		graphics.update(deltaTime);
		physics.update(deltaTime);

		// Render
		behaviourManager.onPreRender();
		graphics.render(deltaTime);
		behaviourManager.onPostRender();


		// 1ms sleep
		Sleep(1);
	}

	glfwTerminate();
	return 0;
}

Engine::Engine() : graphics(), input(this), physics(), behaviourManager(this) {
}
Engine::~Engine() {
	// Unload all entities and components
	for (ScenePtr& scene : scenes) {
		scene->clear();
	}
}

EntityManager& Engine::getEntityManager() {
	return entityManager;
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