#ifndef GAME_H
#define GAME_H
#include "KeyListener.h"

namespace GameEngine {
	class Engine;
}

//Will derive from Engine when it is ready
class Game :
	public GameEngine::KeyListener
{
public:
	Game(GameEngine::Engine* engine);
	~Game();

	void initiate();

	void keyPressed(std::string buttonName);
	void keyReleased(std::string buttonName);
private:
	GameEngine::Engine* engine;
};
#endif