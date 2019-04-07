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

		// Game loop
		int			initiate();
		int			start();
		void		terminate();

		// Level
		LevelPtr	createLevel();
		void		setCurrentLevel(LevelPtr level);
		LevelPtr	getCurrentLevel();
		LevelPtr	getDebugLevel();
		/* Saves current level to file */
		void		saveLevel(const char* fileName);
		/* Loads level from file and returns new level */
		LevelPtr	loadLevel(const char* fileName);

		// Systems
		Input&		getInput();
		Graphics&	getGraphics();

	private:
		// Game loop
		bool		running;

		// Level
		LevelPtr	currentLevel;
		LevelPtr	debugLevel;		// Where you put entities showing debugging data. These will not be loaded/saved

		// Main Systems
		Graphics*	graphics;
		Input*		input;
		Physics*	physics;
	};
}
#endif