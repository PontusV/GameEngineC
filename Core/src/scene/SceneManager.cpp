#include "SceneManager.h"
#include "Scene.h"
#include <iostream>
#include <fstream>
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
}


void SceneManager::update() {
	for (auto it = sceneMap.begin(); it != sceneMap.end(); it++) {
		it->second->processQueue();
	}
}

void SceneManager::saveScene(ScenePtr scene, const char* fileName) { //To be added: file location of scene map (currently hard coded to the map Scenes)
	std::ofstream file;
	std::string sceneDir("Scenes/");
	sceneDir.append(fileName);
	file.open(sceneDir, std::ios::out | std::ios::binary | std::ios::trunc);

	// Scene name
	std::string name = scene->getName();
	file.write(name.c_str(), sizeof(char) * (name.size() + 1));
	// Scene
	scene->serialize(file);
	file.close();

	std::cout << "Saved scene to: " << sceneDir << "\n";
}

ScenePtr SceneManager::loadScene(const char* fileName) { //To be added: file location of scene map (currently hard coded to the map Scenes)
	std::ifstream file;
	std::string sceneDir("sceneDir/");
	sceneDir.append(fileName);
	std::cout << "Loading Scene: " << sceneDir << "\n";
	file.open(sceneDir, std::ios::in | std::ios::binary);

	// Scene name
	std::string name;
	std::getline(file, name, '\0');
	// Scene
	ScenePtr scene = std::make_shared<Scene>(entityManager, name);
	scene->deserialize(file);
	file.close();
	scene->awake();

	return scene;
}

ScenePtr SceneManager::createScene(std::string name) {
	ScenePtr scene = std::make_shared<Scene>(entityManager, name);
	sceneMap[name] = scene;
	return scene;
}

ScenePtr SceneManager::getScene(std::string name) {
	return sceneMap[name];
}

std::vector<ScenePtr> SceneManager::getAllScenes() {
	std::vector<ScenePtr> scenes;
	for (auto it = sceneMap.begin(); it != sceneMap.end(); it++) {
		scenes.push_back(it->second);
	}
	return scenes;
}

std::vector<std::pair<std::string, ScenePtr>> SceneManager::getAllScenesAsPairs() {
	std::vector<std::pair<std::string, ScenePtr>> scenes;
	for (auto it = sceneMap.begin(); it != sceneMap.end(); it++) {
		scenes.push_back(*it);
	}
	return scenes;
}