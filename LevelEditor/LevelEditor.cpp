#include "LevelEditor.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <Windows.h>

using namespace Core;
using namespace Editor;

LevelEditor::LevelEditor() {
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

	// Example scene
	ScenePtr sceneWorld = engine.getSceneManager().createScene("World");

	EntityHandle object = sceneWorld->createEntity("Test_Object",
		Image("resources/images/awesomeface.png"),
		RectTransform(350, 350, 350, 350, 0.0f, Alignment::CENTER),
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
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 150");

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = (GLfloat)glfwGetTime();
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImVec2 viewportSize = ImVec2(1.0f, 1.0f);
	viewport.initialize(viewportSize.x, viewportSize.y);

	// Game loop
	running = true;
	while (running && window.isActive()) {
		// Calculate delta time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Viewport
		ImGui::Begin("Scene window");

		ImVec2 currentViewportSize = ImVec2(ImGui::GetWindowContentRegionMax().x - 10, ImGui::GetWindowContentRegionMax().y - 30);
		if (viewportSize.x != currentViewportSize.x || viewportSize.y != currentViewportSize.y) {
			viewportSize = currentViewportSize;
			viewport.setSize(viewportSize.x, viewportSize.y);
			engine.resizeViewport(viewportSize.x, viewportSize.y);
		}

		// Game tick
		viewport.begin();
		engine.tick(deltaTime);
		viewport.end();

		ImGui::GetWindowDrawList()->AddImage((ImTextureID)static_cast<uintptr_t>(viewport.getTextureID()), ImVec2(ImGui::GetCursorScreenPos()), ImVec2(ImGui::GetCursorScreenPos().x + viewportSize.x, ImGui::GetCursorScreenPos().y + viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
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