#ifndef ENGINE_DLL_H
#define ENGINE_DLL_H

#include <Windows.h>
#include <memory>
#include <cstddef>
#include <vector>
#include <string>
#include "imgui/imgui.h"

typedef std::size_t EntityID;
typedef std::size_t TypeID;
typedef void* EnginePtr;
typedef void (*SceneAddedCallbackFun)(void*, std::size_t);
typedef void (*SceneRemovedCallbackFun)(void*, std::size_t);
typedef void* (*GLADloadproc)(const char* name);

typedef void* (*CreateEngineFun)();
typedef void* (*ReleaseEngineFun)(void*);
typedef bool (*CreateTemplateEntityFun)(EnginePtr, std::size_t, const char*, float, float, float, float);
typedef bool (*EngineInitFun)(EnginePtr, GLADloadproc, int, int);
typedef void (*EngineTickFun)(EnginePtr, float);
typedef void (*GetCameraPositionFun)(EnginePtr, float*);
typedef void (*SetCameraPositionFun)(EnginePtr, float, float);
typedef void (*SetViewportSizeFun)(EnginePtr, float, float);
typedef EntityID (*GetEntityAtPosFun)(EnginePtr, float, float);

typedef bool (*GetRectSizeFun)(EnginePtr, EntityID, float*);
typedef bool (*GetMinRectScreenPositionFun)(EnginePtr, EntityID, float*);
typedef bool (*GetWorldPositionFun)(EnginePtr, EntityID, float*);
typedef bool (*GetLocalPositionFun)(EnginePtr, EntityID, float*);
typedef bool (*SetLocalPositionFun)(EnginePtr, EntityID, float, float);
typedef bool (*SetWorldPositionFun)(EnginePtr, EntityID, float, float);

typedef bool (*HasAnnotationFun)(TypeID, const char*);
typedef void (*GetTypeNameFun)(TypeID, char*, std::size_t);
typedef void (*OnUpdateFun)(void*, TypeID, std::size_t);
typedef void (*GetDerivedTypeIDsFun)(TypeID, TypeID*, std::size_t);
typedef void (*GetAllReflectedTypesFun)(TypeID*, std::size_t);
typedef std::size_t (*GetDerivedTypeIDsCountFun)(TypeID);
typedef std::size_t (*GetAllReflectedTypesCountFun)();

typedef std::size_t (*GetPropertiesCountFun)(TypeID);
typedef std::size_t (*GetPropertyIDFun)(TypeID, std::size_t);
typedef std::size_t (*GetPropertyTypeFun)(TypeID, std::size_t);
typedef std::size_t(*GetPropertyFieldCountFun)(TypeID, std::size_t, void*);
typedef void (*GetPropertyFieldsFun)(TypeID, std::size_t, void*, void**, std::size_t);
typedef void (*GetPropertyNameFun)(TypeID, std::size_t, char*, std::size_t);
typedef void (*GetPropertyTypeNameFun)(TypeID, std::size_t, char*, std::size_t);

typedef bool (*LoadSceneFun)(EnginePtr, const char*);
typedef bool (*UnloadSceneFun)(EnginePtr, std::size_t);
typedef bool (*SaveSceneFun)(EnginePtr, std::size_t);
typedef bool (*CreateSceneFun)(EnginePtr, const char*, const char*);
typedef bool (*DestroyEntityFun)(EnginePtr, std::size_t, EntityID);
typedef bool (*GetAllEntitiesFun)(EnginePtr, std::size_t, EntityID*, std::size_t);
typedef bool (*GetSceneNameFun)(EnginePtr, std::size_t, char*, std::size_t);
typedef bool (*GetSceneFilePathFun)(EnginePtr, std::size_t, char*, std::size_t);
typedef bool (*HasSceneChangedFun)(EnginePtr, std::size_t);
typedef std::size_t (*GetSceneCountFun)(EnginePtr);
typedef std::size_t (*GetAllEntitiesCountFun)(EnginePtr, std::size_t);
typedef void (*SetSceneAddedCallbackFun)(EnginePtr, SceneAddedCallbackFun);
typedef void (*SetSceneRemovedCallbackFun)(EnginePtr, SceneRemovedCallbackFun);
typedef void (*SetSceneCallbackPtrFun)(EnginePtr, void*);

typedef bool (*AddComponentFun)(EnginePtr, EntityID, std::size_t, TypeID);
typedef bool (*RemoveComponentFun)(EnginePtr, EntityID, std::size_t, TypeID);
typedef bool (*RenameEntityFun)(EnginePtr, EntityID, const char*);
typedef void (*GetComponentsFun)(EnginePtr, EntityID, void**, TypeID*, std::size_t);
typedef bool (*IsEntityChildFun)(EnginePtr, EntityID, EntityID);
typedef bool (*SetEntityParentFun)(EnginePtr, EntityID, EntityID);
typedef bool (*IsEntityNameAvailableFun)(EnginePtr, const char*);
typedef bool (*HasEntityParentFun)(EnginePtr, EntityID);
typedef bool (*DetachEntityParentFun)(EnginePtr, std::size_t, EntityID);
typedef void (*GetEntityNameFun)(EnginePtr, EntityID, char*, std::size_t);
typedef EntityID (*GetEntityChildFun)(EnginePtr, EntityID, std::size_t);
typedef std::size_t (*GetComponentsCountFun)(EnginePtr, EntityID);
typedef std::size_t (*GetEntityChildCountFun)(EnginePtr, EntityID);
typedef std::size_t (*GetEntityImmediateChildCountFun)(EnginePtr, EntityID);

namespace Editor {
	enum class InspectorFieldRenderType {
		NONE = 0,
		BOOL,
		DECIMAL,
		SIGNED_CHAR,
		UNSIGNED_CHAR,
		SIGNED_NUMBER,
		UNSIGNED_NUMBER,
		STRING,
		WIDE_STRING,
		IMAGE_PATH,
		SHADER_PATH,
		COLOR,
		VECTOR2,
		FONT
	};
	struct ReflectedTypeData {
		std::size_t typeID;
		std::string typeName;
	};
	struct ReflectedPropertyData {
		std::size_t index;
		std::size_t id;
		std::string name;
		std::string typeName;
		InspectorFieldRenderType renderer;
		std::vector<void*> fieldPtrs;
	};
	struct ComponentData {
		TypeID typeID;
		void* instance;
	};

	class EngineDLLInterface {
	public:
		// Engine
		EnginePtr createEngine();
		void releaseEngine();
		bool createTemplateEntity(std::size_t sceneIndex, const char* name, float x, float y, float width, float height);
		bool engineInit(GLADloadproc ptr, int screenWidth, int screenHeight);
		void engineTick(float deltaTime);
		ImVec2 getCameraPosition();
		void setCameraPosition(float x, float y);
		void setViewportSize(float width, float height);
		EntityID getEntityAtPos(float x, float y);

		// Transform / RectTransform
		ImVec2 getRectSize(EntityID entityID);
		ImVec2 getMinRectScreenPosition(EntityID entityID);
		ImVec2 getWorldPosition(EntityID entityID);
		ImVec2 getLocalPosition(EntityID entityID);
		bool setLocalPosition(EntityID entityID, float x, float y);
		bool setWorldPosition(EntityID entityID, float x, float y);

		// Reflection general
		bool hasAnnotation(TypeID typeID, const char* annotation);
		std::string getTypeName(TypeID typeID);
		void onUpdate(void* instance, TypeID typeID, std::size_t propIndex);
		std::vector<TypeID> getDerivedTypeIDs(TypeID typeID);
		std::vector<ReflectedTypeData> getAllReflectedTypes();
		std::size_t getDerivedTypeIDsCount(TypeID typeID);
		std::size_t getAllReflectedTypesCount();

		// Reflection properties
		std::size_t getPropertiesCount(TypeID typeID);
		std::vector<ReflectedPropertyData> getProperties(TypeID typeID, void* instance);
		std::size_t getPropertyID(TypeID typeID, std::size_t propIndex);
		std::size_t getPropertyType(TypeID typeID, std::size_t propIndex);
		std::size_t getPropertyFieldCount(TypeID typeID, std::size_t propIndex, void* instance);
		std::vector<void*> getPropertyFields(TypeID typeID, std::size_t propIndex, void* instance);
		std::string getPropertyName(TypeID typeID, std::size_t propIndex);
		std::string getPropertyTypeName(TypeID typeID, std::size_t propIndex);

		// Scene
		bool loadScene(const char* path);
		bool unloadScene(std::size_t sceneIndex);
		bool saveScene(std::size_t sceneIndex);
		bool createScene(const char* name, const char* path);
		bool destroyEntity(std::size_t sceneIndex, EntityID entityID);
		std::vector<EntityID> getAllEntities(std::size_t sceneIndex);
		std::string getSceneName(std::size_t sceneIndex);
		std::string getSceneFilePath(std::size_t sceneIndex);
		bool hasSceneChanged(std::size_t sceneIndex);
		std::size_t getSceneCount();
		std::size_t getAllEntitiesCount(std::size_t sceneIndex);
		void setSceneAddedCallback(SceneAddedCallbackFun fun);
		void setSceneRemovedCallback(SceneRemovedCallbackFun fun);
		void setSceneCallbackPtr(void* ptr);

		// Entity
		bool addComponent(EntityID entityID, std::size_t sceneIndex, TypeID typeID);
		bool removeComponent(EntityID entityID, std::size_t sceneIndex, TypeID typeID);
		bool renameEntity(EntityID entityID, const char* name);
		std::vector<ComponentData> getComponents(EntityID entityID);
		bool isEntityChild(EntityID entityID, EntityID parentID);
		bool setEntityParent(std::size_t sceneIndex, EntityID entityID, EntityID parentID);
		bool isEntityNameAvailable(const char* name);
		bool hasEntityParent(EntityID entityID);
		bool detachEntityParent(std::size_t sceneIndex, EntityID entityID);
		std::string getEntityName(EntityID entityID);
		EntityID getEntityChild(EntityID entityID, std::size_t index);
		std::size_t getComponentsCount(EntityID entityID);
		std::size_t getEntityChildCount(EntityID entityID);
		std::size_t getEntityImmediateChildCount(EntityID entityID);

	protected:
		// Function ptrs
		CreateEngineFun createEngineFun;
		ReleaseEngineFun releaseEngineFun;
		CreateTemplateEntityFun createTemplateEntityFun;
		EngineInitFun engineInitFun;
		EngineTickFun engineTickFun;
		GetCameraPositionFun getCameraPositionFun;
		SetCameraPositionFun setCameraPositionFun;
		SetViewportSizeFun setViewportSizeFun;
		GetEntityAtPosFun getEntityAtPosFun;

		GetRectSizeFun getRectSizeFun;
		GetMinRectScreenPositionFun getMinRectScreenPositionFun;
		GetWorldPositionFun getWorldPositionFun;
		GetLocalPositionFun getLocalPositionFun;
		SetLocalPositionFun setLocalPositionFun;
		SetWorldPositionFun setWorldPositionFun;

		HasAnnotationFun hasAnnotationFun;
		GetTypeNameFun getTypeNameFun;
		OnUpdateFun onUpdateFun;
		GetDerivedTypeIDsFun getDerivedTypeIDsFun;
		GetAllReflectedTypesFun getAllReflectedTypesFun;
		GetDerivedTypeIDsCountFun getDerivedTypeIDsCountFun;
		GetAllReflectedTypesCountFun getAllReflectedTypesCountFun;

		GetPropertiesCountFun getPropertiesCountFun;
		GetPropertyIDFun getPropertyIDFun;
		GetPropertyTypeFun getPropertyTypeFun;
		GetPropertyFieldCountFun getPropertyFieldCountFun;
		GetPropertyFieldsFun getPropertyFieldsFun;
		GetPropertyNameFun getPropertyNameFun;
		GetPropertyTypeNameFun getPropertyTypeNameFun;

		LoadSceneFun loadSceneFun;
		UnloadSceneFun unloadSceneFun;
		SaveSceneFun saveSceneFun;
		CreateSceneFun createSceneFun;
		DestroyEntityFun destroyEntityFun;
		GetAllEntitiesFun getAllEntitiesFun;
		GetSceneNameFun getSceneNameFun;
		GetSceneFilePathFun getSceneFilePathFun;
		HasSceneChangedFun hasSceneChangedFun;
		GetSceneCountFun getSceneCountFun;
		GetAllEntitiesCountFun getAllEntitiesCountFun;
		SetSceneAddedCallbackFun setSceneAddedCallbackFun;
		SetSceneRemovedCallbackFun setSceneRemovedCallbackFun;
		SetSceneCallbackPtrFun setSceneCallbackPtrFun;

		AddComponentFun addComponentFun;
		RemoveComponentFun removeComponentFun;
		RenameEntityFun renameEntityFun;
		GetComponentsFun getComponentsFun;
		IsEntityChildFun isEntityChildFun;
		SetEntityParentFun setEntityParentFun;
		IsEntityNameAvailableFun isEntityNameAvailableFun;
		HasEntityParentFun hasEntityParentFun;
		DetachEntityParentFun detachEntityParentFun;
		GetEntityNameFun getEntityNameFun;
		GetEntityChildFun getEntityChildFun;
		GetComponentsCountFun getComponentsCountFun;
		GetEntityChildCountFun getEntityChildCountFun;
		GetEntityImmediateChildCountFun getEntityImmediateChildCountFun;
	private:
		EnginePtr engine;
	};
	
	class EngineDLL : public EngineDLLInterface {
	public:
		EngineDLL();
		~EngineDLL();

		/* Loads DLL at specified path. Expects path to be pointing towards an Engine DLL. Returns true if DLL has been loaded successfully */
		bool load(const wchar_t* path);
		/*
			MAKE SURE ALL ALLOCATED RESOURCES HAVE BEEN RELEASED BEFORE USE.
			Unloads the DLL. Releases allocated memory for the DLLInterface. Returns true if DLL has been unloaded successfully
		*/
		bool unload();
		/* Returns true if DLL has been loaded successfully */
		bool isLoaded() const;

		std::size_t getEntitySceneIndex(EntityID entityID);
	private:
		HINSTANCE handle;
	};
}
#endif