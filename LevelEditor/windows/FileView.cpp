#include "FileView.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"

#include "utils/string.h"
#include "utils/Clipboard.h"
#include "LevelEditor.h"

#include <tuple>

using namespace Editor;

FileView::FileView(LevelEditor* editor): editor(editor) {}
FileView::~FileView() {}

std::string getFileNameFromPath(std::string path) {
	return path.substr(path.find_last_of("\\") + 1);
}

std::string getDirFromPath(std::string path) {
	return path.substr(0, path.find_last_of("\\"));
}

void FileView::renderDirectory(std::vector<FileEntry> entries) {
	for (const FileEntry& entryData : entries) {
		const std::filesystem::directory_entry& entry = entryData.entry;
		std::string filePath = Core::utf8_encode(entry.path().wstring());
		std::string fileName = getFileNameFromPath(filePath);
		std::string dirPath = getDirFromPath(filePath);

		bool renameCurrentActive = renameActive && renameFilePath == filePath;
		std::string treeNodeId = std::string("##").append(filePath);
		std::string treeNodeLabel = renameCurrentActive ? "" : fileName;
		std::string popupId = std::string(filePath).append("_popup");

		bool disableRename = false;
		bool openPopup = false;
		bool openDeleteModal = false;
		bool selected = isSelected(entry);

		ImGuiTreeNodeFlags flags = selected ? ImGuiTreeNodeFlags_Selected : 0;
		flags |= entry.is_directory() ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf;

		if (renameCurrentActive) ImGui::AlignTextToFramePadding();
		bool isOpen = ImGui::TreeNodeEx(treeNodeId.c_str(), flags, treeNodeLabel.c_str());

		bool isClicked = !selectedLastMousePress && isMousePressed && selected && ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) || !selected && ImGui::IsItemClicked(ImGuiMouseButton_Left);

		if (isClicked && !ImGui::GetIO().KeyCtrl) { // Select new
			selectedLastMousePress = true;
			deselectAll();
			select(entry);
		}
		else if (isClicked && selected) { // Select new
			selectedLastMousePress = true;
			deselect(entry);
		}
		else if (isClicked) { // Select new
			selectedLastMousePress = true;
			select(entry);
		}
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && !ImGui::GetIO().KeyCtrl && !ImGui::GetIO().KeyShift) {
			if (!selected) {
				deselectAll();
				select(entry);
			}
		}

		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("DIRECTORY_ENTRY", NULL, 0);
			if (selectedEntries.size() == 1) {
				ImGui::Text("%s", fileName.c_str());
			}
			else {
				ImGui::Text("%s", "Multiple files...");
			}
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("DIRECTORY_ENTRY")) {
				std::size_t startIndex = pasteQueue.size();
				pasteQueue.resize(pasteQueue.size() + selectedEntries.size());
				for (std::size_t i = 0; i < selectedEntries.size(); i++) {
					PasteEntry& newEntry = pasteQueue[startIndex + i];
					std::wstring sourcePath = selectedEntries[i].path().wstring();
					std::wstring fileName = sourcePath.substr(sourcePath.find_last_of(L"\\") + 1);
					newEntry.sourcePath = sourcePath;
					newEntry.destPath = Core::utf8_decode(std::string(entry.is_directory() ? filePath : dirPath)).append(L"\\").append(fileName);
					newEntry.move = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (renameCurrentActive) {
			ImGui::SameLine();
			std::string* value = &fileName;
			char buffer[64];
			strncpy_s(buffer, value->c_str(), value->size());

			if (ImGui::InputText("##File_Rename_Field", buffer, 64, ImGuiInputTextFlags_EnterReturnsTrue)) {
				renameActive = false;
				std::wstring oldFileName = Core::utf8_decode(std::string(dirPath).append("\\").append(fileName));
				std::wstring newFileName = Core::utf8_decode(std::string(dirPath).append("\\").append(buffer));
				int result = _wrename(oldFileName.c_str(), newFileName.c_str());
				if (result == 0) {
					std::wcout << L"Item successfully renamed from '" << oldFileName << L"' to '" << newFileName << L"'" << std::endl;
				}
				else {
					std::wcout << L"Item failed to rename from '" << oldFileName << L"' to '" << newFileName << L"'" << std::endl;
				}
				refreshNextFrame = true;
			}
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered()) {
				disableRename = true;
			}
		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			openPopup = true;
		}
		if (entry.is_directory()) {
			if (isOpen) {
				renderDirectory(entryData.children);
			}
		}
		else {
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (fileName.ends_with(".proj")) {
					editor->openProject(entry.path().wstring());
				}
				else if (fileName.ends_with(".scene")) {
					editor->openScene(entry.path().wstring());
				}
			}
		}
		if (isOpen) {
			ImGui::TreePop();
		}

		if (openPopup) {
			ImGui::OpenPopup(popupId.c_str());
		}
		if (ImGui::BeginPopup(popupId.c_str())) {
			bool selectedDir = selectedEntries.size() == 1 && selectedEntries[0].is_directory();
			// OPTIONAL TODO: Add file -> Scene, other resources
			if (ImGui::Selectable("Cut")) {
				cut();
			}
			if (ImGui::Selectable("Copy")) {
				copy();
			}
			if (selectedDir) {
				if (ImGui::Selectable("Paste")) {
					paste();
				}
			}
			if (ImGui::Selectable("Delete")) {
				openDeleteModal = true;
			}
			if (ImGui::Selectable("Rename", false, selectedEntries.size() == 1 ? 0 : ImGuiSelectableFlags_Disabled)) {
				renameActive = true;
				renameFilePath = filePath;
			}
			ImGui::EndPopup();
		}

		if (disableRename) {
			renameActive = false;
		}
		std::string deleteModalId = std::string("Delete item##").append(filePath);
		if (openDeleteModal || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))) {
			ImGui::OpenPopup(deleteModalId.c_str());
		}
		if (ImGui::BeginPopupModal(deleteModalId.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			static std::string errorMessage = "";
			if (!errorMessage.empty()) {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), errorMessage.c_str());
			}
			if (selectedEntries.size() == 1) {
				ImGui::Text(std::string("Are you sure you want to delete '").append(getFileNameFromPath(Core::utf8_encode(selectedEntries[0].path().wstring()))).append("'?").c_str());
			}
			else {
				ImGui::Text("Are you sure you want to delete the selected items?");
			}

			if (ImGui::Button("Confirm", ImVec2(120, 0))) {
				errorMessage = "";
				for (const std::filesystem::directory_entry& selectedEntry : selectedEntries) {
					std::string selectedPath = Core::utf8_encode(selectedEntry.path().wstring());
					int result = std::filesystem::remove_all(selectedEntry.path());
					if (result > 0) {
						std::cout << "Succesfully deleted: " << selectedPath << std::endl;
					}
					else {
						std::cout << "Failed to delete: " << selectedPath << std::endl;
						errorMessage = std::string("Failed to remove file: ").append(selectedPath);
						break;
					}
				}
				refreshNextFrame = true;
				if (errorMessage.empty()) {
					ImGui::CloseCurrentPopup();
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
	}
}

std::vector<std::wstring> getEntryPathsFromDirectory(std::wstring dirPath) {
	std::vector<std::wstring> result;
	if (!std::filesystem::is_directory(dirPath)) return result;
	for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
		result.push_back(entry.path().wstring());
	}
	return result;
}

void FileView::processPaste() {
	if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_C))) {
		copy();
	}
	else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_X))) {
		cut();
	}
	else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_V))) {
		paste();
	}
	if (pasteOpenError) {
		ImGui::OpenPopup("File_Paste_Error");
		pasteOpenError = false;
	}
	if (ImGui::BeginPopupModal("File_Paste_Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text(pasteErrorMessage.c_str());

		ImGui::SetItemDefaultFocus();
		if (ImGui::Button("OK", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
		return;
	}
	if (pasteQueue.empty()) return;
	if (pasteOpenOverwrite) {
		ImGui::OpenPopup("File_overwrite_popup");
		pasteOpenOverwrite = false;
	}
	if (ImGui::BeginPopupModal("File_overwrite_popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		PasteEntry& entry = pasteQueue.front();
		std::string fileName = getFileNameFromPath(Core::utf8_encode(entry.sourcePath));
		ImGui::Text(std::string("The destination already contains a file with the name ").append(fileName).c_str());
		ImGui::Text(std::string("src: ").append(Core::utf8_encode(entry.sourcePath)).c_str());
		ImGui::Text(std::string("dst: ").append(Core::utf8_encode(entry.destPath)).c_str());

		if (ImGui::Button("Replace")) {
			entry.overwriteApproved = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Replace All")) {
			for (PasteEntry& e : pasteQueue) {
				e.overwriteApproved = true;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Skip")) {
			pasteQueue.pop_front();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
		return;
	}
	while (!pasteQueue.empty() && !pasteOpenOverwrite) {
		const PasteEntry& entry = pasteQueue.front();
		const std::wstring& source = entry.sourcePath;
		const std::wstring& destination = entry.destPath;
		const bool& overwrite = entry.overwriteApproved;
		const bool& pasteFromCut = entry.move;
		if (source == destination) {
			pasteErrorMessage = "Error: The source and destination are the same";
			pasteOpenError = true;
			pasteQueue.pop_front();
			continue;
		}
		else if (destination.find(source) == 0) {
			pasteErrorMessage = "Error: The the destination folder is inside the source folder";
			pasteOpenError = true;
			pasteQueue.pop_front();
			continue;
		}
		else if (!std::filesystem::exists(source)) {
			pasteQueue.pop_front();
			continue;
		}
		std::wstring fileName = source.substr(source.find_last_of(L"\\") + 1);
		bool isDirectory = std::filesystem::is_directory(source);
		bool exists = std::filesystem::exists(destination);
		if (exists && !overwrite) {
			if (isDirectory) {
				std::vector<std::wstring> items = getEntryPathsFromDirectory(source);
				std::vector<PasteEntry> newEntries(items.size());

				for (std::size_t i = 0; i < items.size(); i++) {
					PasteEntry& newEntry = newEntries[i];
					std::wstring fileName = items[i].substr(items[i].find_last_of(L"\\") + 1);
					newEntry.sourcePath = items[i];
					newEntry.destPath = std::wstring(destination).append(L"\\").append(fileName);
					newEntry.move = entry.move;
					if (i == items.size() - 1 && entry.move) {
						newEntry.removePath = source;
					}
				}
				if (pasteFromCut && !entry.removePath.empty() && std::filesystem::is_empty(entry.removePath)) {
					std::filesystem::remove(entry.removePath);
				}
				pasteQueue.pop_front();
				pasteQueue.insert(pasteQueue.begin(), newEntries.begin(), newEntries.end());
			}
			else {
				pasteOpenOverwrite = true;
			}
		}
		else if (pasteFromCut && !exists) {
			std::error_code error;
			std::filesystem::rename(source, destination, error);
			if (error) {
				pasteErrorMessage = error.message();
				pasteOpenError = true;
				pasteQueue.clear();
				std::cout << "Error during rename " << error.value() << std::endl;
			}
			else {
				if (!entry.removePath.empty() && std::filesystem::is_empty(entry.removePath)) {
					std::filesystem::remove(entry.removePath);
				}
				pasteQueue.pop_front();
			}
			refreshNextFrame = true;
		}
		else {
			std::error_code error;
			std::filesystem::copy(source, destination, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive, error);
			if (error) {
				pasteErrorMessage = error.message();
				pasteOpenError = true;
				std::cout << "Error during copy " << error.value() << std::endl;
				pasteQueue.clear();
			}
			else if (pasteFromCut) {
				std::error_code removeError;
				std::filesystem::remove_all(source, removeError);
				if (removeError) {
					pasteErrorMessage = removeError.message();
					pasteOpenError = true;
					std::cout << "Error during remove after copy " << removeError.value() << std::endl;
					pasteQueue.clear();
				}
				else {
					if (!entry.removePath.empty() && std::filesystem::is_empty(entry.removePath)) {
						std::filesystem::remove(entry.removePath);
					}
				}
			}
			if (!pasteQueue.empty())
				pasteQueue.pop_front();
			refreshNextFrame = true;
		}
	}
}

void FileView::tick() {
	ImGui::Begin("File Browser");
	if (ImGui::Button("Refresh") || refreshNextFrame) {
		refresh();
	}
	renderDirectory(sourceFileEntries);
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		isMousePressed = false;
		selectedLastMousePress = false;
	}
	else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		isMousePressed = true;
	}
	processPaste();
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
	std::sort(result.begin(), result.end(), [](const FileEntry& lhs, const FileEntry& rhs) {
		bool isDirectory = lhs.entry.is_directory();
		bool otherIsDirectory = rhs.entry.is_directory();
		std::string string = lhs.entry.path().string();
		std::string otherString = rhs.entry.path().string();
		return std::tie(isDirectory, string) < std::tie(otherIsDirectory, otherString);
	});
	return result;
}

void FileView::refresh() {
	deselectAll();
	refreshNextFrame = false;
	std::wcout << L"refresh " << sourcePath << std::endl;
	sourceFileEntries = getFileEntries(sourcePath);
}

void FileView::select(std::filesystem::directory_entry filePath) {
	if (isSelected(filePath)) return;
	selectedEntries.push_back(filePath);
}
bool FileView::deselect(std::filesystem::directory_entry filePath) {
	for (auto it = selectedEntries.begin(); it < selectedEntries.end(); it++) {
		if (*it == filePath) {
			selectedEntries.erase(it);
			return true;
		}
	}
	return false;
}
void FileView::deselectAll() {
	selectedEntries.clear();
}

bool FileView::isSelected(const std::filesystem::directory_entry& filePath) const {
	for (const std::filesystem::directory_entry& path : selectedEntries) {
		if (path == filePath) return true;
	}
	return false;
}

void FileView::cut() {
	std::vector<std::wstring> copyPaths(selectedEntries.size());
	for (std::size_t i = 0; i < selectedEntries.size(); i++) {
		copyPaths[i] = selectedEntries[i].path().wstring();
	}
	Core::Clipboard::setDataAsFiles(copyPaths);
	clipboardCutSequenceNumber = Core::Clipboard::getSequenceNumber();
}

void FileView::copy() {
	std::vector<std::wstring> copyPaths(selectedEntries.size());
	for (std::size_t i = 0; i < selectedEntries.size(); i++) {
		copyPaths[i] = selectedEntries[i].path().wstring();
	}
	Core::Clipboard::setDataAsFiles(copyPaths);
}

void FileView::paste() {
	bool isFromCut = clipboardCutSequenceNumber == Core::Clipboard::getSequenceNumber();
	std::wstring destination = selectedEntries[0].path().wstring();
	std::vector<std::wstring> sourcePaths = Core::Clipboard::getDataAsFiles();
	std::size_t startIndex = pasteQueue.size();
	pasteQueue.resize(pasteQueue.size() + sourcePaths.size());
	for (std::size_t i = 0; i < sourcePaths.size(); i++) {
		PasteEntry& entry = pasteQueue[startIndex + i];
		std::wstring fileName = sourcePaths[i].substr(sourcePaths[i].find_last_of(L"\\") + 1);
		entry.sourcePath = sourcePaths[i];
		entry.destPath = std::wstring(destination).append(L"\\").append(fileName);
		entry.move = isFromCut;
	}
	if (isFromCut) {
		Core::Clipboard::clear();
	}
}