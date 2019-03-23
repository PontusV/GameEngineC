#ifndef EDITOR_H
#define EDITOR_H
#include "Engine.h"
#include "KeyListener.h"

namespace LevelEditor {
	class Editor :
		public Core::KeyListener
	{
	public:
		Editor();
		~Editor();
		void keyPressed(std::string buttonName);
		void keyReleased(std::string buttonName);
	private:
		void initiate();
		Core::Engine engine;
	};
}
#endif