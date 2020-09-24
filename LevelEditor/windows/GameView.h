#ifndef GAME_VIEW_H
#define GAME_VIEW_H
#include "entity/handle/EntityHandle.h"
#include "Viewport.h"
#include "GridRenderer.h"
#include "imgui/imgui.h"

namespace Core {
	class Engine;
}

namespace Editor {

	/* The game view window */
	class GameView {
	public:
		GameView(Core::Engine* engine);
		~GameView();

		void initialize();

		/* Renders imGui window and updates the game */
		void tick(float deltaTime);

		Core::EntityHandle& getTarget();
		void setTarget(Core::EntityHandle handle);

		ImVec2 getViewportSize() const;

	private:
		ImVec2 viewportSize = ImVec2(1.0f, 1.0f);

		bool pressed = false;
		bool targetPressed = false;
		Core::EntityHandle target;

		Core::Engine* engine;
		Viewport viewport;
		GridRenderer grid;
	};
}
#endif