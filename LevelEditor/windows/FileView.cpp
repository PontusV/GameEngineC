#include "FileView.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "utils/string.h"
#include "LevelEditor.h"

using namespace Editor;

FileView::FileView(LevelEditor* editor): editor(editor) {}
FileView::~FileView() {}

std::string getFileNameFromPath(std::string path) {
	return path.substr(path.find_last_of("\\") + 1);
}

void FileView::renderDirectory(std::vector<FileEntry> entries) {
	for (const FileEntry& entryData : entries) {
		const auto& entry = entryData.entry;
		std::string fileName = getFileNameFromPath(entry.path().string());
		if (entry.is_directory()) {
			if (ImGui::TreeNodeEx(fileName.c_str(), ImGuiTreeNodeFlags_OpenOnArrow)) {
				renderDirectory(entryData.children);
				ImGui::TreePop();
			}
		}
		else {
			if (ImGui::TreeNodeEx(fileName.c_str(), ImGuiTreeNodeFlags_Leaf)) {
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					if (fileName.ends_with(".proj")) {
						editor->openProject(entry.path().wstring());
					} else if (fileName.ends_with(".scene")) {
						editor->openScene(entry.path().wstring());
					}
				}
				ImGui::TreePop();
			}
		}
	}
}

void FileView::tick() {
	ImGui::Begin("File Browser");
	if (ImGui::Button("Refresh")) {
		refresh();
	}
	renderDirectory(sourceFileEntries);
	ImGui::End();
}

void FileView::setSourcePath(std::wstring path) {
	sourcePath = path;
	refresh();
}

std::vector<FileEntry> getFileEntries(std::wstring dirPath) {
	std::vector<FileEntry> result;
	if (std::filesystem::exists(dirPath)) {
		for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
			if (entry.is_directory()) {
				result.push_back({ entry, getFileEntries(entry.path().wstring()) });
			}
			else {
				result.push_back({ entry, std::vector<FileEntry>() });
			}
		}
	}
	return result;
}

void FileView::refresh() {
	std::wcout << L"refresh " << sourcePath << std::endl;
	sourceFileEntries = getFileEntries(sourcePath);
}