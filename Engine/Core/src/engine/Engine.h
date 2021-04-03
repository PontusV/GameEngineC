#ifndef ENGINE_H
#define ENGINE_H

#include <Core/Engine.h>
#include "entity/EntityManager.h"
#include "scene/SceneManager.h"
#include "graphics/Graphics.h"
#include "physics/Physics.h"
#include "input/Input.h"
#include "ui/GUISystem.h"
#include "BehaviourManager.h"
#include <memory>

struct SDL_Renderer;

namespace Core {
	class Engine : public IEngine {
	public:
		Engine();
		~Engine();

		/* Initializes the engine and all systems in preparation of start */
		int initiate(bool skipGLFW = false);
		/* Starts the game loop */
		int start();
		/* Runs one tick of the game. Handles input, updates systems and renders */
		void tick(float deltaTime);
		/* Stops the game loop and terminates glfw */
		void terminate();
		/* Used by DLL interface. Deletes this instance */
		void release() override;

		void resizeViewport(unsigned int width, unsigned int height);

		// GLFW Callbacks
		void framebufferSizeCallback(int width, int height);
		void keyCallback(int key, int scancode, int action, int mods);
		void characterCallback(unsigned int codepoint);
		void cursorPositionCallback(double xpos, double ypos);
		void mouseButtonCallback(int button, int action, int mods);
		void scrollCallback(double xoffset, double yoffset);

		// Systems
		Input& getInput();
		Graphics& getGraphics();
		Physics& getPhysics();
		EntityManager& getEntityManager();
		SceneManager& getSceneManager();
		BehaviourManager& getBehaviourManager();
		GUISystem& getGUISystem();

		IInput* getInputInterface() override;
		IGraphics* getGraphicsInterface() override;
		ISceneManager* getSceneManagerInterface() override;
		IEntityManager* getEntityManagerInterface() override;
	private:
		// Game loop
		bool running = false;

		// Scene
		ScenePtr debugScene; // Where you put entities showing debugging data. These will not be loaded/saved

		// Main Systems
		EntityManager entityManager;
		SceneManager sceneManager;
		Graphics graphics;
		Input input;
		Physics physics;
		GUISystem guiSystem;

		BehaviourManager behaviourManager;
	};
}
#endif