#include "SceneManager.h"
#include "Scene.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <system_error>
using namespace Core;

SceneManager::SceneManager(EntityManager* entityManager) : entityManager(entityManager) {

}
SceneManager::~SceneManager() {
	clear();
}

void SceneManager::clear() {
	for (ScenePtr& scene : scenes) {
		scene->clear();
	}
	scenes.clear();
	scenePathMap.clear();
}


void SceneManager::update() {
	for (ScenePtr& scene : scenes) {
		scene->processQueue();
	}
}

bool SceneManager::saveScene(ScenePtr scene, const wchar_t* filePath, bool assignPath) {
	if (scene == nullptr) {
		std::cout << "SceneManager::saveScene::ERROR Failed to save scene because give pointer was nullptr" << std::endl;
		return false;
	}
	std::ofstream file;
	if (filePath) {
		std::wcout << L"Saving scene to " << filePath << std::endl;
		file.open(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
		if (assignPath) {
			scenePathMap.insert(std::pair(scene, std::wstring(filePath)));
		}
	} else {
		auto it = scenePathMap.find(scene);
		if (it == scenePathMap.end()) {
			// Error: Path not stored
			std::wcout << L"SceneManager::saveScene::ERROR Failed to save scene. Missing file path to scene" << std::endl;
			return false;
		}
		std::wcout << L"Saving scene to " << it->second << std::endl;
		file.open(it->second.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
	}

	// Scene
	scene->serialize(file);
	file.close();

	std::wcout << L"Successfully saved scene" << std::endl;
	return true;
}

bool SceneManager::saveScene(std::size_t sceneIndex, const wchar_t* filePath, bool assignPath) {
	ScenePtr scene = getScene(sceneIndex);
	return saveScene(scene, filePath, assignPath);
}

ScenePtr SceneManager::loadSceneImpl(const wchar_t* filePath, std::wstring name) {
	std::wcout << L"Loading Scene: " << filePath << std::endl;
	std::ifstream file;
	try {
		file.open(filePath, std::ios::in | std::ios::binary);
	}
	catch (std::system_error& e) {
		std::wcout << L"Failed to load Scene: " << filePath << ". File does not exist" << std::endl;
		return nullptr;
	}
	if (file.fail()) {
		std::wcout << L"Failed to load Scene: " << filePath << ". File does not exist" << std::endl;
		return nullptr;
	}

	// Scene
	ScenePtr scene = std::make_shared<Scene>(entityManager, name);
	scene->deserialize(file);
	file.close();
	scene->awake();
	return scene;
}

ScenePtr SceneManager::loadScene(const wchar_t* srcFilePath, const wchar_t* destFilePath) {
	// Retrieves scene name from dest file path
	std::wstring destPath = std::wstring(destFilePath);
	std::size_t nameStartIndex = destPath.find_last_of(L"\\");
	std::wstring fileName = destPath.substr(nameStartIndex == std::wstring::npos ? 0 : nameStartIndex + 1);
	std::wstring sceneName = fileName.substr(0, fileName.find_last_of(L"."));

	// TODO: Unload current scene when loading backup? Dunno
	for (ScenePtr& scene : scenes) {
		if (sceneName.compare(scene->getName()) == 0) {
			return scene;
		}
	}

	// Scene
	ScenePtr scene = loadSceneImpl(srcFilePath, sceneName);
	if (scene == nullptr) {
		return nullptr;
	}

	scenes.push_back(scene);
	scenePathMap.insert(std::pair(scene, std::wstring(destFilePath)));
	if (sceneAddedCallback) sceneAddedCallback(callbackPtr, scenes.size() - 1);
	return scene;
}

ScenePtr SceneManager::loadScene(const wchar_t* filePath) {
	std::wstring path = std::wstring(filePath);
	std::size_t nameStartIndex = path.find_last_of(L"\\");
	std::wstring fileName = path.substr(nameStartIndex == std::wstring::npos ? 0 : nameStartIndex + 1);
	std::wstring sceneName = fileName.substr(0, fileName.find_last_of(L"."));

	for (ScenePtr& scene : scenes) {
		if (sceneName.compare(scene->getName()) == 0) {
			return scene;
		}
	}

	// Scene
	ScenePtr scene = loadSceneImpl(filePath, sceneName);
	if (scene == nullptr) {
		return nullptr;
	}

	scenes.push_back(scene);
	scenePathMap.insert(std::pair(scene, std::wstring(filePath)));
	if (sceneAddedCallback) sceneAddedCallback(callbackPtr, scenes.size() - 1);
	return scene;
}

bool SceneManager::unloadScene(std::wstring name) {
	std::wcout << L"Unloading Scene: " << name << std::endl;
	std::size_t sceneIndex = getSceneIndex(name);
	unloadScene(sceneIndex);
}

bool SceneManager::unloadScene(std::size_t index) {
	ScenePtr scene = getScene(index);
	if (scene == nullptr) {
		std::wcout << L"Failed to unload scene. Scene name not found in sceneMap" << std::endl;
		return false;
	}
	scene->clear();
	scenes.erase(scenes.begin() + index);
	if (sceneRemovedCallback) sceneRemovedCallback(callbackPtr, index);
	auto scenePathIt = scenePathMap.find(scene);
	if (scenePathIt == scenePathMap.end()) {
		std::wcout << L"Failed to unload scene. Scene name not found in scenePathMap" << std::endl;
		return false;
	}
	scenePathMap.erase(scenePathIt);
	return true;
}

ScenePtr SceneManager::createScene(std::wstring name) {
	ScenePtr scene = std::make_shared<Scene>(entityManager, name);
	scenes.push_back(scene);
	if (sceneAddedCallback) sceneAddedCallback(callbackPtr, scenes.size() - 1);
	return scene;
}

ScenePtr SceneManager::getScene(std::wstring name) {
	if (std::size_t sceneIndex = getSceneIndex(name) != -1) {
		return getScene(sceneIndex);
	}
	return nullptr;
}

ScenePtr SceneManager::getScene(std::size_t index) {
	if (index > scenes.size() - 1) {
		std::cout << "SceneManager::getScene::ERROR Out of bounds. Index: " << index << ", Scenes size: " << scenes.size() << std::endl;
		return nullptr;
	}
	return scenes[index];
}

std::size_t SceneManager::getSceneIndex(std::wstring name) {
	for (std::size_t i = 0; i < scenes.size(); i++) {
		if (name.compare(scenes[i]->getName()) == 0) {
			return i;
		}
	}
	return -1;
}

std::vector<ScenePtr> SceneManager::getAllScenes() {
	return scenes;
}

std::size_t SceneManager::getAllScenesCount() {
	return scenes.size();
}

bool SceneManager::setSceneFilePath(Scene* scene, const wchar_t* filePath) {
	if (scene == nullptr) return false;
	scenePathMap.insert(std::pair(scene, std::wstring(filePath)));
	return true;
}

void SceneManager::setSceneAddedCallback(SceneAddedCallbackFun callback) {
	sceneAddedCallback = callback;
}

void SceneManager::setSceneRemovedCallback(SceneRemovedCallbackFun callback) {
	sceneRemovedCallback = callback;
}

void SceneManager::setCallbackPtr(void* ptr) {
	callbackPtr = ptr;
}

const wchar_t* SceneManager::getSceneFilePath(std::size_t sceneIndex) {
	ScenePtr scene = getScene(sceneIndex);
	if (scene == nullptr) return nullptr;
	auto it = scenePathMap.find(scene);
	if (it != scenePathMap.end()) {
		return it->second.c_str();
	}
	return nullptr;
}