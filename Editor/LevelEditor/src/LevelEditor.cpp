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

constexpr wchar_t PREFAB_FILE_TYPE[] = L".prefab";
ImVec2 resolutionDefault = ImVec2(600, 480);

LevelEditor::LevelEditor() : undoRedoManager(&engineDLL), gameView(this, &undoRedoManager), inspector(&engineDLL, &gameView, &undoRedoManager, &popupManager), hierarchy(this, &gameView, &undoRedoManager), fileView(this) {
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

static void glfw_window_close_callback(GLFWwindow* window) {
	LevelEditor* editor = static_cast<LevelEditor*>(glfwGetWindowUserPointer(window));
	editor->terminate();
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
	glfwSetWindowUserPointer(glfwWindow, this);
	glfwSetWindowCloseCallback(glfwWindow, glfw_window_close_callback);

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
	while (running) {
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

		if (isInEditMode()) {
			if (ImGui::IsKeyPressed(GLFW_KEY_Z) && ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
				undoRedoManager.undo();
			}
			else if (ImGui::IsKeyPressed(GLFW_KEY_Y) && ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
				undoRedoManager.redo();
			}
			else if (ImGui::IsKeyPressed(GLFW_KEY_S) && ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
				saveAll();
			}
		}

		menubar.tick(this, editMode);
		popupManager.tick(this);
		toolbar.tick(this);

		ImGui::End();

		// Inspector window
		ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);
		inspector.tick();
		// Game view window
		gameView.tick(paused ? 0 : deltaTime, editMode, currentFPS);
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

void LevelEditor::terminate(bool force) {
	if (force) {
		running = false;
		return;
	}
	if (!isInEditMode()) {
		setEditMode(true);
	}
	bool hasUnsavedChanges = false;
	if (engineDLL.isLoaded()) {
		for (const EntityID& entityID : engineDLL.getAllEntities()) {
			if (!engineDLL.hasEntityParent(entityID)) {
				if (undoRedoManager.isUnsaved(entityID)) {
					hasUnsavedChanges = true;
					break;
				}
			}
		}
	}
	if (hasUnsavedChanges) {
		popupManager.openSaveChanges();
	}
	else {
		running = false;
	}
}

bool LevelEditor::createProject(std::wstring name, std::wstring path) {
	if (!isInEditMode()) return false;
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
	if (!isInEditMode() || !projectSettings.isLoaded()) return;
	if (engineDLL.isLoaded()) {
		unloadEngine();
	}
	std::wstring editorTempPath = projectSettings.getPath() + TEMP_EDITOR_BUILD_PATH;
	std::filesystem::remove_all(editorTempPath);
}

void LevelEditor::openProject(std::wstring path) {
	if (!isInEditMode() || path.empty()) return;
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
		openPrefabAsScene(scenePath);
	}
}

bool LevelEditor::openPrefabAsScene(std::wstring path) {
	if (!isInEditMode()) return false;
	if (!engineDLL.isLoaded()) {
		std::cout << "LevelEditor::openPrefabAsScene::ERROR Unable to open scene because the Engine has not been loaded" << std::endl;
		return false;
	}
	bool isAlreadyOpen = false;
	for (const EntityID& entityID : engineDLL.getAllEntities()) {
		if (engineDLL.isEntityPrefabRoot(entityID) && engineDLL.getEntityParent(entityID) == 0) {
			std::string filePath = engineDLL.getPrefabFilePath(entityID);
			std::wstring decodedFilepath = utf8_decode(filePath);
			if (path == decodedFilepath) {
				isAlreadyOpen = true;
				break;
			}
		}
	}
	if (isAlreadyOpen) {
		std::cout << "LevelEditor::openPrefabAsScene::ERROR Unable to open prefab as scene because it is already open" << std::endl;
		return false;
	}
	std::string encodedPath = utf8_encode(path);
	// Get scene name from path
	std::size_t nameStartIndex = path.find_last_of(L"\\");
	std::wstring fileName = path.substr(nameStartIndex == std::wstring::npos ? 0 : nameStartIndex + 1);
	std::string sceneName = utf8_encode(fileName.substr(0, fileName.find_last_of(L".")));
	// Create scene root node
	EntityID entityID = engineDLL.createEntityFromPrefab(encodedPath.c_str(), 0, 0);
	if (!engineDLL.setEntityName(entityID, sceneName.c_str())) {
		std::cout << "LevelEditor::openPrefabAsScene::ERROR Failed to name the scene at path: " << encodedPath << std::endl;
		if (!engineDLL.destroyEntity(entityID)) {
			std::cout << "LevelEditor::openPrefabAsScene::ERROR Failed to remove scene at path: " << encodedPath << std::endl;
		}
		return false;
	}
	if (entityID) {
		projectSettings.addOpenScene(path);
		hierarchy.setActiveScene(entityID);
		return true;
	}
	return false;
}

void LevelEditor::closeScene(EntityID rootEntityID) {
	if (!isInEditMode()) return;
	if (!engineDLL.isLoaded()) {
		std::cout << "Unable to close scene because the Engine has not been loaded" << std::endl;
		return;
	}
	if (!engineDLL.isEntityPrefabRoot(rootEntityID)) {
		std::cout << "Unable to close given scene " << rootEntityID << " because it is not a prefab" << std::endl;
		return;
	}
	if (engineDLL.hasEntityParent(rootEntityID)) {
		std::cout << "Unable to close given scene " << rootEntityID << "because it is not a root" << std::endl;
		return;
	}
	std::wstring scenePath = utf8_decode(engineDLL.getPrefabFilePath(rootEntityID));
	if (engineDLL.destroyEntity(rootEntityID)) {
		// TODO: Check for unsaved changes?
		if (gameView.getTarget().rootEntityID == rootEntityID) {
			gameView.releaseTarget();
		}
		undoRedoManager.removeEntityFromStack(rootEntityID);
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
		engineDLL.setAssetDirPath(utf8_encode(projectSettings.getAssetDirPath()).c_str());
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
	std::cout << "Reloading dll" << std::endl;
	// Save state
	if (!saveGameState()) {
		std::wcout << L"Failed to save game state. Canceling reload of Engine DLL..." << std::endl;
		return false;
	}
	ImVec2 cameraPosition = engineDLL.getCameraPosition();
	// End of save state
	unloadEngine();
	loadEngine(path);
	// Load state
	engineDLL.setCameraScale(1.0f / gameView.getZoom());
	engineDLL.setCameraPosition(cameraPosition.x, cameraPosition.y);

	return loadGameState();
	// End of load state
}

bool LevelEditor::buildGame() {
	if (!isInEditMode()) return false;
	std::cout << "Build game is currently a WIP" << std::endl;
	return false;
}

bool LevelEditor::saveAll() {
	if (!isInEditMode()) return false;
	for (const EntityID& entityID : engineDLL.getAllEntities()) {
		if (!engineDLL.hasEntityParent(entityID) && engineDLL.isEntityPrefabRoot(entityID)) {
			if (undoRedoManager.isUnsaved(entityID)) {
				auto path = engineDLL.getPrefabFilePath(entityID);
				if (engineDLL.savePrefab(entityID, path.c_str())) {
					undoRedoManager.resetStepsSinceSave(entityID);
					undoRedoManager.setDirty(entityID, false);
					if (!engineDLL.updatePrefabs(path.c_str())) {
						std::cout << "LevelEditor::saveAll::ERROR Failed to update prefabs connected to path: " << path << std::endl;
					}
				}
				else {
					std::cout << "LevelEditor::saveAll::ERROR Failed to save scene " << entityID << " to path: " << path << std::endl;
				}
			}
		}
	}
	projectSettings.save();
	editorSettings.save();
	return true;
}

bool LevelEditor::isInEditMode() const {
	return editMode;
}

void LevelEditor::setEditMode(bool value) {
	if (editMode == value) return;
	editMode = value;
	// TODO: EditMode = false, disable save, disable UndoRedoStack
	if (value) {
		gameView.releaseTarget();
		loadGameState();
		undoRedoManager.enable();
	}
	else {
		saveGameState();
		undoRedoManager.disable();
	}
}

bool LevelEditor::isPaused() const {
	return paused;
}

void LevelEditor::setPaused(bool value) {
	paused = value;
}

ProjectSettings* LevelEditor::getProjectSettings() {
	return &projectSettings;
}

UndoRedoManager* LevelEditor::getUndoRedoManager() {
	return &undoRedoManager;
}

GameView* LevelEditor::getGameView() {
	return &gameView;
}

FileView* LevelEditor::getFileView() {
	return &fileView;
}

PopupManager* LevelEditor::getPopupManager() {
	return &popupManager;
}

bool LevelEditor::saveGameState() {
	std::wstring tempEditorDirectory = projectSettings.getPath() + TEMP_EDITOR_BUILD_PATH + L"/";
	std::wstring decodedPath = tempEditorDirectory + L"temp" + PREFAB_FILE_TYPE;
	std::string path = utf8_encode(decodedPath);
	return engineDLL.saveGameState(path.c_str());
}

bool LevelEditor::loadGameState() {
	std::wstring tempEditorDirectory = projectSettings.getPath() + TEMP_EDITOR_BUILD_PATH + L"/";
	std::wstring decodedPath = tempEditorDirectory + L"temp" + PREFAB_FILE_TYPE;
	std::string path = utf8_encode(decodedPath);
	if (!engineDLL.loadGameState(path.c_str())) {
		std::cout << "Failed to load game state temp scene file: " << std::endl;
		return false;
	}
	if (!std::filesystem::remove(path)) {
		std::cout << "Failed to cleanup temp scene file: " << path << std::endl;
	}
	return true;
}