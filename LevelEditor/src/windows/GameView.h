#ifndef GAME_VIEW_H
#define GAME_VIEW_H
#include <Core/EntityHandle.h>
#include "Viewport.h"
#include "GridRenderer.h"
#include "imgui/imgui.h"

namespace Core {
	class IEngine;
}

namespace Editor {

	/* The game view window */
	class GameView {
	public:
		GameView(Core::IEngine* engine);
		~GameView();

		void initialize();

		/* Renders imGui window and updates the game */
		void tick(float deltaTime);

		Core::IEntityHandle* getTarget();
		void setTarget(Core::IEntityHandle* handle);

		ImVec2 getViewportSize() const;

	private:
		ImVec2 viewportSize = ImVec2(1.0f, 1.0f);

		bool pressed = false;
		bool targetPressed = false;
		Core::IEntityHandle* target;

		Core::IEngine* engine;
		Viewport viewport;
		GridRenderer grid;
	};
}
#endif