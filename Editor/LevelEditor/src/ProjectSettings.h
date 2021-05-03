#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <string>
#include <vector>

namespace Editor {
	class ProjectSettings {
	public:
		ProjectSettings();
		~ProjectSettings();
		/* Creates a new project settings file. Note: Overwrites any file with same name at specified path */
		static ProjectSettings create(const wchar_t* name, const wchar_t* path);
		/* Loads a new project settings file */
		static ProjectSettings load(const wchar_t* name, const wchar_t* path);

		/* Saves the current settings */
		bool save();
		/* Returns true if project settings have been loaded */
		bool isLoaded() const;

		/* Returns the name of the project */
		std::wstring getName() const;
		/* Returns the path to the project directory */
		std::wstring getPath() const;
		/* Returns the full path to the settings file */
		std::wstring getFilePath() const;

		/* Sets the build path of the Engine DLL */
		void setEngineDLLPath(std::wstring path);
		/* Returns the full parsed build path of the Engine DLL */
		std::wstring getEngineDLLPath() const;
		/* Returns the absolute path to the assets of the project */
		std::wstring getAssetDirPath() const;

		std::vector<std::wstring>& getOpenScenes();
		void addOpenScene(std::wstring path);
		void removeOpenScene(std::wstring path);
		void clearOpenScenes();


	private:
		/* Reads settings from file at path */
		bool read(const wchar_t* path);
		/* Writes the current settings to file at path. If no file currently exists one will be created. */
		bool write(const wchar_t* path);
		/* Parses the given path. Returns true if successful. Replaces certain symbols */
		bool parsePath(std::wstring& string) const;

	private:
		std::wstring name; // Name of the project
		std::wstring path; // Path to the project directory

		std::wstring engineDLLPath; // Path to the Engine / Game.dll
		std::vector<std::wstring> openScenes; // All currently opened scenes
	};
}
#endif