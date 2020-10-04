#include "EditorSettings.h"

#include "utils/string.h"
#include "utils/file.h"

#include <fstream>
#include <iostream>

using namespace Editor;

EditorSettings::EditorSettings() {
	std::fill(recentProjectPaths, recentProjectPaths + EDITOR_RECENT_PROJECT_COUNT, 0);
}

EditorSettings::~EditorSettings() {

}

/* Saves Editor settings to file */
bool EditorSettings::save() {
	std::ofstream file;
	file.open(Core::utf8_encode(getApplicationDirectoryPath()).append("\\").append(EDITOR_SETTINGS_FILE_NAME), std::ios::out | std::ios::trunc);
	if (!file.is_open()) return false;

	// SAVE
	file << EDITOR_RECENT_PROJECT_COUNT << '\0';
	for (std::size_t i = 0; i < EDITOR_RECENT_PROJECT_COUNT; i++) {
		file << Core::utf8_encode(recentProjectPaths[i]).c_str();
	}

	file.close();
	return true;
}
/* Loads Editor settings file */
bool EditorSettings::load() {
	std::ifstream file;
	file.open(Core::utf8_encode(getApplicationDirectoryPath()).append("\\").append(EDITOR_SETTINGS_FILE_NAME), std::ios::in);
	if (!file.is_open()) return false;

	// LOAD
	std::string recentCountString;
	std::getline(file, recentCountString, '\0');
	std::size_t recentCount = stoi(recentCountString);
	if (recentCount > EDITOR_RECENT_PROJECT_COUNT) {
		recentCount = EDITOR_RECENT_PROJECT_COUNT;
	}
	for (std::size_t i = 0; i < recentCount; i++) {
		std::string path;
		std::getline(file, path, '\0');
		recentProjectPaths[i] = Core::utf8_decode(path);
	}

	file.close();
	return true;
}

std::wstring* EditorSettings::getRecentProjectPaths() {
	return recentProjectPaths;
}

void EditorSettings::pushRecentProjectPath(std::wstring path) {
	bool exists = false;
	for (std::wstring recentPath : recentProjectPaths) {
		if (path == recentPath) {
			exists = true;
			return;
		}
	}
	if (exists) return;
	for (std::size_t i = 1; i < EDITOR_RECENT_PROJECT_COUNT; i++) {
		recentProjectPaths[i] = recentProjectPaths[i - 1];
	}
	recentProjectPaths[0] = path;
	save();
}