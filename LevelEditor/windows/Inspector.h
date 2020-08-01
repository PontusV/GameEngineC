#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "GameView.h"

namespace Editor {

	/* The game view window */
	class Inspector {
	public:
		Inspector(GameView* gameView);
		~Inspector();

		/* Renders ImGui window */
		void tick();

	private:
		GameView* gameView;
	};
}
#endif