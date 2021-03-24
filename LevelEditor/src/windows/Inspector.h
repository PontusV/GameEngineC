#ifndef INSPECTOR_H
#define INSPECTOR_H
#include <Core/IEntity.h>
#include <string>

namespace Editor {

	class GameView;

	/* The game view window */
	class Inspector {
	public:
		Inspector(GameView* gameView);
		~Inspector();

		/* Renders ImGui window */
		void tick();

	private:
		GameView* gameView;
		Core::Entity prevTarget = Core::Entity(0);
		bool renameActive = false;
		std::string renameValue;
	};
}
#endif