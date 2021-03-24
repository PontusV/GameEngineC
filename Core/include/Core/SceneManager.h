#pragma once
#include "dllexport.h"
#include "Scene.h"

namespace Core {

	typedef void (*SceneAddedCallback)(void* ptr, IScene*);
	typedef void (*SceneRemovedCallback)(void* ptr, IScene*);

	extern "C" class DLLEXPORT ISceneManager {
	public:
		/* Removes all Scenes. Unloads all entities and components they contain. */
		virtual void clear() = 0;

		/* Creates a new Scene and loads it */
		virtual IScene* createIScene(const wchar_t* name) = 0;
		/* Retrieves a Scene by name */
		virtual IScene* getIScene(const wchar_t* name) = 0;
		/* Retrieves all loaded Scenes */
		virtual void getAllIScenes(IScene** out, std::size_t count) = 0;
		/* Retrieves the count of all loaded Scenes */
		virtual std::size_t getAllScenesCount() = 0;
		/* Saves scene to file */
		virtual bool saveScene(IScene* scene, const wchar_t* filePath = nullptr) = 0;
		/* Loads scene from file */
		virtual IScene* loadIScene(const wchar_t* filePath) = 0;
		/* Unloads scene */
		virtual bool unloadScene(const wchar_t* name) = 0;
		/* Sets the file path of the specified scene */
		virtual void setSceneFilePath(IScene* scene, const wchar_t* filePath) = 0;

		/* Calls the given callback whenever a scene has been added */
		virtual void setSceneAddedCallback(SceneAddedCallback callback) = 0;
		/* Calls the given callback whenever a scene has been removed */
		virtual void setSceneRemovedCallback(SceneRemovedCallback callback) = 0;
		/* Provides this ptr as an argument in the callback functions */
		virtual void setCallbackPtr(void* ptr) = 0;
	};
}