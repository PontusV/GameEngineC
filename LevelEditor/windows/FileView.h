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
		bool selected = false;
		bool renameActive = false;
		bool opened = false;
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
		
		void select(FileEntry& entry);
		/* Selects all items between clickTarget and entry */
		void shiftSelect(FileEntry& entry);
		/* Returns true if deselect was successful */
		bool deselect(FileEntry& entry);
		void deselectAll();

		void openFile(const std::filesystem::directory_entry& entry);
		bool rename(std::filesystem::directory_entry& entry);

		void setSourcePath(std::wstring path);

	private:
		void renderDirectory(std::vector<FileEntry>& entries);
		void processPaste();

		void cut();
		void copy();
		void paste();

		void createDirectory(std::wstring path);
		bool shiftSelectImpl(std::vector<FileEntry>& entries, FileEntry& other, bool& found);
	private:
		LevelEditor* editor;
		std::wstring sourcePath; // Path to the project directory
		std::vector<FileEntry> sourceFileEntries;
		std::vector<std::filesystem::directory_entry> selectedEntries;
		bool projectSelected = false;
		unsigned long clipboardCutSequenceNumber = -1;
		bool isMousePressed = false; // Used to only listen to 1 tick of ImGui::IsMouseReleased
		bool selectedLastMousePress = false;

		bool openItemPopup = false;
		bool openDeleteItemPopup = false;

		bool pasteOpenError = false;
		std::string pasteErrorMessage;
		bool pasteOpenOverwrite = false;
		std::deque<PasteEntry> pasteQueue;

		std::filesystem::directory_entry clickTarget; // The entry of the last clicked item
		bool refreshNextFrame = false;
		bool renameNextFrame = false;
		std::string renameFilePath;
	};
}
#endif