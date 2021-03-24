#include "SceneManager.h"
#include "Scene.h"
#include <iostream>
#include <fstream>
#include <utility>
using namespace Core;

SceneManager::SceneManager(EntityManager* entityManager) : entityManager(entityManager) {

}
SceneManager::~SceneManager() {
	clear();
}

void SceneManager::clear() {
	for (auto it = sceneMap.begin(); it != sceneMap.end(); it++) {
		it->second->clear();
	}
	sceneMap.clear();
	scenePathMap.clear();
}


void SceneManager::update() {
	for (auto it = sceneMap.begin(); it != sceneMap.end(); it++) {
		it->second->processQueue();
	}
}

bool SceneManager::saveScene(ScenePtr scene, const wchar_t* filePath) { //To be added: file location of scene map (currently hard coded to the map Scenes)
	std::ofstream file;
	if (filePath) {
		std::wcout << L"Saving scene to " << filePath << std::endl;
		file.open(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
		scenePathMap.insert(std::pair(scene->getName(), std::wstring(filePath)));
	} else {
		auto it = scenePathMap.find(scene->getName());
		if (it == scenePathMap.end()) {
			// Error: Path not stored
			std::wcout << L"Failed to save scene. Missing file path to scene" << std::endl;
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

ScenePtr SceneManager::loadScene(const wchar_t* filePath) {
	std::ifstream file;
	std::wstring path = std::wstring(filePath);
	std::size_t nameStartIndex = path.find_last_of(L"\\");
	std::wstring fileName = path.substr(nameStartIndex == std::wstring::npos ? 0 : nameStartIndex + 1);
	std::wstring sceneName = fileName.substr(0, fileName.find_last_of(L"."));
	auto it = sceneMap.find(sceneName);
	if (it != sceneMap.end()) {
		return it->second;
	}

	std::wcout << L"Loading Scene: " << filePath << std::endl;
	file.open(filePath, std::ios::in | std::ios::binary);

	// Scene
	ScenePtr scene = std::make_shared<Scene>(entityManager, sceneName);
	scene->deserialize(file);
	file.close();
	scene->awake();

	sceneMap[sceneName] = scene;
	scenePathMap.insert(std::pair(scene->getName(), std::wstring(filePath)));
	if (sceneAddedCallback) sceneAddedCallback(callbackPtr, scene.get());
	return scene;
}

bool SceneManager::unloadScene(std::wstring name) {
	return unloadScene(name.c_str());
}

ScenePtr SceneManager::createScene(std::wstring name) {
	ScenePtr scene = std::make_shared<Scene>(entityManager, name);
	sceneMap[name] = scene;
	if (sceneAddedCallback) sceneAddedCallback(callbackPtr, scene.get());
	return scene;
}

ScenePtr SceneManager::getScene(std::wstring name) {
	return sceneMap[name];
}

std::vector<ScenePtr> SceneManager::getAllScenes() {
	std::vector<ScenePtr> scenes;
	for (auto it = sceneMap.begin(); it != sceneMap.end(); it++) {
		scenes.push_back(it->second);
	}
	return scenes;
}

std::size_t SceneManager::getAllScenesCount() {
	return sceneMap.size();
}

std::vector<std::pair<std::wstring, ScenePtr>> SceneManager::getAllScenesAsPairs() {
	std::vector<std::pair<std::wstring, ScenePtr>> scenes;
	for (auto it = sceneMap.begin(); it != sceneMap.end(); it++) {
		scenes.push_back(*it);
	}
	return scenes;
}

void SceneManager::setSceneFilePath(IScene* scene, const wchar_t* filePath) {
	scenePathMap.insert(std::pair(scene->getName(), std::wstring(filePath)));
}

IScene* SceneManager::loadIScene(const wchar_t* filePath) {
	return loadScene(filePath).get();
}

IScene* SceneManager::createIScene(const wchar_t* name) {
	return createScene(name).get();
}

IScene* SceneManager::getIScene(const wchar_t* name) {
	return getScene(name).get();
}

bool SceneManager::unloadScene(const wchar_t* name) {
	std::wcout << L"Unloading Scene: " << name << std::endl;
	auto sceneIt = sceneMap.find(name);
	if (sceneIt == sceneMap.end()) {
		std::wcout << L"Failed to unload scene. Scene name not found in sceneMap" << std::endl;
		return false;
	}
	ScenePtr scene = sceneIt->second;
	if (sceneRemovedCallback) sceneRemovedCallback(callbackPtr, scene.get());
	scene->clear();
	sceneMap.erase(sceneIt);
	auto scenePathIt = scenePathMap.find(name);
	if (scenePathIt == scenePathMap.end()) {
		std::wcout << L"Failed to unload scene. Scene name not found in scenePathMap" << std::endl;
		return false;
	}
	scenePathMap.erase(scenePathIt);
	return true;
}

bool SceneManager::saveScene(IScene* scene, const wchar_t* filePath) {
	std::ofstream file;
	if (filePath) {
		std::wcout << L"Saving scene to " << filePath << std::endl;
		file.open(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
		scenePathMap.insert(std::pair(scene->getName(), std::wstring(filePath)));
	}
	else {
		auto it = scenePathMap.find(scene->getName());
		if (it == scenePathMap.end()) {
			// Error: Path not stored
			std::wcout << L"Failed to save scene. Missing file path to scene" << std::endl;
			return false;
		}
		std::wcout << L"Saving scene to " << it->second << std::endl;
		file.open(it->second.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
	}

	// Scene
	reinterpret_cast<Scene*>(scene)->serialize(file); // TODO: Make this prettier
	file.close();

	std::wcout << L"Successfully saved scene" << std::endl;
	return true;
}

void SceneManager::getAllIScenes(IScene** out, std::size_t count) {
	std::vector<ScenePtr> scenes = getAllScenes();
	std::size_t size = std::min(count, scenes.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = static_cast<IScene*>(scenes[i].get());
	}
}

void SceneManager::setSceneAddedCallback(SceneAddedCallback callback) {
	sceneAddedCallback = callback;
}

void SceneManager::setSceneRemovedCallback(SceneRemovedCallback callback) {
	sceneRemovedCallback = callback;
}

void SceneManager::setCallbackPtr(void* ptr) {
	callbackPtr = ptr;
}