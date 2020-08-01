#include "LevelEditor.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <Windows.h>

using namespace Core;
using namespace Editor;

LevelEditor::LevelEditor() : gameView(&engine), inspector(&gameView), hierarchy(&engine.getSceneManager()) {
	engine.getInput().addKeyListener(this);
}


LevelEditor::~LevelEditor() {
}


int LevelEditor::initiate() {
	// Initiate engine
	int result = engine.initiate(true);
	if (result != 0) {
		return result;
	}
	Window& window = engine.getGraphics().getWindow();
	window.setBackgroundColor(Vector3(0.15f, 0.15f, 0.15f));

	// Example scene
	ScenePtr sceneWorld = engine.getSceneManager().createScene("World");

	EntityHandle object = sceneWorld->createEntity("Test_Object",
		Image("resources/images/awesomeface.png"),
		RectTransform(350, 350, 350, 350, 0.0f, Alignment::CENTER),
		SpriteRenderer()
	);

	EntityHandle object2 = sceneWorld->createEntity("Test_Object_2",
		Image("resources/images/awesomeface.png"),
		Text(L"TEst едц", Font("resources/fonts/segoeui.ttf", 30), Color(255,255,255,255)),
		RectTransform(100, 100, 200, 200, 0.0f, Alignment::CENTER),
		SpriteRenderer()
	);

	// Level created, calling awake
	sceneWorld->awake();

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
		Vector2 windowSize = window.getResolution();
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y), ImGuiCond_Always);
		ImGui::Begin("Editor", &running, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

		ImGuiID dockspaceID = ImGui::GetID("MainDockSpace");
		ImGui::DockSpace(dockspaceID, ImVec2(0, 0));

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Dummy file")) {}
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
					std::cout << "Create new game object WIP" << std::endl;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
		// Inspector window
		ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);
		inspector.tick();
		// Game view window
		gameView.tick(deltaTime);
		EntityHandle target = gameView.getTarget();
		hierarchy.tick(target);

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