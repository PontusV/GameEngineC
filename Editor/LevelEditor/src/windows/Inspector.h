#ifndef INSPECTOR_H
#define INSPECTOR_H
#include <string>

typedef std::size_t EntityID;

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

		EntityID prevTarget = 0;
		bool renameActive = false;
		std::string renameValue;
	};
}
#endif