#ifndef EDITOR_H
#define EDITOR_H
#include "EditorSettings.h"
#include "ProjectSettings.h"
#include <Core/Engine.h>
#include <Core/SceneManager.h>
#include <Core/Scene.h>
#include "windows/GameView.h"
#include "windows/Inspector.h"
#include "windows/Hierarchy.h"
#include "windows/FileView.h"
#include "EngineDLL.h"
#include <string>

namespace Editor {
	class LevelEditor {
	public:
		LevelEditor();
		~LevelEditor();
		int initiate();
		/* Start the engine and rendering of the editor imgui */
		int start();
		void terminate();

		/* Returns the name of the currently opened project */
		std::wstring getProjectName();

		/* Opens the project at the specified path. If a project is already opened it will be closed and the Engine will be unloaded */
		void openProject(std::wstring path);
		void openScene(std::wstring path);
		void closeScene(Core::IScene* scene);

		/* Loads and initializes the Engine DLL of the current project. Returns true if successful*/
		bool loadEngine();
		/* Unloads the currently loaded Engine DLL of the current project. Returns true if successful */
		bool unloadEngine();
		/* Builds a new Game EXE from the current project. Returns true if successful */
		bool buildGame();

		std::vector<Core::IScene*> getLoadedScenes();
		Core::IEngine* getEngine();
		EngineDLL* getEngineDLL();
	private:
		EditorSettings editorSettings;
		ProjectSettings projectSettings;

		EngineDLL engineDLL;
		Core::IEngine* engine;

		GameView gameView;
		Inspector inspector;
		Hierarchy hierarchy;
		FileView fileView;
		bool running = false;
	};
}
#endif