#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <string>

namespace Editor {
	class ProjectSettings {
	public:
		ProjectSettings();
		~ProjectSettings();
		/* Saves the current settings */
		void save();
		/* Creates a new project settings file. Note: Overwrites any file with same name at specified path */
		static ProjectSettings create(const wchar_t* name, const wchar_t* path);
		/* Loads a new project settings file */
		static ProjectSettings load(const wchar_t* name, const wchar_t* path);

		bool isLoaded();

		std::wstring getName();
		std::wstring getPath();
		/* Returns the full path to the settings file */
		std::wstring getFilePath();

	private:
		/* Reads settings from path */
		bool read(const wchar_t* path);
		/* Writes the current settings to path */
		bool write(const wchar_t* path);

	private:
		std::wstring name;
		std::wstring path;
	};
}
#endif