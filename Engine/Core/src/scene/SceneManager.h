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
	typedef void* (*SceneAddedCallbackFun)(void*, std::size_t);
	typedef void* (*SceneRemovedCallbackFun)(void*, std::size_t);

	class SceneManager {
	public:
		SceneManager(EntityManager* entityManager);
		~SceneManager();
		/* Removes all Scenes. Unloads all entities and components they contain. */
		void clear();
		/* Updates all scenes */
		void update();

		/* Creates a new Scene and loads it */
		ScenePtr createScene(std::wstring name);
		/* Retrieves a Scene by name */
		ScenePtr getScene(std::wstring name);
		/* Retrieves a Scene by index */
		ScenePtr getScene(std::size_t index);
		/* Retrieves a Scene index by name */
		std::size_t getSceneIndex(std::wstring name);
		/* Retrieves all loaded Scenes */
		std::vector<ScenePtr> getAllScenes();
		/* Retrieves the count of all loaded Scenes */
		std::size_t getAllScenesCount();
		/* Retrieves all loaded Scenes and their names. */
		//std::vector<std::pair<std::wstring, ScenePtr>> getAllScenesAsPairs();
		/* Saves scene to file */
		bool saveScene(ScenePtr scene, const wchar_t* filePath = nullptr);
		/* Saves scene to file */
		bool saveScene(std::size_t sceneIndex, const wchar_t* filePath = nullptr);
		/* Loads scene from file */
		ScenePtr loadScene(const wchar_t* filePath);
		/* Unloads scene by name */
		bool unloadScene(std::wstring name);
		/* Unloads scene by index */
		bool unloadScene(std::size_t sceneIndex);
		/* Returns the full path of the scene */
		const wchar_t* getSceneFilePath(std::size_t sceneIndex);

		bool setSceneFilePath(Scene* scene, const wchar_t* filePath);
		/* Used by DLL interface. Calls the given callback whenever a scene has been added */
		void setSceneAddedCallback(SceneAddedCallbackFun callback);
		/* Used by DLL interface. Calls the given callback whenever a scene has been removed */
		void setSceneRemovedCallback(SceneRemovedCallbackFun callback);
		/* Used by DLL interface. Provides this ptr as an argument in the callback functions */
		void setCallbackPtr(void* ptr);

	private:
		std::vector<ScenePtr> scenes;
		std::map<ScenePtr, std::wstring> scenePathMap;
		EntityManager* entityManager;

		SceneAddedCallbackFun sceneAddedCallback;
		SceneRemovedCallbackFun sceneRemovedCallback;
		void* callbackPtr; // A pointer sent to the callback functions
	};
}
#endif