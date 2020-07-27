#ifndef ENGINE_H
#define ENGINE_H

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
	class Engine {
	public:
		Engine();
		~Engine();

		int initiate(bool skipCallbacks = false);
		/* Starts the game loop */
		int start();
		/* Runs one tick of the game. Handles input, updates systems and renders */
		void tick(float deltaTime);
		void terminate();

		void resizeViewport(unsigned int width, unsigned int height);

		// Systems
		Input& getInput();
		Graphics& getGraphics();
		Physics& getPhysics();
		EntityManager& getEntityManager();
		SceneManager& getSceneManager();
		BehaviourManager& getBehaviourManager();
		GUISystem& getGUISystem();

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