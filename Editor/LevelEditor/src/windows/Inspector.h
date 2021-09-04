#ifndef INSPECTOR_H
#define INSPECTOR_H
#include <string>
#include "imgui/imgui.h"

typedef std::size_t EntityID;

namespace Editor {

	class GameView;
	class ProjectSettings;
	class UndoRedoManager;
	class PopupManager;
	class EngineDLL;

	/* The game view window */
	class Inspector {
	public:
		Inspector(EngineDLL* engineDLL, GameView* gameView, UndoRedoManager* undoRedoManager, PopupManager* popupManager);
		~Inspector();

		/* Renders ImGui window */
		void tick();
	
	private:
		void renderComponent(EntityID rootEntityID, EntityID entityID, std::string entityName, std::size_t typeID, void* component, std::size_t index);

	private:
		GameView* gameView;
		EngineDLL* engineDLL;
		UndoRedoManager* undoRedoManager;
		PopupManager* popupManager;

		EntityID prevTarget = 0;
		bool renameActive = false;
		std::string renameValue;

		ImVec4 colorEditOldValue;
	};
}
#endif