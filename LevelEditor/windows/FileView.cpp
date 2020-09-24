#include "FileView.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace Editor;

FileView::FileView() {}
FileView::~FileView() {}

void FileView::tick() {
	ImGui::Begin("File View");
	ImGui::Text("WORK IN PROGRESS");
	ImGui::End();
}