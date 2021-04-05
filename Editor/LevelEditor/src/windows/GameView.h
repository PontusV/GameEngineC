#ifndef GAME_VIEW_H
#define GAME_VIEW_H
#include "Viewport.h"
#include "GridRenderer.h"
#include "imgui/imgui.h"

typedef std::size_t EntityID;

namespace Editor {
	struct EntityTargetData {
		EntityID entityID;
		std::size_t sceneIndex;
		std::string entityName;
	};
	class LevelEditor;

	/* The game view window */
	class GameView {
	public:
		GameView(LevelEditor* engineDLL);
		~GameView();

		void initialize(ImVec2 viewportSize);

		/* Renders imGui window and updates the game */
		void tick(float deltaTime);

		EntityTargetData getTarget();
		void setTarget(EntityID entityID);
		void updateTargetData();
		/* Releases allocated memory by Engine DLL and assigns the pointer of target a value of nullptr */
		void releaseTarget();

		ImVec2 getViewportSize() const;

	private:
		ImVec2 viewportSize = ImVec2(1.0f, 1.0f);

		bool pressed = false;
		bool targetPressed = false;
		EntityTargetData target;

		LevelEditor* editor;
		Viewport viewport;
		GridRenderer grid;
	};
}
#endif