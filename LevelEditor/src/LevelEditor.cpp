#include "LevelEditor.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "ResourceManager.h"
#include "utils/file.h"
#include "utils/string.h"

#include <Core/Core.h>
#include <fstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

using namespace Core;
using namespace Editor;

constexpr wchar_t SCENE_FILE_TYPE[] = L".scene";
ImVec2 resolutionDefault = ImVec2(600, 480);

LevelEditor::LevelEditor() : engine(nullptr), gameView(this), inspector(&engineDLL, &gameView), hierarchy(this, &gameView), fileView(this) {
}


LevelEditor::~LevelEditor() {
	unloadEngine();
}

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Editor Glfw Error %d: %s\n", error, description);
}

Matrix4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
	Matrix4 result;
	result.data[0] = 2 / (right - left);
	result.data[5] = 2 / (top - bottom);
	result.data[10] = -2 / (zFar - zNear);
	result.data[12] = -(right + left) / (right - left);
	result.data[13] = -(top + bottom) / (top - bottom);
	result.data[14] = -(zFar + zNear) / (zFar - zNear);
	return result;
}


int LevelEditor::initiate() {
	glfwSetErrorCallback(glfw_error_callback);
	// Load settings
	if (!editorSettings.load()) {
		std::cout << "Failed to load Editor settings" << std::endl;
	}

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW!" << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//glfwWindowHint(GLFW_SAMPLES, 16); // Anti-alias (Makes Sprites blurry)

	GLFWwindow* glfwWindow = glfwCreateWindow(resolutionDefault.x, resolutionDefault.y, "Editor", NULL, NULL);
	if (!glfwWindow) {
		glfwTerminate();
		std::cout << "Failed to create GLFW window!" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(glfwWindow);
	glfwSwapInterval(0); //0 = DISABLES VSYNC, 1 = ENABLES VSYNC

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	Matrix4 projectionMatrix = ortho(0.0f, static_cast<GLfloat>(resolutionDefault.x), static_cast<GLfloat>(resolutionDefault.y), 0.0f, -1.0f, 1.0f);
	ResourceManager::getInstance().initShader(projectionMatrix);

	// OpenGL configuration
	glViewport(0, 0, (int)resolutionDefault.x, (int)resolutionDefault.y);
	// Enable transparently
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Stencil (Test)
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glStencilMask(0x00); // Disable stencil writes
	// End of config

	// Window hints
	glfwWindowHint(GLFW_DECORATED, GL_TRUE);

	// Loading recent project
	std::wstring recentProjectPath = editorSettings.getRecentProjectPaths()[0];
	openProject(recentProjectPath);
	
	// Start editor loop
	return start();
}

int LevelEditor::start() {
	GLFWwindow* window = glfwGetCurrentContext();
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
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = (GLfloat)glfwGetTime();
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Window initialization
	gameView.initialize(ImVec2(resolutionDefault.x, resolutionDefault.y));

	// Game loop
	running = true;

	while (running && !glfwWindowShouldClose(window)) {
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
		//ImGuiStyle::ScaleAllSizes(2.0f);int width = 0;
		int width = 0;
		int height = 0;
		glfwGetWindowSize(window, &width, &height);
		ImVec2 windowSize = ImVec2(width, height);
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y), ImGuiCond_Always);
		ImGui::Begin("Editor", &running, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGuiID dockspaceID = ImGui::GetID("MainDockSpace");
		ImGui::DockSpace(dockspaceID, ImVec2(0, 0));

		// Note: OpenPopup does not work inside BeginMenuBar
		bool open_create_entity_popup = false;
		bool open_create_project_popup = false;
		bool open_create_scene_popup = false;

		if (ImGui::IsKeyPressed(GLFW_KEY_ESCAPE)) { // TODO: Keybind
			//engine->terminate();
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_S) && ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) { // TODO: Keybind
			/*
			for (IScene* scene : loadedScenes) {
				engine->getSceneManagerInterface()->saveScene(scene);
			}
			projectSettings.save();
			*/
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("New")) {
					if (ImGui::MenuItem("New Project")) {
						open_create_project_popup = true;
					}
					if (ImGui::MenuItem("New Scene", nullptr, nullptr, engine && projectSettings.isLoaded())) {
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
				if (ImGui::MenuItem("Save Project", "Ctrl+S", nullptr, projectSettings.isLoaded())) { // TODO: Keybind
					projectSettings.save();
				}
				auto loadedScenes = getLoadedScenes();
				if (ImGui::MenuItem("Save Scene", "Ctrl+S", nullptr, engine && projectSettings.isLoaded() && loadedScenes.size() > 0)) { // TODO: Keybind
					// Saving all loaded scenes
					for (IScene* scene : loadedScenes) {
						if (engine) {
							engine->getSceneManagerInterface()->saveScene(scene);
						}
					}
					projectSettings.save();
				}
				if (ImGui::MenuItem("Build", nullptr, nullptr, projectSettings.isLoaded())) {
					buildGame();
				}
				if (ImGui::MenuItem("Exit", "Alt+F4")) {
					terminate();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {} // TODO
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {
					fileView.cut();
				}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {
					fileView.copy();
				}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {
					fileView.paste();
				}
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

		if (open_create_scene_popup && engine) {
			std::wstring path = getSaveFileName(L"Choose scene location", L"Scene\0*.scene;\0", 1, projectSettings.getPath().c_str());
			if (!path.empty()) {
				if (!path.ends_with(SCENE_FILE_TYPE)) {
					path.append(SCENE_FILE_TYPE);
				}
				std::size_t nameStartIndex = path.find_last_of(L"\\");
				std::wstring fileName = path.substr(nameStartIndex == std::wstring::npos ? 0 : nameStartIndex + 1);
				IScene* scene = engine->getSceneManagerInterface()->createIScene(fileName.substr(0, fileName.find_last_of(L".")).c_str());
				engine->getSceneManagerInterface()->saveScene(scene, path.c_str());
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
			if (engine) {
				static std::string errorMessage = "";
				if (!errorMessage.empty()) {
					ImGui::TextColored(ImVec4(1, 0, 0, 1), errorMessage.c_str());
				}
				static char buffer[64] = "";
				ImGui::InputText("Name", buffer, 64);

				if (ImGui::Button("Create", ImVec2(120, 0))) {
					std::string name = std::string(buffer);
					if (engine->getEntityManagerInterface()->isEntityNameAvailable(name.c_str())) {
						Core::IScene* scene = gameView.getTarget()->getIScene();
						if (scene == nullptr) {
							std::vector<IScene*> scenes = getLoadedScenes();
							if (!scenes.empty()) {
								scene = scenes[0];
							}
						}
						else {
							Core::ICamera* camera = engine->getGraphicsInterface()->getCameraInterface();
							ImVec2 pos = ImVec2(camera->getX(), camera->getY());
							ImVec2 viewportSize = gameView.getViewportSize();
							if (auto ptr = engineDLL.getInterface()) {
								ptr->createTemplateEntity(scene, name.c_str(), pos.x + viewportSize.x / 2, pos.y + viewportSize.y / 2);
							}
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
			}
			else {
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
		IEntityHandle* target = gameView.getTarget();
		hierarchy.tick(target);
		// File view window
		fileView.tick();

		// TODO: Hide after editor is finished
		ImGui::ShowDemoWindow();

		ImGui::Render();
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);

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
	if (engineDLL.isLoaded()) {
		engineDLL.unload();
	}
	std::size_t seperator = path.find_last_of(L"\\");
	std::wstring fileName = path.substr(seperator + 1);
	std::wstring dirPath = path.substr(0, seperator);
	projectSettings = ProjectSettings::load(fileName.substr(0, fileName.find_last_of(L".")).c_str(), dirPath.c_str());
	editorSettings.pushRecentProjectPath(path);
	fileView.setSourcePath(dirPath);
	loadEngine();
}

void LevelEditor::openScene(std::wstring path) {
	if (!engine) {
		std::cout << "Unable to open scene because the Engine has not been loaded" << std::endl;
		return;
	}
	engine->getSceneManagerInterface()->loadIScene(path.c_str());
}

void LevelEditor::closeScene(IScene* scene) {
	if (!engine) {
		std::cout << "Unable to close scene because the Engine has not been loaded" << std::endl;
		return;
	}
	engine->getSceneManagerInterface()->unloadScene(scene->getName());
	IEntityHandle* target = gameView.getTarget();
	if (target->getIScene() == scene) {
		target->clear();
	}
}

std::wstring LevelEditor::getProjectName() {
	return projectSettings.getName();
}

std::vector<IScene*> LevelEditor::getLoadedScenes() {
	if (!engine) return {};
	std::size_t sceneCount = engine->getSceneManagerInterface()->getAllScenesCount();
	std::vector<IScene*> loadedScenes(sceneCount);
	engine->getSceneManagerInterface()->getAllIScenes(&loadedScenes[0], sceneCount);
	return loadedScenes;
}

IEngine* LevelEditor::getEngine() {
	return engine;
}

EngineDLL* LevelEditor::getEngineDLL() {
	return &engineDLL;
}

bool LevelEditor::loadEngine() {
	if (engineDLL.isLoaded()) {
		std::cout << "Unable to load engine DLL because one is already loaded" << std::endl;
		return false;
	}
	if (engineDLL.load(L"G:/Projects/ProjectGE/x64/Release/Game.dll")) { // TODO: Get DLL path from project
		if (auto ptr = engineDLL.getInterface()) {
			engine = ptr->createEngine();

			// Initialize egine
			IWindow* window = engine->getGraphicsInterface()->getWindowInterface();
			window->setActive(true);
			window->setResolution(resolutionDefault.x, resolutionDefault.y);
			window->setBackgroundColor(0.15f, 0.15f, 0.15f);
			window->initGLAD((GLADloadproc)glfwGetProcAddress);

			int result = engine->initiate(true);
			if (result != 0) { // Failed to initiate the engine
				std::cout << "Failed to initiate the Engine. Unloading the Engine now..." << std::endl;
				unloadEngine();
				return false;
			}

			hierarchy.initiate(); // Initiate hierarchy view for new Engine DLL
			return true;
		}
	}
	return false;
}

bool LevelEditor::unloadEngine() {
	hierarchy.clear();
	gameView.releaseTarget();
	if (engine) {
		engine->release();
		engine = nullptr;
	}
	engineDLL.unload();
	return true;
}

bool LevelEditor::buildGame() {
	std::cout << "Build game is currently a WIP" << std::endl;
	return false;
}