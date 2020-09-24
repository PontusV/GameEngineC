#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include <string>
#include <map>
#include <memory>
#include <vector>
namespace Core {
	class EntityManager;
	class Scene;
	typedef std::shared_ptr<Scene> ScenePtr;

	class SceneManager {
	public:
		SceneManager(EntityManager* entityManager);
		~SceneManager();
		/* Removes all Scenes. Unloads all entities and components they contain. */
		void clear();

		/* Creates a new Scene and loads it */
		ScenePtr createScene(std::wstring name);
		/* Retrieves a Scene by name */
		ScenePtr getScene(std::wstring name);
		/* Retrieves all loaded Scenes */
		std::vector<ScenePtr> getAllScenes();
		/* Retrieves all loaded Scenes and their names. */
		std::vector<std::pair<std::wstring, ScenePtr>> getAllScenesAsPairs();
		/* Saves scene to file */
		bool saveScene(ScenePtr scene, const wchar_t* filePath = nullptr);
		/* Loads scene from file */
		ScenePtr loadScene(const wchar_t* filePath);

		void setSceneFilePath(ScenePtr scene, const wchar_t* filePath);

		void update();

	private:
		std::map<std::wstring, ScenePtr> sceneMap;
		std::map<std::wstring, std::wstring> scenePathMap;
		EntityManager* entityManager;
	};
}
#endif