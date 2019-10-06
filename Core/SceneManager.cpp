#include "SceneManager.h"
#include "Scene.h"
#include <iostream>
#include <fstream>
using namespace Core;

SceneManager::SceneManager(EntityManager* entityManager) : entityManager(entityManager) {

}
SceneManager::~SceneManager() {
	// Unload all entities and components
	for (auto it = sceneMap.begin(); it != sceneMap.end(); it++) {
		it->second->clear();
	}
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
	//
	ScenePtr scene = std::make_shared<Scene>(entityManager, ObjectType::World);
	scene->deserialize(file);
	//
	file.close();
	scene->awake();

	return scene;
}

ScenePtr SceneManager::createScene(std::string name, ObjectType type) { // TODO: Link name to Scene
	ScenePtr scene = std::make_shared<Scene>(entityManager, type);
	sceneMap[name] = scene;
	return scene;
}

ScenePtr SceneManager::getScene(std::string name) {
	return sceneMap[name];
}