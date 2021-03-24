#ifndef EDITOR_SETTINGS_H
#define EDITOR_SETTINGS_H

#define EDITOR_RECENT_PROJECT_COUNT 5
#define EDITOR_SETTINGS_FILE_NAME "editor.ini"

#include <string>

namespace Editor {
	/* Manages the Editor settings file */
	class EditorSettings {
	public:
		EditorSettings();
		~EditorSettings();

		/* Saves Editor settings to file */
		bool save();
		/* Loads Editor settings file */
		bool load();

		std::wstring* getRecentProjectPaths();
		/* Adds recent project path to from of array and saves current settings to file */
		void pushRecentProjectPath(std::wstring path);

	private:
		std::wstring recentProjectPaths[EDITOR_RECENT_PROJECT_COUNT];
	};
}
#endif