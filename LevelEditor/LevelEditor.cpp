#include "LevelEditor.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "utils/file.h"
#include "utils/string.h"

#include <fstream>
#include <iostream>

#include <GLFW/glfw3.h>
#include <Windows.h>

using namespace Core;
using namespace Editor;

constexpr wchar_t SCENE_FILE_TYPE[] = L".scene";

LevelEditor::LevelEditor() : gameView(&engine), inspector(&gameView), hierarchy(this, &engine.getSceneManager(), &gameView), fileView(this) {
	engine.getInput().addKeyListener(this);
}


LevelEditor::~LevelEditor() {
}


int LevelEditor::initiate() {
	// Load settings
	if (!editorSettings.load()) {
		std::cout << "Failed to load Editor settings" << std::endl;
	}

	// Initiate engine
	int result = engine.initiate(true);
	if (result != 0) {
		return result;
	}
	Window& window = engine.getGraphics().getWindow();
	window.setBackgroundColor(Vector3(0.15f, 0.15f, 0.15f));

	// Keybinds
	Input& input = engine.getInput();
	input.addAction("Terminate", GLFW_KEY_ESCAPE);
	input.addAction("Save", GLFW_KEY_F5);
	input.addAction("Load", GLFW_KEY_F9);

	input.addAction("Camera_move_left", GLFW_KEY_LEFT);
	input.addAction("Camera_move_up", GLFW_KEY_UP);
	input.addAction("Camera_move_right", GLFW_KEY_RIGHT);
	input.addAction("Camera_move_down", GLFW_KEY_DOWN);

	// Window hints
	glfwWindowHint(GLFW_DECORATED, GL_TRUE);

	// Loading recent project
	std::wstring recentProjectPath = editorSettings.getRecentProjectPaths()[0];
	openProject(recentProjectPath);
	
	// Start editor loop
	return start();
}

void LevelEditor::keyPressed(std::string actionName) {
	if (actionName == "Save") {
		//engine.saveLevel("level.dat");
	}
	if (actionName == "Load") {
		//LevelPtr loadedLevel = engine.loadLevel("level.dat");
		//engine.setCurrentLevel(loadedLevel);
	}
	else if (actionName == "Camera_move_up") {
		engine.getGraphics().getCamera().moveY(-100);
	}
	else if (actionName == "Camera_move_right") {
		engine.getGraphics().getCamera().moveX(100);
	}
	else if (actionName == "Camera_move_down") {
		engine.getGraphics().getCamera().moveY(100);
	}
}

void LevelEditor::keyReleased(std::string buttonName) {
	if (buttonName == "Terminate")
		engine.terminate();
}

int LevelEditor::start() {
	Window& window = engine.getGraphics().getWindow();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Docking

	// Global imGui io settings
	io.ConfigWindowsMoveFromTitleBarOnly = true; // Temp?

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 150");

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = (GLfloat)glfwGetTime();
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Window initialization
	gameView.initialize();

	// Game loop
	running = true;
	while (running && window.isActive()) {
		// Calculate delta time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Poll events
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// imGui Windows
		//ImGuiStyle::ScaleAllSizes(2.0f);
		Vector2 windowSize = window.getResolution();
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y), ImGuiCond_Always);
		ImGui::Begin("Editor", &running, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGuiID dockspaceID = ImGui::GetID("MainDockSpace");
		ImGui::DockSpace(dockspaceID, ImVec2(0, 0));

		// Note: OpenPopup does not work inside BeginMenuBar
		bool open_create_entity_popup = false;
		bool open_create_project_popup = false;
		bool open_create_scene_popup = false;

		std::vector<ScenePtr> loadedScenes = engine.getSceneManager().getAllScenes();

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("New")) {
					if (ImGui::MenuItem("New Project")) {
						open_create_project_popup = true;
					}
					if (ImGui::MenuItem("New Scene", nullptr, nullptr, projectSettings.isLoaded())) {
						open_create_scene_popup = true;
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Open")) {
					if (ImGui::MenuItem("Open Project")) {
						std::wstring filePath = getOpenFileName(L"Select A File", L"All Project Files\0*.proj;\0", 1);

						if (!filePath.empty()) {
							openProject(filePath);
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Save Project", "Ctrl + S", nullptr, projectSettings.isLoaded())) {
					projectSettings.save();
				}
				if (ImGui::MenuItem("Save Scene", "Ctrl + S", nullptr, projectSettings.isLoaded() && loadedScenes.size() > 0)) {
					// Saving all loaded scenes
					for (ScenePtr scene : loadedScenes) {
						engine.getSceneManager().saveScene(scene);
					}
					projectSettings.save();
				}
				if (ImGui::MenuItem("Build", nullptr, nullptr, projectSettings.isLoaded())) {
					// TODO: Build game
				}
				if (ImGui::MenuItem("Exit", "Alt + F4")) {
					terminate();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Game Object"))
			{
				if (ImGui::MenuItem("Create new")) {
					open_create_entity_popup = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		static std::wstring dirPath = L"";
		static bool chooseDirPath = false;

		if (open_create_scene_popup) {
			std::wstring path = getSaveFileName(L"Choose scene location", L"Scene\0*.scene;\0", 1, projectSettings.getPath().c_str());
			if (!path.empty()) {
				if (!path.ends_with(SCENE_FILE_TYPE)) {
					path.append(SCENE_FILE_TYPE);
				}
				std::size_t nameStartIndex = path.find_last_of(L"\\");
				std::wstring fileName = path.substr(nameStartIndex == std::wstring::npos ? 0 : nameStartIndex + 1);
				ScenePtr scene = engine.getSceneManager().createScene(fileName.substr(0, fileName.find_last_of(L".")));
				engine.getSceneManager().saveScene(scene, path.c_str());
				scene->awake();
				projectSettings.save();
			}
		}

		if (open_create_project_popup) {
			ImGui::OpenPopup("create_project_popup");
			chooseDirPath = true;
		}

		if (ImGui::BeginPopupModal("create_project_popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			static std::string errorMessage = "";
			if (!errorMessage.empty()) {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), errorMessage.c_str());
			}
			static char buffer[64] = "";
			if (ImGui::Button("Choose directory")) {
				chooseDirPath = true;
			}
			ImGui::Text(utf8_encode(dirPath).append("\\").append(buffer).c_str());
			ImGui::InputText("Project name", buffer, 64);

			if (ImGui::Button("Create", ImVec2(120, 0))) {
				std::wstring name = utf8_decode(buffer);
				std::wstring path = dirPath;

				ProjectSettings newProject = ProjectSettings::create(name.c_str(), path.c_str());
				if (!newProject.getPath().empty()) {
					projectSettings = newProject;
					fileView.setSourcePath(path);
					errorMessage = "";
					dirPath = L"";
					memset(buffer, 0, 64);
					ImGui::CloseCurrentPopup();

					editorSettings.pushRecentProjectPath(newProject.getFilePath());
				}
				else {
					errorMessage = "Failed to create new project!";
				}
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				errorMessage = "";
				dirPath = L"";
				ImGui::CloseCurrentPopup();
			}
			if (chooseDirPath) {
				dirPath = getOpenFolderName(L"Choose project location");
				chooseDirPath = false;
				if (dirPath.empty()) {
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}

		if (open_create_entity_popup)
			ImGui::OpenPopup("create_entity_popup");

		if (ImGui::BeginPopupModal("create_entity_popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			static std::string errorMessage = "";
			if (!errorMessage.empty()) {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), errorMessage.c_str());
			}
			static char buffer[64] = "";
			ImGui::InputText("Name", buffer, 64);

			if (ImGui::Button("Create", ImVec2(120, 0))) {
				std::string name = std::string(buffer);
				if (engine.getEntityManager().isEntityNameAvailable(name)) {
					Core::Scene* scene = gameView.getTarget().getScene();
					if (scene == nullptr) {
						std::vector<ScenePtr> scenes = engine.getSceneManager().getAllScenes();
						if (!scenes.empty()) {
							scene = scenes[0].get();
						}
					}
					if (scene) {
						Core::Vector2 pos = engine.getGraphics().getCamera().getPosition();
						ImVec2 viewportSize = gameView.getViewportSize();
						EntityHandle handle = scene->createEntity(name,
							Image("resources/images/awesomeface.png"),
							RectTransform(pos.x + viewportSize.x/2, pos.y + viewportSize.y / 2, 350, 350, 0.0f, Alignment::CENTER),
							SpriteRenderer()
						);
					}
					errorMessage = "";
					memset(buffer, 0, 64);
					ImGui::CloseCurrentPopup();
				}
				else {
					errorMessage = "Name is already taken!";
				}
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				errorMessage = "";
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::End();

		// Inspector window
		ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);
		inspector.tick();
		// Game view window
		gameView.tick(deltaTime);
		EntityHandle target = gameView.getTarget();
		hierarchy.tick(target);
		// File view window
		fileView.tick();

		// TODO: Hide after editor is finished
		ImGui::ShowDemoWindow();

		ImGui::Render();
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();

		// 1ms sleep
		Sleep(1);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void LevelEditor::terminate() {
	// TODO: Check for unsaved changes
	running = false;
}

void LevelEditor::openProject(std::wstring path) {
	std::size_t seperator = path.find_last_of(L"\\");
	std::wstring fileName = path.substr(seperator + 1);
	std::wstring dirPath = path.substr(0, seperator);
	projectSettings = ProjectSettings::load(fileName.substr(0, fileName.find_last_of(L".")).c_str(), dirPath.c_str());
	editorSettings.pushRecentProjectPath(path);
	fileView.setSourcePath(dirPath);
}

void LevelEditor::openScene(std::wstring path) {
	engine.getSceneManager().loadScene(path.c_str());
}

void LevelEditor::closeScene(ScenePtr scene) {
	engine.getSceneManager().unloadScene(scene->getName());
	EntityHandle& target = gameView.getTarget();
	if (target.getScene() == scene.get()) {
		target.clear();
	}
}