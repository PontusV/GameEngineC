#ifndef ENGINE_H
#define ENGINE_H

#include "entity/EntityManager.h"
#include "event/EventSystem.h"
#include "graphics/Graphics.h"
#include "physics/Physics.h"
#include "input/Input.h"
#include "ui/GUISystem.h"
#include "prefab/PrefabManager.h"
#include "BehaviourManager.h"
#include <memory>

struct SDL_Renderer;

namespace Core {
	class Engine {
	public:
		Engine();
		~Engine();

		/* Initializes the engine and all systems in preparation of start */
		int initiate(bool skipGLFW = false);
		/* Starts the game loop */
		int start();
		/* Runs one tick of the game. Handles input, updates systems and renders */
		void tick(float deltaTime);
		/* Runs one tick of the game in editor mode */
		void editorTick(float deltaTime);
		/* Stops the game loop and terminates glfw */
		void terminate();

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
		PrefabManager& getPrefabManager();
		BehaviourManager& getBehaviourManager();
		//GUISystem& getGUISystem();
	private:
		// Game loop
		bool running = false;

		// Main Systems
		EntityManager entityManager;
		EventSystem eventSystem;
		Graphics graphics;
		Input input;
		Physics physics;
		GUISystem guiSystem;

		PrefabManager prefabManager;
		BehaviourManager behaviourManager;
	};
}
#endif