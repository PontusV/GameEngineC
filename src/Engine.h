#ifndef ENGINE_H
#define ENGINE_H

#include "Graphics.h"
#include "Physics.h"
#include "Input.h"
#include "HandleManager.h"

class Level;
struct SDL_Renderer;

class Engine {
public:
	Engine();
	~Engine();
	void initiate();
	void terminate();

	//Level
	Level* createLevel();
	void saveLevel();
	void loadLevel();
	Level* getCurrentLevel();

	//Return system for testing in main.cpp (Should be removed later)
	HandleManager& getHandleManager();
	Input& getInput();
private:
	Level* currentLevel;
	bool running;

	//Other
	HandleManager handleManager;
	//Main Systems
	Input input;
	Graphics graphics = Graphics(handleManager);
	Physics physics;
};
#endif