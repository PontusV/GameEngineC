#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include "ObjectData.h"
#include <string>
#include <map>
#include <memory>
namespace Core {
	class EntityManager;
	class Scene;
	typedef std::shared_ptr<Scene> ScenePtr;

	class SceneManager {
	public:
		SceneManager(EntityManager* entityManager);
		~SceneManager();
		/* Creates a new Scene and loads it */
		ScenePtr createScene(std::string name, ObjectType type);
		/* Retrieves a Scene by name */
		ScenePtr getScene(std::string name);
		/* Retrieves all loaded Scenes */
		std::vector<ScenePtr> getAllScenes();
		/* Retrieves all loaded Scenes and their names. */
		std::vector<std::pair<std::string, ScenePtr>> getAllScenesAsPairs();
		/* Saves scene to file */
		void saveScene(ScenePtr scene, const char* fileName);
		/* Loads scene from file */
		ScenePtr loadScene(const char* fileName);

		void update();

	private:
		std::map<std::string, ScenePtr> sceneMap;
		EntityManager* entityManager;
	};
}
#endif