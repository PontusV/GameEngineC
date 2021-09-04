#ifndef GAME_VIEW_H
#define GAME_VIEW_H
#include "Viewport.h"
#include "GridRenderer.h"
#include "imgui/imgui.h"

typedef std::size_t EntityID;

namespace Editor {
	struct EntityTargetData {
		EntityID entityID;
		EntityID rootEntityID;
		std::string entityName;
	};
	class LevelEditor;
	class UndoRedoManager;

	/* The game view window */
	class GameView {
	public:
		GameView(LevelEditor* editor, UndoRedoManager* undoRedoManager);
		~GameView();

		void initialize(ImVec2 viewportSize);

		/* Renders imGui window and updates the game */
		void tick(float deltaTime, bool editMode, std::size_t fpsCount);

		EntityTargetData getTarget();
		void setTarget(EntityID entityID);
		void updateTargetData();
		/* Removes target */
		void releaseTarget();

		/* @param value A value of 1.0 equals 100% */
		void setZoom(float value);
		float getZoom() const;
		ImVec2 getViewportSize() const;

	private:
		ImVec2 viewportSize = ImVec2(1.0f, 1.0f);
		float zoom = 1.0f;

		bool pressed = false;
		bool targetPressed = false;
		bool draggingTarget = false;
		ImVec2 targetPressedPosition;
		EntityTargetData target;

		LevelEditor* editor;
		UndoRedoManager* undoRedoManager;
		Viewport viewport;
		Viewport viewportBG;
		GridRenderer grid;
	};
}
#endif