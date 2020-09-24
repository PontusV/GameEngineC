#ifndef FILE_VIEW_H
#define FILE_VIEW_H

#include <string>

namespace Editor {
	class FileView {
	public:
		FileView();
		~FileView();
		/* Renders ImGui window */
		void tick();
	private:
		std::wstring sourcePath; // Path to the project folder
	};
}
#endif