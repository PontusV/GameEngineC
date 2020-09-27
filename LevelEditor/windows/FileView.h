#ifndef FILE_VIEW_H
#define FILE_VIEW_H

#include <string>
#include <vector>
#include <filesystem>

namespace Editor {

	class LevelEditor;

	struct FileEntry {
		std::filesystem::directory_entry entry;
		std::vector<FileEntry> children;
	};

	class FileView {
	public:
		FileView(LevelEditor* editor);
		~FileView();
		/* Renders ImGui window */
		void tick();

		/* Updates files to show current files */
		void refresh();

		void setSourcePath(std::wstring path);

	private:
		void renderDirectory(std::vector<FileEntry> entries);
	private:
		LevelEditor* editor;
		std::wstring sourcePath; // Path to the project folder
		std::vector<FileEntry> sourceFileEntries;
	};
}
#endif