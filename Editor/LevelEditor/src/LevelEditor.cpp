#include "LevelEditor.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "ResourceManager.h"
#include "utils/file.h"
#include "utils/string.h"
#include "IconsFontAwesome5.h"

#include <fstream>
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

using namespace Editor;

constexpr wchar_t SCENE_FILE_TYPE[] = L".scene";
ImVec2 resolutionDefault = ImVec2(600, 480);

LevelEditor::LevelEditor() : undoRedoManager(&engineDLL), gameView(this, &undoRedoManager), inspector(&engineDLL, &gameView, &undoRedoManager), hierarchy(this, &gameView, &undoRedoManager), fileView(this) {
}


LevelEditor::~LevelEditor() {
	if (engineDLL.isLoaded()) {
		unloadEngine();
	}
	closeProject();
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

	glfwWindow = glfwCreateWindow(resolutionDefault.x, resolutionDefault.y, "Editor", NULL, NULL);
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
	// Enable antialias (for grid)
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
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
	if (!recentProjectPath.empty()) {
		openProject(recentProjectPath);
	}
	
	// Start editor loop
	return start();
}

void LevelEditor::updateEngineDLL(float& updateTime) {
	// Check reload Engine DLL
	if (updateTime < 1.0f) return;
	updateTime = 0.0f;
	if (projectSettings.isLoaded()) {
		std::wstring dllPath = projectSettings.getEngineDLLPath();
		std::wstring newDLLPath = projectSettings.getPath() + TEMP_EDITOR_BUILD_PATH + L"/" + engineDLL.getCurrentDLLName();
		if (std::filesystem::exists(dllPath)) {
			if (engineDLL.isLoaded()) {
				auto lastReadTimeRaw = std::filesystem::last_write_time(newDLLPath);
				auto lastWriteTimeRaw = std::filesystem::last_write_time(dllPath);
				auto lastReadTime = std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::system_clock::duration>(lastReadTimeRaw - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()));
				auto lastWriteTime = std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::system_clock::duration>(lastWriteTimeRaw - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()));
				if (lastWriteTime != lastReadTime) {
					std::wstring path = copyEngineDLL();
					if (!path.empty()) {
						reloadEngine(path);
					}
				}
			}
		}
		else {
			// Wait between loading
			Sleep(500);
		}
	}
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

	// Setup FontAwesome
	io.Fonts->AddFontDefault();
	// merge in icons from Font Awesome
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 420");

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = (GLfloat)glfwGetTime();
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Window initialization
	gameView.initialize(ImVec2(resolutionDefault.x, resolutionDefault.y));

	std::size_t currentFPS = 0;
	std::size_t fpsCount = 0;
	GLfloat fpsTimer = 0;

	// Game loop
	running = true;

	GLfloat engineDLLUpdateTime = 0.0f;
	while (running && !glfwWindowShouldClose(window)) {
		// Calculate delta time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Calculate FPS
		fpsTimer += deltaTime;
		fpsCount++;
		if (fpsTimer > 1.0f) {
			fpsTimer -= 1.0f;
			currentFPS = fpsCount;
			fpsCount = 0;
		}

		engineDLLUpdateTime += deltaTime;
		updateEngineDLL(engineDLLUpdateTime);

		// Poll events
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// imGui Windows
		//ImGuiStyle::ScaleAllSizes(2.0f);
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

		if (ImGui::IsKeyPressed(GLFW_KEY_Z) && ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
			undoRedoManager.undo();
		}
		else if (ImGui::IsKeyPressed(GLFW_KEY_Y) && ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
			undoRedoManager.redo();
		}
		else if (ImGui::IsKeyPressed(GLFW_KEY_S) && ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
			// Save all
			std::size_t loadedSceneCount = engineDLL.getSceneCount();
			for (std::size_t i = 0; i < loadedSceneCount; i++) {
				engineDLL.saveScene(i);
			}
			projectSettings.save();
			editorSettings.save();
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::BeginMenu("New")) {
					if (ImGui::MenuItem("New Project")) {
						open_create_project_popup = true;
					}
					if (ImGui::MenuItem("New Scene", nullptr, nullptr, engineDLL.isLoaded() && projectSettings.isLoaded())) {
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
				auto loadedSceneCount = engineDLL.getSceneCount();
				if (ImGui::MenuItem("Save Scene", "Ctrl+S", nullptr, engineDLL.isLoaded() && projectSettings.isLoaded() && loadedSceneCount > 0)) { // TODO: Keybind
					// Saving all loaded scenes
					std::size_t sceneCount = engineDLL.getSceneCount();
					for (std::size_t i = 0; i < sceneCount; i++) {
						engineDLL.saveScene(i);
					}
					projectSettings.save();
				}
				if (ImGui::MenuItem("Reload", nullptr, nullptr, projectSettings.isLoaded() && engineDLL.isLoaded())) {
					std::wstring path = copyEngineDLL();
					if (!path.empty()) {
						reloadEngine(path);
					}
				}
				if (ImGui::MenuItem("Build", nullptr, nullptr, projectSettings.isLoaded())) {
					buildGame();
				}
				if (ImGui::MenuItem("Exit", "Alt+F4")) {
					terminate();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo", "CTRL+Z", false, undoRedoManager.isUndoAvailable())) {
					undoRedoManager.undo();
					gameView.updateTargetData();
				}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, undoRedoManager.isRedoAvailable())) {
					undoRedoManager.redo();
				}
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
			if (ImGui::BeginMenu("Game Object")) {
				if (ImGui::MenuItem("Create new")) {
					open_create_entity_popup = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		static std::wstring dirPath = L"";
		static bool chooseDirPath = false;

		if (open_create_scene_popup && engineDLL.isLoaded() && projectSettings.isLoaded()) {
			std::wstring path = getSaveFileName(L"Choose scene location", L"Scene\0*.scene;\0", 1, projectSettings.getPath().c_str());
			if (!path.empty()) {
				if (!path.ends_with(SCENE_FILE_TYPE)) {
					path.append(SCENE_FILE_TYPE);
				}
				std::size_t nameStartIndex = path.find_last_of(L"\\");
				std::wstring fileName = path.substr(nameStartIndex == std::wstring::npos ? 0 : nameStartIndex + 1);
				std::string encodedName = utf8_encode(fileName.substr(0, fileName.find_last_of(L".")));
				std::string encodedPath = utf8_encode(path);
				engineDLL.createScene(encodedName.c_str(), encodedPath.c_str());
				projectSettings.addOpenScene(path);
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
			ImGui::Text(utf8_encode(dirPath).append(dirPath.ends_with(L"\\") ? "" : "\\").append(buffer).c_str());
			ImGui::InputText("Project name", buffer, 64);

			if (ImGui::Button("Create", ImVec2(120, 0))) {
				std::wstring name = utf8_decode(buffer);
				std::wstring path = std::wstring(dirPath).append(path.ends_with(L"/") || path.ends_with(L"\\") ? L"" : L"/").append(name);

				if (createProject(name, path)) {
					openProject(path);
					errorMessage = "";
					dirPath = L"";
					memset(buffer, 0, 64);
					ImGui::CloseCurrentPopup();
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
			if (engineDLL.isLoaded()) {
				static std::string errorMessage = "";
				if (!errorMessage.empty()) {
					ImGui::TextColored(ImVec4(1, 0, 0, 1), errorMessage.c_str());
				}
				static char buffer[64] = "";
				ImGui::InputText("Name", buffer, 64);

				if (ImGui::Button("Create", ImVec2(120, 0))) {
					std::string name = std::string(buffer);
					if (engineDLL.isEntityNameAvailable(name.c_str())) {
						std::size_t sceneIndex = gameView.getTarget().sceneIndex;
						ImVec2 pos = engineDLL.getCameraPosition();
						ImVec2 viewportSize = gameView.getViewportSize();
						float x = pos.x + viewportSize.x / 2;
						float y = pos.y + viewportSize.y / 2;
						float width = 350;
						float height = 350;
						if (engineDLL.createTemplateEntity(sceneIndex, name.c_str(), x, y, width, height)) {
							undoRedoManager.registerUndo(std::make_unique<DestroyEntityAction>(DestroyEntityAction(sceneIndex, name)));
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
		gameView.tick(deltaTime, currentFPS);
		EntityTargetData target = gameView.getTarget();
		hierarchy.tick(target.entityID);
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
	if (projectSettings.isLoaded())
		projectSettings.save();
	return 0;
}

void LevelEditor::terminate() {
	// TODO: Check for unsaved changes
	running = false;
}

bool LevelEditor::createProject(std::wstring name, std::wstring path) {
	// TODO: Check if there already is a project inside selected path
	ProjectSettings newProject = ProjectSettings::create(name.c_str(), path.c_str());
	if (!newProject.isLoaded()) {
		std::cout << "Failed to create new project" << std::endl;
		return false;
	}
	newProject.setEngineDLLPath(L"${ProjectDir}Engine/x64/Release/Game.dll");
	if (!newProject.save()) {
		return false;
	}

	std::cout << "Creating project directories and installing Engine source code..." << std::endl;
	std::filesystem::create_directories(path + L"/Assets");
	std::filesystem::create_directories(path + L"/Scenes");
	std::filesystem::create_directories(path + L"/Engine");
	std::filesystem::create_directories(path + L"/Engine/Core");
	std::filesystem::create_directories(path + L"/Engine/Game");
	std::filesystem::create_directories(path + L"/Engine/x64/Release");
	std::filesystem::create_directories(path + L"/Engine/libs");
	std::filesystem::create_directories(path + L"/Engine/tools");
	auto copyOptions = std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing;
	std::filesystem::copy(ASSETS_PATH, path + L"/Assets", copyOptions);
	std::filesystem::copy(std::wstring(ENGINE_SOLUTION_PATH).append(L"/Core"), path + L"/Engine/Core", copyOptions);
	std::filesystem::copy(std::wstring(ENGINE_SOLUTION_PATH).append(L"/Game"), path + L"/Engine/Game", copyOptions);
	std::filesystem::copy(std::wstring(ENGINE_SOLUTION_PATH).append(ENGINE_SOLUTION_BUILD_PATH), path + L"/Engine" + ENGINE_SOLUTION_BUILD_PATH, copyOptions);
	std::filesystem::copy(std::wstring(ENGINE_SOLUTION_PATH).append(L"/libs"), path + L"/Engine/libs", copyOptions);
	std::filesystem::copy(std::wstring(ENGINE_SOLUTION_PATH).append(L"/tools"), path + L"/Engine/tools", copyOptions);
	std::filesystem::copy_file(std::wstring(ENGINE_SOLUTION_PATH).append(L"/Engine.sln"), path + L"/Engine/Engine.sln");
	std::cout << "Project has been created successfully!" << std::endl;
	return true;
}

void LevelEditor::closeProject() {
	if (!projectSettings.isLoaded()) return;
	if (engineDLL.isLoaded()) {
		unloadEngine();
	}
	std::wstring editorTempPath = projectSettings.getPath() + TEMP_EDITOR_BUILD_PATH;
	std::filesystem::remove_all(editorTempPath);
}

void LevelEditor::openProject(std::wstring path) {
	if (path.empty()) return;
	closeProject();
	std::size_t seperator = path.find_last_of(L"\\");
	if (seperator == std::wstring::npos) {
		seperator = path.find_last_of(L"/");
	}
	std::wstring fileName = path.substr(seperator + 1);
	std::wstring dirPath = path.substr(0, seperator);
	projectSettings = ProjectSettings::load(fileName.substr(0, fileName.find_last_of(L".")).c_str(), dirPath.c_str());
	if (!projectSettings.isLoaded()) {
		std::wcout << L"Failed to load project at: " << path << std::endl;
		return;
	}
	editorSettings.pushRecentProjectPath(path);
	fileView.setSourcePath(dirPath);

	// Load Game.dll
	std::wstring dllPath = copyEngineDLL();
	if (!dllPath.empty()) {
		loadEngine(dllPath);
	}
	else {
		std::cout << "LevelEditor::openProject::WARNING Failed to copy Engine DLL to temp Editor directory" << std::endl;
	}

	// Open scenes
	std::vector<std::wstring> scenes = projectSettings.getOpenScenes();
	projectSettings.getOpenScenes().clear();
	for (std::wstring& scenePath : scenes) {
		openScene(scenePath);
	}
}

bool LevelEditor::openScene(std::wstring path) {
	if (!engineDLL.isLoaded()) {
		std::cout << "Unable to open scene because the Engine has not been loaded" << std::endl;
		return false;
	}
	std::string encodedPath = utf8_encode(path);
	if (engineDLL.loadScene(encodedPath.c_str())) {
		projectSettings.addOpenScene(path);
		return true;
	}
	return false;
}

bool LevelEditor::openSceneFromBackup(std::wstring srcPath, std::wstring destPath) {
	if (!engineDLL.isLoaded()) {
		std::cout << "Unable to open scene because the Engine has not been loaded" << std::endl;
		return false;
	}
	std::string encodedSrcPath = utf8_encode(srcPath);
	std::string encodedDestPath = utf8_encode(destPath);
	if (engineDLL.loadSceneBackup(encodedSrcPath.c_str(), encodedDestPath.c_str())) {
		projectSettings.addOpenScene(destPath);
		return true;
	}
	return false;
}

void LevelEditor::closeScene(std::size_t sceneIndex) {
	if (!engineDLL.isLoaded()) {
		std::cout << "Unable to close scene because the Engine has not been loaded" << std::endl;
		return;
	}
	std::wstring scenePath = utf8_decode(engineDLL.getSceneFilePath(sceneIndex));
	if (engineDLL.unloadScene(sceneIndex)) {
		if (gameView.getTarget().sceneIndex == sceneIndex) {
			gameView.releaseTarget();
		}
		undoRedoManager.removeSceneFromStack(sceneIndex);
		projectSettings.removeOpenScene(scenePath);
	}
}

std::wstring LevelEditor::getProjectName() {
	return projectSettings.getName();
}

EngineDLL* LevelEditor::getEngineDLL() {
	return &engineDLL;
}

Hierarchy* LevelEditor::getHierarchy() {
	return &hierarchy;
}

std::wstring LevelEditor::copyEngineDLL() {
	if (!projectSettings.isLoaded()) {
		std::cout << "LevelEditor::copyEngineDLL::ERROR No project is currently loaded!" << std::endl;
		return std::wstring();
	}
	std::wstring dllPath = projectSettings.getEngineDLLPath();
	std::wstring newDLLPath = projectSettings.getPath() + TEMP_EDITOR_BUILD_PATH + L"/" + engineDLL.getNextDLLName();
	// Moves Engine DLL to allow for the Game solution to build while Editor is running
	if (std::filesystem::exists(dllPath)) {
		std::wstring tempDirPath = projectSettings.getPath() + TEMP_EDITOR_BUILD_PATH;
		if (!std::filesystem::is_directory(tempDirPath)) {
			if (std::filesystem::exists(tempDirPath)) {
				std::wcout << L"Failed to create temp Editor directory. A file called .Editor is blocking creation of the directory. Path: " << tempDirPath << std::endl;
				return std::wstring();
			}
			if (!std::filesystem::create_directory(tempDirPath)) {
				std::cout << "Failed to create temp Editor directory" << std::endl;
				return std::wstring();
			}
		}
		std::wcout << dllPath << std::endl;
		std::wcout << newDLLPath << std::endl;
		try {
			std::filesystem::copy_file(dllPath, newDLLPath, std::filesystem::copy_options::overwrite_existing);
		}
		catch (const std::filesystem::filesystem_error& error) {
			std::cout << error.what() << std::endl;
			return std::wstring();
		}
	}
	else {
		std::cout << "No Engine.dll found in project Engine DLL Path" << std::endl;
		return std::wstring();
	}
	return newDLLPath;
}

bool LevelEditor::loadEngine(std::wstring path) {
	if (path.empty()) return false;
	if (engineDLL.isLoaded()) {
		std::cout << "Unable to load engine DLL because one is already loaded" << std::endl;
		return false;
	}
	if (engineDLL.load(path.c_str())) {
		undoRedoManager.clearStack();
		// Initialize egine
		int width, height;
		glfwGetWindowSize(glfwWindow, &width, &height);
		if (!engineDLL.engineInit((GLADloadproc)glfwGetProcAddress, width, height)) {
			std::cout << "Failed to initiate the Engine. Unloading the Engine DLL now..." << std::endl;
			unloadEngine();
			return false;
		}
		ImVec2 viewPortSize = gameView.getViewportSize();
		engineDLL.setViewportSize(viewPortSize.x, viewPortSize.y);
		engineDLL.setCallbackPtr(this);

		hierarchy.initiate(); // Initiate hierarchy view for new Engine DLL
		std::cout << "Successfully loaded new Engine DLL" << std::endl;
		return true;
	}
	return false;
}

bool LevelEditor::unloadEngine() {
	undoRedoManager.clearStack();
	hierarchy.clear();
	gameView.releaseTarget();
	engineDLL.unload();
	return true;
}

bool LevelEditor::reloadEngine(std::wstring path) {
	if (!engineDLL.isLoaded()) {
		std::cout << "LevelEditor::reloadEngine::ERROR Unable to reload Engine DLL before one has been loaded" << std::endl;
		return false;
	}
	std::wstring tempEditorDirectory = projectSettings.getPath() + TEMP_EDITOR_BUILD_PATH + L"/";
	std::cout << "Reloading dll" << std::endl;
	// Save state
	std::size_t sceneCount = engineDLL.getSceneCount();
	std::vector<std::pair<std::string, std::string>> tempScenes(sceneCount);
	for (std::size_t i = 0; i < sceneCount; i++) {
		std::wstring scenePath = tempEditorDirectory + std::to_wstring(i) + SCENE_FILE_TYPE;
		std::string encodedScenePath = utf8_encode(scenePath);
		std::string destScenePath = engineDLL.getSceneFilePath(i);
		if (!engineDLL.saveSceneBackup(i, encodedScenePath.c_str())) {
			std::wcout << L"Failed to save scene backup. Path: " << scenePath << L". Canceling reload of Engine DLL..." << std::endl;
			return false;
		}
		tempScenes[i] = std::make_pair(encodedScenePath, destScenePath);
	}
	std::string targetName = gameView.getTarget().entityName;
	// End of save state
	unloadEngine();
	loadEngine(path);
	// Load state
	projectSettings.clearOpenScenes();
	for (auto scene : tempScenes) {
		engineDLL.loadSceneBackup(scene.first.c_str(), scene.second.c_str());
		if (!std::filesystem::remove(scene.first.c_str())) {
			std::cout << "Failed to cleanup temp scene file: " << scene.first << std::endl;
		}
	}
	EntityID targetID = engineDLL.getEntityFromName(targetName.c_str());
	gameView.setTarget(targetID);
	// End of load state
}

bool LevelEditor::buildGame() {
	std::cout << "Build game is currently a WIP" << std::endl;
	return false;
}