#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include <Core/SceneManager.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
namespace Core {
	class EntityManager;
	class Scene;
	typedef std::shared_ptr<Scene> ScenePtr;

	class SceneManager: public ISceneManager {
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
		/* Retrieves the count of all loaded Scenes */
		std::size_t getAllScenesCount();
		/* Retrieves all loaded Scenes and their names. */
		std::vector<std::pair<std::wstring, ScenePtr>> getAllScenesAsPairs();
		/* Saves scene to file */
		bool saveScene(ScenePtr scene, const wchar_t* filePath = nullptr);
		/* Loads scene from file */
		ScenePtr loadScene(const wchar_t* filePath);
		/* Unloads scene */
		bool unloadScene(std::wstring name);
		/* Returns the full path of the scene */
		const wchar_t* getSceneFilePath(const wchar_t* name);

		void update();

		/* Used by DLL interface */
		void setSceneFilePath(IScene* scene, const wchar_t* filePath) override;
		/* Used by DLL interface */
		bool unloadScene(const wchar_t* name) override;
		/* Used by DLL interface */
		bool saveScene(IScene* scene, const wchar_t* filePath = nullptr) override;
		/* Used by DLL interface */
		void getAllIScenes(IScene** out, std::size_t count) override;
		/* Used by DLL interface */
		IScene* loadIScene(const wchar_t* filePath) override;
		/* Used by DLL interface */
		IScene* createIScene(const wchar_t* name) override;
		/* Used by DLL interface */
		IScene* getIScene(const wchar_t* name) override;
		/* Used by DLL interface. Calls the given callback whenever a scene has been added */
		void setSceneAddedCallback(SceneAddedCallback callback);
		/* Used by DLL interface. Calls the given callback whenever a scene has been removed */
		void setSceneRemovedCallback(SceneRemovedCallback callback);
		/* Used by DLL interface. Provides this ptr as an argument in the callback functions */
		void setCallbackPtr(void* ptr);

	private:
		std::map<std::wstring, ScenePtr> sceneMap;
		std::map<std::wstring, std::wstring> scenePathMap;
		EntityManager* entityManager;

		SceneAddedCallback sceneAddedCallback;
		SceneRemovedCallback sceneRemovedCallback;
		void* callbackPtr; // A pointer sent to the callback functions
	};
}
#endif