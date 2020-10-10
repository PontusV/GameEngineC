#ifndef EDITOR_H
#define EDITOR_H
#include "EditorSettings.h"
#include "ProjectSettings.h"
#include "engine/Engine.h"
#include "input/KeyListener.h"
#include "windows/GameView.h"
#include "windows/Inspector.h"
#include "windows/Hierarchy.h"
#include "windows/FileView.h"
#include "scene/SceneManager.h"
#include <string>

namespace Editor {
	class LevelEditor : public Core::KeyListener {
	public:
		LevelEditor();
		~LevelEditor();
		int initiate();
		/* Start the engine and rendering of the editor imgui */
		int start();
		void terminate();

		void keyPressed(std::string buttonName);
		void keyReleased(std::string buttonName);

		std::wstring getProjectName();

		void openProject(std::wstring path);
		void openScene(std::wstring path);
		void closeScene(Core::ScenePtr scene);
	private:
		EditorSettings editorSettings;
		ProjectSettings projectSettings;
		Core::Engine engine;
		GameView gameView;
		Inspector inspector;
		Hierarchy hierarchy;
		FileView fileView;
		bool running = false;
	};
}
#endif