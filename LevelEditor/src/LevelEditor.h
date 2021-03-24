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

		std::wstring getProjectName();

		void openProject(std::wstring path);
		void openScene(std::wstring path);
		void closeScene(Core::IScene* scene);
	private:
		EditorSettings editorSettings;
		ProjectSettings projectSettings;

		Core::IEngine* engine;

		GameView gameView;
		Inspector inspector;
		Hierarchy hierarchy;
		FileView fileView;
		bool running = false;
	};
}
#endif