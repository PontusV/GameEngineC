#include "Engine.h"
#include "SDL.h"
#include "Level.h"
#include "Entity.h"
#include "Image.h"
#include <iostream>
#include "Game.h"
#define _CRTDBG_MAP_ALLOC

//For creating components

int main(int argc, char* args[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //Lets static variables be destroyed before looking for memory leaks

	Engine engine;
	Game game(&engine);
	//Start game engine
	engine.initiate();

	return 0;
}