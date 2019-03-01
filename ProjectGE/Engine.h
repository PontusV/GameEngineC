#ifndef ENGINE_H
#define ENGINE_H

#include "Graphics.h"
#include "Physics.h"
#include "Input.h"
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct SDL_Renderer;

namespace GameEngine {
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
		//Screen size
		const GLuint SCREEN_WIDTH = 800;
		const GLuint SCREEN_HEIGHT = 600;
		//
		LevelPtr currentLevel;
		bool running;

		//Main Systems
		Input input;
		Graphics graphics;
		Physics physics;
	};
}
#endif