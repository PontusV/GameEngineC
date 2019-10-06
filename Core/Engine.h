#ifndef ENGINE_H
#define ENGINE_H

#include "EntityManager.h"
#include "Graphics.h"
#include "Physics.h"
#include "Input.h"
#include "BehaviourManager.h"
#include "ObjectData.h"
#include <memory>

struct SDL_Renderer;

namespace Core {
	class Scene;

	typedef std::shared_ptr<Scene> ScenePtr;

	class Engine {
	public:
		Engine();
		~Engine();

		// Game loop
		int initiate();
		int start();
		void terminate();

		/* Creates a new Scene and loads it */
		ScenePtr createScene(std::string name, ObjectType type = ObjectType::World);
		/* Retrieves a Scene by name */
		ScenePtr getScene(std::string name) {} // TODO
		/* Saves scene to file */
		void saveScene(ScenePtr scene, const char* fileName); // Move to Scene
		/* Loads scene from file */
		ScenePtr loadScene(const char* fileName);

		// Systems
		EntityManager& getEntityManager();
		Input& getInput();
		Graphics& getGraphics();
		Physics& getPhysics();
		BehaviourManager& getBehaviourManager();

	private:
		// Game loop
		bool running = false;

		// Scene
		std::vector<ScenePtr> scenes;
		ScenePtr debugScene; // Where you put entities showing debugging data. These will not be loaded/saved

		// Main Systems
		EntityManager entityManager;
		Graphics graphics;
		Input input;
		Physics physics;

		BehaviourManager behaviourManager;
	};
}
#endif