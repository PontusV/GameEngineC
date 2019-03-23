#ifndef ENGINE_H
#define ENGINE_H

#include "Graphics.h"
#include "Physics.h"
#include "Input.h"
#include <memory>

struct SDL_Renderer;

namespace Core {
	class Level;

	typedef std::shared_ptr<Level> LevelPtr;

	class Engine {
	public:
		Engine();
		~Engine();
		int initiate();
		void terminate();

		//Level
		LevelPtr createLevel();
		void saveLevel();
		void loadLevel();
		LevelPtr getCurrentLevel();

		//Return system for testing in main.cpp (Should be removed later)
		Input& getInput();
		Graphics& getGraphics();
	private:
		//
		LevelPtr currentLevel;
		bool running;

		//Main Systems
		Graphics*	graphics;
		Input*		input;
		Physics*	physics;
	};
}
#endif