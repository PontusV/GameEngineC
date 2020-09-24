#include "ProjectSettings.h"
#include <iostream>
#include <fstream>
#include "utils/string.h"
#include <filesystem>

using namespace Editor;

ProjectSettings::ProjectSettings() {}
ProjectSettings::~ProjectSettings() {}

/* Reads settings from path */
bool ProjectSettings::read(const wchar_t* path) {
	std::ifstream file;
	file.open(path, std::ios::in);
	if (!file.is_open()) return false;
	file.close();
	return true;
}
/* Writes the current settings to path */
bool ProjectSettings::write(const wchar_t* path) {
	std::ofstream file;
	file.open(path, std::ios::out | std::ios::trunc);
	if (!file.is_open()) return false;
	file.close();
	return true;
}

ProjectSettings ProjectSettings::create(std::wstring name, const wchar_t* path) {
	ProjectSettings settings;
	if (!std::filesystem::create_directory(path)) {
		return settings;
	}
	settings.name = name;
	settings.path = path;
	settings.save();
	return settings;
}

ProjectSettings ProjectSettings::load(std::wstring name, const wchar_t* path) {
	ProjectSettings settings;
	settings.name = name;
	settings.path = path;
	settings.read(settings.getFilePath().c_str());
	return settings;
}

void ProjectSettings::save() {
	write(getFilePath().c_str());
}

bool ProjectSettings::isLoaded() {
	return !path.empty();
}

std::wstring ProjectSettings::getName() {
	return name;
}
std::wstring ProjectSettings::getPath() {
	return path;
}
std::wstring ProjectSettings::getFilePath() {
	std::wstring filePath = path;
	return filePath.append(name).append(L".proj");
}