#include "SDL.h"
#include "SDL_ttf.h"
#include "Engine.h"
#include "Level.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

/* Saves current level. */
void Engine::saveLevel() { //To be added: file location of level map (currently hard coded to the map Levels)
	/*ofstream file;
	file.open("Levels/level.dat", ios::out | ios::binary | ios::trunc);
	handleManager.write(file);
	currentLevel->serialize(file);
	graphics.write(file);
	file.close();*/

}

/* Loads level from file. */
void Engine::loadLevel() { //To be added: file location of level map (currently hard coded to the map Levels)
	/*if (currentLevel != nullptr) //Delete currentLevel if a level is already loaded
		delete currentLevel;
	handleManager.reset();

	ifstream file;
	file.open("Levels/level.dat", ios::in | ios::binary);
	//
	Level* level = new Level(handleManager);
	handleManager.read(file);
	level->deserialize(file);
	graphics.read(file, &handleManager);
	//
	file.close();

	//Change level
	currentLevel = level;*/
}

Level* Engine::createLevel() {
	Level* level = new Level(handleManager);
	currentLevel = level;
	return level;
}

/* Stops the gameloop. */
void Engine::terminate() {
	running = false;
}

void Engine::initiate() {
	running = true;
	graphics.createWindow();

	uint32_t setFPS = 60;
	float timePerFrame = (float)1000/setFPS;
	uint32_t fpsCounter = 0;
	//Gameloop
	auto lastTime = std::chrono::high_resolution_clock::now();
	while (running) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = float(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count());
		lastTime = currentTime;

		//Look for input events from SDL
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			input.processInputEvent(event);
		}
		graphics.update(deltaTime);

		int sleepTime = int(timePerFrame - deltaTime);
		if (sleepTime > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
	}
}

Engine::Engine() : input(this) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
}


Engine::~Engine() {
	delete currentLevel;

	TTF_Quit();
	SDL_Quit();
}

HandleManager& Engine::getHandleManager() {
	return handleManager;
}

Input& Engine::getInput() {
	return input;
}

Level* Engine::getCurrentLevel() {
	return currentLevel;
}