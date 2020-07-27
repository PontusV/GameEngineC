#ifndef EDITOR_H
#define EDITOR_H
#include "engine/Engine.h"
#include "input/KeyListener.h"
#include "windows/GameView.h"
#include "windows/Inspector.h"
#include <string>

namespace Editor {
	class LevelEditor : public Core::KeyListener {
	public:
		LevelEditor();
		~LevelEditor();
		int initiate();
		/* Start the engine and rendering of the editor imgui */
		int start();

		void keyPressed(std::string buttonName);
		void keyReleased(std::string buttonName);
	private:
		Core::Engine engine;
		GameView gameView;
		Inspector inspector;
		bool running = false;
	};
}
#endif