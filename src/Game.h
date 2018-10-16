#ifndef GAME_H
#define GAME_H
#include "KeyListener.h"

class Engine;

//Will derive from Engine when it is ready
class Game :
	public KeyListener
{
public:
	Game(Engine* engine);
	~Game();

	void initiate();

	void keyPressed(std::string buttonName);
	void keyReleased(std::string buttonName);
private:
	Engine* engine;
};
#endif