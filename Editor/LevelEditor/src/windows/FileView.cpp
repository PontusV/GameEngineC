#include "FileView.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include "IconsFontAwesome5.h"

#include "utils/string.h"
#include "utils/file.h"
#include "utils/Clipboard.h"
#include "LevelEditor.h"

#include <iostream>
#include <GLFW/glfw3.h>

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

void FileView::openFile(const std::filesystem::directory_entry& entry) {
	std::string filePath = utf8_encode(entry.path().wstring());
	std::string fileName = getFileNameFromPath(filePath);
	if (fileName.ends_with(".proj")) {
		editor->openProject(entry.path().wstring());
	}
	else if (fileName.ends_with(".scene")) {
		editor->openScene(entry.path().wstring());
	}
}

void FileView::renderDirectory(std::vector<FileEntry>& entries) {
	for (FileEntry& entryData : entries) {
		const std::filesystem::directory_entry& entry = entryData.entry;
		std::string filePath = utf8_encode(entry.path().wstring());
		std::string fileName = getFileNameFromPath(filePath);
		std::string dirPath = getDirFromPath(filePath);

		bool& renameActive = entryData.renameActive;
		if (renameActive && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
			renameActive = false;
		}
		if (renameNextFrame && renameFilePath == filePath) renameActive = true;
		std::string treeNodeId = std::string("##").append(filePath);
		std::string treeNodeLabel = renameActive ? "" : fileName;

		const char* icon = ICON_FA_FILE;
		if (entry.is_directory()) {
			ImGuiID nodeID = ImGui::GetID(treeNodeId.c_str());
			if (ImGui::TreeNodeBehaviorIsOpen(nodeID)) {
				icon = ICON_FA_FOLDER_OPEN;
			}
			else {
				icon = ICON_FA_FOLDER;
			}
		}
		else {
			// TODO: Different kind of files
			auto extension = entry.path().extension();
			if (extension == ".scene") {
				icon = ICON_FA_FILE_ALT;
			} else if (extension == ".proj") {
				icon = ICON_FA_FILE_ALT;
			}
			else if (extension == ".h" || extension == ".hpp" || extension == ".c" || extension == ".cpp") {
				icon = ICON_FA_FILE_CODE;
			}
			else {
				icon = ICON_FA_FILE;
			}
		}
		treeNodeLabel = icon + (" " + treeNodeLabel);

		bool selected = entryData.selected;
		bool windowFocused = ImGui::IsWindowFocused();

		ImGuiTreeNodeFlags flags = renameActive ? 0 : ImGuiTreeNodeFlags_SpanFullWidth; // TODO: Overlap the input text instead of reducing width
		if (selected) flags |= ImGuiTreeNodeFlags_Selected;
		flags |= entry.is_directory() ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf;

		if (renameActive) ImGui::AlignTextToFramePadding();
		if (!windowFocused) {
			ImVec4 color = ImVec4(1, 1, 1, 0.15f);
			ImGui::PushStyleColor(ImGuiCol_Header, color);
		}
		bool isOpen = ImGui::TreeNodeEx(treeNodeId.c_str(), flags, treeNodeLabel.c_str());
		if (!windowFocused) {
			ImGui::PopStyleColor();
		}
		entryData.opened = isOpen;
		// Imgui navigation select when focus changes
		if (ImGui::IsItemFocused() && prevFocusedEntry != entry) {
			prevFocusedEntry = entry;
			if (!ImGui::GetIO().KeyShift) {
				deselectAll();
				clickTarget = entry;
			}
			select(entryData);
		}

		bool isClicked = !renameActive && (!selectedLastMousePress && isMousePressed && selected && ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) || !selected && ImGui::IsItemClicked(ImGuiMouseButton_Left));

		if (isClicked) {
			selectedLastMousePress = true;
			if (ImGui::GetIO().KeyShift) {
				deselectAll();
				shiftSelect(entryData);
				if (ImGui::GetIO().KeyCtrl) clickTarget = entry;
			}
			else if (ImGui::GetIO().KeyCtrl && selected) {
				deselect(entryData);
			}
			else if (ImGui::GetIO().KeyCtrl) {
				select(entryData);
				clickTarget = entry;
			}
			else {
				deselectAll();
				select(entryData);
				clickTarget = entry;
			}
		}
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && !ImGui::GetIO().KeyCtrl && !ImGui::GetIO().KeyShift) {
			projectSelected = false;
			if (!selected) {
				deselectAll();
				select(entryData);
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
					newEntry.destPath = utf8_decode(std::string(entry.is_directory() ? filePath : dirPath)).append(L"\\").append(fileName);
					newEntry.move = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (renameActive) {
			ImGui::SameLine();
			std::string* value = &fileName;
			char buffer[64];
			strncpy_s(buffer, value->c_str(), value->size());

			if (ImGui::InputText("##File_Rename_Field", buffer, 64, ImGuiInputTextFlags_EnterReturnsTrue)) {
				renameActive = false;
				std::wstring oldFileName = utf8_decode(std::string(dirPath).append("\\").append(fileName));
				std::wstring newFileName = utf8_decode(std::string(dirPath).append("\\").append(buffer));
				int result = _wrename(oldFileName.c_str(), newFileName.c_str());
				if (result != 0) {
					std::wcout << L"Item failed to rename from '" << oldFileName << L"' to '" << newFileName << L"'" << std::endl;
				}
				refreshNextFrame = true;
			}
			ImGui::SetKeyboardFocusHere(0);
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered()) {
				renameActive = false;
			}
		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Right) || ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
			openItemPopup = true;
		}
		if (entry.is_directory()) {
			if (isOpen) {
				renderDirectory(entryData.children);
			}
		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			// Note: Disabled for now
			//openFile(entry);
		}
		if (isOpen) {
			ImGui::TreePop();
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
	if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_C)) && ImGui::IsWindowFocused()) {
		copy();
	}
	else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_X)) && ImGui::IsWindowFocused()) {
		cut();
	}
	else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_V)) && ImGui::IsWindowFocused()) {
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
		std::string fileName = getFileNameFromPath(utf8_encode(entry.sourcePath));
		ImGui::Text(std::string("The destination already contains a file with the name ").append(fileName).c_str());
		ImGui::Text(std::string("src: ").append(utf8_encode(entry.sourcePath)).c_str());
		ImGui::Text(std::string("dst: ").append(utf8_encode(entry.destPath)).c_str());

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
	std::string projectName = utf8_encode(editor->getProjectName());
	if (!projectName.empty()) {
		ImGuiTreeNodeFlags rootProjectFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
		if (projectSelected) rootProjectFlags |= ImGuiTreeNodeFlags_Selected;
		bool windowFocused = ImGui::IsWindowFocused();
		if (!windowFocused) {
			ImVec4 color = ImVec4(1, 1, 1, 0.15f);
			ImGui::PushStyleColor(ImGuiCol_Header, color);
		}
		bool isOpen = ImGui::TreeNodeEx(projectName.c_str(), rootProjectFlags); // TODO: Project Icon
		if (!windowFocused) {
			ImGui::PopStyleColor();
		}
		bool isClicked = !selectedLastMousePress && isMousePressed && projectSelected && ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) || !projectSelected && ImGui::IsItemClicked(ImGuiMouseButton_Left);
		if (isClicked && !ImGui::GetIO().KeyCtrl) {
			selectedLastMousePress = true;
			deselectAll();
			projectSelected = true;
		}
		else if (isClicked && projectSelected) {
			selectedLastMousePress = true;
			projectSelected = false;
		}
		else if (isClicked) {
			selectedLastMousePress = true;
			projectSelected = true;
		}
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && !ImGui::GetIO().KeyCtrl && !ImGui::GetIO().KeyShift) {
			if (!projectSelected) {
				deselectAll();
				projectSelected = true;
			}
		}
		//
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup("Project_popup_menu");
		}
		if (ImGui::BeginPopup("Project_popup_menu")) {
			bool selectedDir = selectedEntries.size() == 1 && selectedEntries[0].is_directory();
			if (ImGui::BeginMenu("Add")) {
				if (ImGui::MenuItem("New Item...")) {
					std::cout << "Work in progress" << std::endl;
					// TODO: Open Modal for creating new item in Editor
					refreshNextFrame = true;
				}
				if (ImGui::MenuItem("New Folder")) {
					createDirectory(sourcePath);
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Paste", nullptr, nullptr, selectedEntries.size() == 0)) {
				paste();
			}
			ImGui::EndPopup();
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("DIRECTORY_ENTRY")) {
				std::size_t startIndex = pasteQueue.size();
				pasteQueue.resize(pasteQueue.size() + selectedEntries.size());
				for (std::size_t i = 0; i < selectedEntries.size(); i++) {
					PasteEntry& newEntry = pasteQueue[startIndex + i];
					std::wstring srcPath = selectedEntries[i].path().wstring();
					std::wstring fileName = srcPath.substr(srcPath.find_last_of(L"\\") + 1);
					newEntry.sourcePath = srcPath;
					newEntry.destPath = std::wstring(sourcePath).append(L"\\").append(fileName);
					newEntry.move = true;
				}
			}
			ImGui::EndDragDropTarget();
		}
		openItemPopup = false;
		openDeleteItemPopup = false;
		bool disableRename = selectedEntries.size() != 1;
		if (!disableRename && ImGui::IsKeyPressed(GLFW_KEY_F2)) { // F2 key
			rename(selectedEntries[0]);
		}
		if (isOpen) {
			renderDirectory(sourceFileEntries);
			ImGui::TreePop();
		}
		renameNextFrame = false;
		if (openItemPopup) {
			ImGui::OpenPopup("Item popup menu");
		}
		if (ImGui::BeginPopup("Item popup menu")) {
			bool selectedDir = selectedEntries.size() == 1 && selectedEntries[0].is_directory();
			if (selectedDir) {
				if (ImGui::BeginMenu("Add")) {
					if (ImGui::MenuItem("New Item...")) {
						std::cout << "Work in progress" << std::endl;
						// TODO: Open Modal for creating new item in Editor
						refreshNextFrame = true;
					}
					if (ImGui::MenuItem("New Folder")) {
						createDirectory(selectedEntries[0].path().wstring());
					}
					ImGui::EndMenu();
				}
			}
			else {
				if (ImGui::MenuItem("Open") && selectedEntries.size() == 1) {
					openFile(selectedEntries[0]);
				}
			}
			if (ImGui::MenuItem("Cut", "CTRL+X")) {
				cut();
			}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {
				copy();
			}
			if (selectedDir) {
				if (ImGui::MenuItem("Paste", "CTRL+V")) {
					paste();
				}
			}
			if (ImGui::MenuItem("Delete", "Del")) {
				openDeleteItemPopup = true;
			}
			if (disableRename) ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disableRename);
			if (ImGui::MenuItem("Rename", "F2")) {
				rename(selectedEntries[0]);
			}
			ImGui::PopItemFlag();
			if (disableRename) ImGui::PopStyleColor();
			ImGui::EndPopup();
		}
		if (openDeleteItemPopup || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)) && windowFocused && selectedEntries.size() > 0) {
			ImGui::OpenPopup("Delete item(s)");
		}
		if (ImGui::BeginPopupModal("Delete item(s)", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			static std::string errorMessage = "";
			if (!errorMessage.empty()) {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), errorMessage.c_str());
			}
			if (selectedEntries.size() == 1) {
				ImGui::Text(std::string("Are you sure you want to delete '").append(getFileNameFromPath(utf8_encode(selectedEntries[0].path().wstring()))).append("'?").c_str());
			}
			else {
				ImGui::Text("Are you sure you want to delete the selected items?");
			}

			if (ImGui::Button("Confirm", ImVec2(120, 0))) {
				errorMessage = "";
				for (const std::filesystem::directory_entry& selectedEntry : selectedEntries) {
					std::string selectedPath = utf8_encode(selectedEntry.path().wstring());
					int result = std::filesystem::remove_all(selectedEntry.path());
					if (result == 0) {
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
	sourceFileEntries = getFileEntries(sourcePath);
}

FileEntry* findFileEntry(std::vector<FileEntry>& entries, const std::filesystem::directory_entry& entry) {
	for (FileEntry& item : entries) {
		if (item.entry == entry) return &item;
		FileEntry* ptr = findFileEntry(item.children, entry);
		if (ptr) return ptr;

	}
	return nullptr;
}

bool FileView::rename(std::filesystem::directory_entry& entry) {
	if (FileEntry* ptr = findFileEntry(sourceFileEntries, entry)) {
		ptr->renameActive = true;
		return true;
	}
	return false;
}

void FileView::select(FileEntry& item) {
	if (!item.selected) {
		selectedEntries.push_back(item.entry);
	}
	item.selected = true;
}
bool FileView::deselect(FileEntry& entry) {
	if (!entry.selected) return false;
	entry.selected = false;
	auto it = selectedEntries.begin();
	while (it != selectedEntries.end()) {
		if (*it == entry.entry) {
			it = selectedEntries.erase(it);
			break;
		}
		else {
			it++;
		}
	}
	return true;
}
void deselectEntries(std::vector<FileEntry>& entries) {
	for (FileEntry& entry : entries) {
		entry.selected = false;
		deselectEntries(entry.children);
	}
}
void FileView::deselectAll() {
	projectSelected = false;
	deselectEntries(sourceFileEntries);
	selectedEntries.clear();
}

bool FileView::shiftSelectImpl(std::vector<FileEntry>& entries, FileEntry& other, bool& found) {
	for (FileEntry& item : entries) {
		bool newFound = item.entry == other.entry || item.entry == clickTarget;
		if (found) {
			select(item);
			if (newFound) {
				return true;
			}
		}
		else if (newFound) {
			found = true;
			select(item);
		}
		if (item.entry.is_directory() && item.opened) {
			if (shiftSelectImpl(item.children, other, found)) {
				return true;
			}
		}
	}
	return false;
}

void FileView::shiftSelect(FileEntry& other) {
	bool found = false;
	shiftSelectImpl(sourceFileEntries, other, found);
}

void FileView::cut() {
	std::vector<std::wstring> copyPaths(selectedEntries.size());
	for (std::size_t i = 0; i < selectedEntries.size(); i++) {
		copyPaths[i] = selectedEntries[i].path().wstring();
	}
	Clipboard::setDataAsFiles(copyPaths);
	clipboardCutSequenceNumber = Clipboard::getSequenceNumber();
}

void FileView::copy() {
	std::vector<std::wstring> copyPaths(selectedEntries.size());
	for (std::size_t i = 0; i < selectedEntries.size(); i++) {
		copyPaths[i] = selectedEntries[i].path().wstring();
	}
	Clipboard::setDataAsFiles(copyPaths);
}

void FileView::paste() {
	if (selectedEntries.size() == 1 && projectSelected || selectedEntries.size() == 0 && !projectSelected) return;
	bool isFromCut = clipboardCutSequenceNumber == Clipboard::getSequenceNumber();
	std::wstring destination = projectSelected ? sourcePath : selectedEntries[0].path().wstring();
	std::vector<std::wstring> sourcePaths = Clipboard::getDataAsFiles();
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
		Clipboard::clear();
	}
}

void FileView::createDirectory(std::wstring path) {
	std::wstring srcDir = path.append(L"\\");
	std::wstring newDirectoryName = L"NewFolder";
	std::size_t dirIndex = 0;
	while (std::filesystem::exists(srcDir + newDirectoryName)) {
		if (dirIndex++ == 0)
			newDirectoryName.append(std::to_wstring(dirIndex));
		else
			newDirectoryName = newDirectoryName.substr(0, newDirectoryName.length() - 1).append(std::to_wstring(dirIndex));
	}
	if (std::filesystem::create_directory(srcDir + newDirectoryName)) {
		refreshNextFrame = true;
		renameNextFrame = true;
		renameFilePath = utf8_encode(srcDir + newDirectoryName);
	}
	else {
		// TODO: Show error
	}
}

bool FileView::isDirectorySelected() {
	return selectedEntries.size() == 1 && selectedEntries[0].is_directory();
}

std::size_t FileView::getSelectCount() {
	return selectedEntries.size();
}