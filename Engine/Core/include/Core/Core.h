#pragma once
#include "dllexport.h"
#include <cstddef>

#define MAX_FIELD_COUNT 10

typedef std::size_t EntityID;
typedef std::size_t TypeID;
typedef void* (*SceneAddedCallbackFun)(void*, std::size_t);
typedef void* (*SceneRemovedCallbackFun)(void*, std::size_t);
typedef void* (*GLADloadproc)(const char* name);

namespace Core {
	class Engine;
}

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

struct PropertyFieldData {
	std::size_t count;
	void* ptrBuffer[MAX_FIELD_COUNT];
};

extern "C" {
	// Engine
	DLLEXPORT Core::Engine* createEngine();
	DLLEXPORT void releaseEngine(Core::Engine* engine);
	DLLEXPORT bool createTemplateEntity(Core::Engine* engine, std::size_t sceneIndex, const char* name, float x, float y, float width, float height);
	DLLEXPORT bool engineInit(Core::Engine* engine, GLADloadproc ptr, int screenWidth, int screenHeight);
	DLLEXPORT void engineTick(Core::Engine* engine, float deltaTime);
	DLLEXPORT void getCameraPosition(Core::Engine* engine, float* out);
	DLLEXPORT void setCameraPosition(Core::Engine* engine, float x, float y);
	DLLEXPORT void setViewportSize(Core::Engine* engine, float width, float height);
	DLLEXPORT EntityID getEntityAtPos(Core::Engine* engine, float x, float y);

	// Transform / RectTransform
	DLLEXPORT bool getRectSize(Core::Engine* engine, EntityID entityID, float* out);
	DLLEXPORT bool getMinRectScreenPosition(Core::Engine* engine, EntityID entityID, float* out);
	DLLEXPORT bool getWorldPosition(Core::Engine* engine, EntityID entityID, float* out);
	DLLEXPORT bool getLocalPosition(Core::Engine* engine, EntityID entityID, float* out);
	DLLEXPORT bool setLocalPosition(Core::Engine* engine, EntityID entityID, float x, float y);
	DLLEXPORT bool setWorldPosition(Core::Engine* engine, EntityID entityID, float x, float y);

	// Reflection general
	DLLEXPORT bool hasAnnotation(TypeID typeID, const char* annotation);
	DLLEXPORT void getTypeName(TypeID typeID, char* out, std::size_t outSize);
	DLLEXPORT void onUpdate(void* instance, TypeID typeID, std::size_t propIndex);
	DLLEXPORT void getDerivedTypeIDs(TypeID typeID, TypeID* out, std::size_t outSize);
	DLLEXPORT void getAllReflectedTypes(TypeID* out, std::size_t outSize);
	DLLEXPORT std::size_t getDerivedTypeIDsCount(TypeID typeID);
	DLLEXPORT std::size_t getAllReflectedTypesCount();
	
	// Reflection properties
	DLLEXPORT std::size_t getPropertiesCount(TypeID typeID);
	DLLEXPORT std::size_t getPropertyID(TypeID typeID, std::size_t propIndex);
	DLLEXPORT std::size_t getPropertyType(TypeID typeID, std::size_t propIndex);
	DLLEXPORT std::size_t getPropertyFieldCount(TypeID typeID, std::size_t propIndex, void* instance);
	DLLEXPORT void getPropertyFields(TypeID typeID, std::size_t propIndex, void* instance, void** out, std::size_t outSize);
	DLLEXPORT void getPropertyName(TypeID typeID, std::size_t propIndex, char* out, std::size_t outSize);
	DLLEXPORT void getPropertyTypeName(TypeID typeID, std::size_t propIndex, char* out, std::size_t outSize);
	
	// Scene
	DLLEXPORT bool loadScene(Core::Engine* engine, const char* path);
	DLLEXPORT bool unloadScene(Core::Engine* engine, std::size_t sceneIndex);
	DLLEXPORT bool saveScene(Core::Engine* engine, std::size_t sceneIndex);
	DLLEXPORT bool createScene(Core::Engine* engine, const char* name, const char* path);
	DLLEXPORT bool destroyEntity(Core::Engine* engine, std::size_t sceneIndex, EntityID entityID);
	DLLEXPORT bool getAllEntities(Core::Engine* engine, std::size_t sceneIndex, EntityID* out, std::size_t outSize);
	DLLEXPORT bool getSceneName(Core::Engine* engine, std::size_t sceneIndex, char* out, std::size_t outSize);
	DLLEXPORT bool getSceneFilePath(Core::Engine* engine, std::size_t sceneIndex, char* out, std::size_t outSize);
	DLLEXPORT bool hasSceneChanged(Core::Engine* engine, std::size_t sceneIndex);
	DLLEXPORT std::size_t getSceneCount(Core::Engine* engine);
	DLLEXPORT std::size_t getAllEntitiesCount(Core::Engine* engine, std::size_t sceneIndex);
	DLLEXPORT void setSceneAddedCallback(Core::Engine* engine, SceneAddedCallbackFun fun);
	DLLEXPORT void setSceneRemovedCallback(Core::Engine* engine, SceneRemovedCallbackFun fun);
	DLLEXPORT void setSceneCallbackPtr(Core::Engine* engine, void* ptr);
	
	// Entity
	DLLEXPORT bool addComponent(Core::Engine* engine, EntityID entityID, std::size_t sceneIndex, TypeID typeID);
	DLLEXPORT bool removeComponent(Core::Engine* engine, EntityID entityID, std::size_t sceneIndex, TypeID typeID);
	DLLEXPORT bool renameEntity(Core::Engine* engine, EntityID entityID, const char* name);
	DLLEXPORT void getComponents(Core::Engine* engine, EntityID entityID, void** outPtrs, TypeID* outTypeIDs, std::size_t outSize);
	DLLEXPORT bool isEntityChild(Core::Engine* engine, EntityID entityID, EntityID parentID);
	DLLEXPORT bool setEntityParent(Core::Engine* engine, std::size_t sceneIndex, EntityID entityID, EntityID parentID);
	DLLEXPORT bool isEntityNameAvailable(Core::Engine* engine, const char* name);
	DLLEXPORT bool hasEntityParent(Core::Engine* engine, EntityID entityID);
	DLLEXPORT bool detachEntityParent(Core::Engine* engine, std::size_t sceneIndex, EntityID entityID);
	DLLEXPORT void getEntityName(Core::Engine* engine, EntityID entityID, char* out, std::size_t outSize);
	DLLEXPORT EntityID getEntityChild(Core::Engine* engine, EntityID entityID, std::size_t index);
	DLLEXPORT std::size_t getComponentsCount(Core::Engine* engine, EntityID entityID);
	DLLEXPORT std::size_t getEntityChildCount(Core::Engine* engine, EntityID entityID);
	DLLEXPORT std::size_t getEntityImmediateChildCount(Core::Engine* engine, EntityID entityID);
}