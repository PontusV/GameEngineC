#ifndef EDITOR_H
#define EDITOR_H
#include "EditorSettings.h"
#include "ProjectSettings.h"
#include "windows/GameView.h"
#include "windows/Inspector.h"
#include "windows/Hierarchy.h"
#include "windows/FileView.h"
#include "EngineDLL.h"
#include <string>

// Path to the solution to be copied when creating a new project
#define ENGINE_SOLUTION_PATH L"G:/Projects/ProjectGE/Engine"
#define ENGINE_SOLUTION_BUILD_PATH L"/x64/Release"
#define ASSETS_PATH L"G:/Projects/ProjectGE/Assets"
// Defines where in the project directory all neccessary temp files for the Editor goes
#define TEMP_EDITOR_BUILD_PATH L"/.Editor"

struct GLFWwindow;

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

		bool createProject(std::wstring name, std::wstring path);
		/* Opens the project at the specified path. If a project is already opened it will be closed and the Engine will be unloaded */
		void openProject(std::wstring path);
		/* Closes the current project. Unloads Engine / Game.dll. Cleans up temp Editor directory */
		void closeProject();
		/* Returns true if successful */
		bool openScene(std::wstring path);
		void closeScene(std::size_t sceneIndex);

		/* Loads and initializes the Engine DLL of the current project. Returns true if successful*/
		bool loadEngine();
		/* Unloads the currently loaded Engine DLL of the current project. Returns true if successful */
		bool unloadEngine();
		/* Builds a new Game EXE from the current project. Returns true if successful */
		bool buildGame();

		EngineDLL* getEngineDLL();
	private:
		EditorSettings editorSettings;
		ProjectSettings projectSettings;

		EngineDLL engineDLL;

		GameView gameView;
		Inspector inspector;
		Hierarchy hierarchy;
		FileView fileView;

		GLFWwindow* glfwWindow;
		bool running = false;
	};
}
#endif