#ifndef ENGINE_H
#define ENGINE_H

#include "EntityManager.h"
#include "SceneManager.h"
#include "Graphics.h"
#include "Physics.h"
#include "Input.h"
#include "BehaviourManager.h"
#include <memory>

struct SDL_Renderer;

namespace Core {
	class Engine {
	public:
		Engine();
		~Engine();

		// Game loop
		int initiate();
		int start();
		void terminate();

		// Systems
		EntityManager& getEntityManager();
		SceneManager& getSceneManager();
		Input& getInput();
		Graphics& getGraphics();
		Physics& getPhysics();
		BehaviourManager& getBehaviourManager();

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

		BehaviourManager behaviourManager;
	};
}
#endif