#include "ProjectSettings.h"
#include "utils/string.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>

using namespace Editor;

ProjectSettings::ProjectSettings() {}
ProjectSettings::~ProjectSettings() {}

void replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::wstring::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

bool ProjectSettings::parsePath(std::wstring& string) const {
	replaceAll(string, L"${ProjectDir}", std::wstring(path).append(L"/"));
	return true;
}

/* Reads settings from path */
bool ProjectSettings::read(const wchar_t* path) {
	try {
		std::ifstream file(path);
		cereal::XMLInputArchive archive(file);
		std::string encodedEngineDLLPath;
		std::vector<std::string> encodedScenePaths;
		archive(encodedEngineDLLPath, encodedScenePaths);
		engineDLLPath = utf8_decode(encodedEngineDLLPath);
		openScenes.resize(encodedScenePaths.size());
		for (std::size_t i = 0; i < encodedScenePaths.size(); i++) {
			openScenes[i] = utf8_decode(encodedScenePaths[i]);
		}
	}
	catch (...) {
		std::cout << "ProjectSettings::read::ERROR" << std::endl;
		return false;
	}

	return true;
}
/* Writes the current settings to path */
bool ProjectSettings::write(const wchar_t* path) {
	std::ofstream file(path);
	cereal::XMLOutputArchive archive(file);
	std::string encodedEngineDLLPath = utf8_encode(engineDLLPath);
	std::vector<std::string> encodedScenePaths(openScenes.size());
	for (std::size_t i = 0; i < openScenes.size(); i++) {
		encodedScenePaths[i] = utf8_encode(openScenes[i]);
	}
	archive(encodedEngineDLLPath, encodedScenePaths);

	return true;
}

ProjectSettings ProjectSettings::create(const wchar_t* name, const wchar_t* path) {
	ProjectSettings settings;
	if (!std::filesystem::create_directory(path)) {
		std::wcout << L"ProjectSettings::create::ERROR Failed to create directory at path: " << path << std::endl;
		return settings;
	}
	settings.name = name;
	settings.path = path;
	settings.save();
	return settings;
}

ProjectSettings ProjectSettings::load(const wchar_t* name, const wchar_t* path) {
	ProjectSettings settings;
	settings.name = name;
	settings.path = path;
	if (settings.read(settings.getFilePath().c_str())) {
		return settings;
	}
	return ProjectSettings();
}

bool ProjectSettings::save() {
	std::wcout << L"Saving project to " << getFilePath() << std::endl;
	if (write(getFilePath().c_str())) {
		std::cout << "Saved project successfully" << std::endl;
		return true;
	}
	else {
		std::cout << "Failed to save project" << std::endl;
		return false;
	}
}

bool ProjectSettings::isLoaded() const {
	return !path.empty();
}

std::wstring ProjectSettings::getName() const {
	return name;
}
std::wstring ProjectSettings::getPath() const {
	return path.ends_with(L"\\") || path.ends_with(L"/") ? path.substr(0, path.size() - 1) : path;
}
std::wstring ProjectSettings::getFilePath() const {
	return getPath().append(L"\\").append(name).append(L".proj");
}

void ProjectSettings::setEngineDLLPath(std::wstring path) {
	engineDLLPath = path;
}

std::wstring ProjectSettings::getEngineDLLPath() const {
	std::wstring result = engineDLLPath;
	parsePath(result);
	return result;
}

std::vector<std::wstring>& ProjectSettings::getOpenScenes() {
	return openScenes;
}

void ProjectSettings::addOpenScene(std::wstring path) {
	openScenes.push_back(path);
}

void ProjectSettings::removeOpenScene(std::wstring path) {
	auto it = std::find(openScenes.begin(), openScenes.end(), path);
	if (it != openScenes.end()) {
		openScenes.erase(it);
	}
}

void ProjectSettings::clearOpenScenes() {
	openScenes.clear();
}

std::wstring ProjectSettings::getAssetDirPath() const {
	return getPath().append(L"/Assets");
}