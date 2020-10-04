#ifndef FILE_VIEW_H
#define FILE_VIEW_H

#include <string>
#include <vector>
#include <deque>
#include <filesystem>
#include <utility>

namespace Editor {

	class LevelEditor;

	struct FileEntry {
		std::filesystem::directory_entry entry;
		std::vector<FileEntry> children;
	};

	struct PasteEntry {
		std::wstring sourcePath;
		std::wstring destPath;
		bool overwriteApproved = false;
		bool move = false;				// Determines whether the item will be copied or moved

		std::wstring removePath;
	};

	class FileView {
	public:
		FileView(LevelEditor* editor);
		~FileView();
		/* Renders ImGui window */
		void tick();

		/* Updates files to show current files */
		void refresh();
		
		void select(std::filesystem::directory_entry filePath);
		/* Returns true if deselect was successful */
		bool deselect(std::filesystem::directory_entry filePath);
		void deselectAll();
		bool isSelected(const std::filesystem::directory_entry& filePath) const;

		void setSourcePath(std::wstring path);

	private:
		void renderDirectory(std::vector<FileEntry> entries);
		void processPaste();

		void cut();
		void copy();
		void paste();

	private:
		LevelEditor* editor;
		std::wstring sourcePath; // Path to the project directory
		std::vector<FileEntry> sourceFileEntries;
		std::vector<std::filesystem::directory_entry> selectedEntries;
		unsigned long clipboardCutSequenceNumber = -1;
		bool isMousePressed = false; // Used to only listen to 1 tick of ImGui::IsMouseReleased
		bool selectedLastMousePress = false;

		bool pasteOpenError = false;
		std::string pasteErrorMessage;
		bool pasteOpenOverwrite = false;
		std::deque<PasteEntry> pasteQueue;

		bool refreshNextFrame = false;
		bool renameActive = false;
		std::string renameFilePath;
	};
}
#endif