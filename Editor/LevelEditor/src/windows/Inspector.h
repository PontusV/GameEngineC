#ifndef INSPECTOR_H
#define INSPECTOR_H
#include <Core/Entity.h>
#include <string>

namespace Editor {

	class GameView;
	class EngineDLL;

	/* The game view window */
	class Inspector {
	public:
		Inspector(EngineDLL* engineDLL, GameView* gameView);
		~Inspector();

		/* Renders ImGui window */
		void tick();

	private:
		GameView* gameView;
		EngineDLL* engineDLL;

		Core::Entity prevTarget = Core::Entity(0);
		bool renameActive = false;
		std::string renameValue;
	};
}
#endif