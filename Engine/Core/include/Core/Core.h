#pragma once
#include "dllexport.h"
#include <cstddef>

#define MAX_FIELD_COUNT 100

typedef std::size_t EntityID;
typedef std::size_t TypeID;
typedef void (*EntitiesChangedCallbackFun)(void*, std::size_t);
typedef void* (*GLADloadproc)(const char* name);

namespace Core {
	class Engine;
}

extern "C" {
	// Engine
	DLLEXPORT Core::Engine* createEngine();
	DLLEXPORT void releaseEngine(Core::Engine* engine);
	DLLEXPORT bool engineInit(Core::Engine* engine, GLADloadproc ptr, int screenWidth, int screenHeight);
	DLLEXPORT void engineTick(Core::Engine* engine, float deltaTime);
	DLLEXPORT void engineEditorTick(Core::Engine* engine, float deltaTime);
	DLLEXPORT void setViewportSize(Core::Engine* engine, float width, float height);
	DLLEXPORT void setAssetDirPath(const char* path);
	DLLEXPORT bool loadGameState(Core::Engine* engine, const char* path);
	DLLEXPORT bool saveGameState(Core::Engine* engine, const char* path);
	DLLEXPORT EntityID createTemplateEntity(Core::Engine* engine, const char* name, float x, float y, float width, float height);

	// Camera
	DLLEXPORT void setCameraPosition(Core::Engine* engine, float x, float y);
	DLLEXPORT void setCameraRotation(Core::Engine* engine, float scale);
	DLLEXPORT void setCameraScale(Core::Engine* engine, float scale);
	DLLEXPORT void getCameraPosition(Core::Engine* engine, float* out);
	DLLEXPORT float getCameraRotation(Core::Engine* engine);
	DLLEXPORT float getCameraScale(Core::Engine* engine);

	// Transform / RectTransform
	DLLEXPORT bool getRectSize(Core::Engine* engine, EntityID entityID, float* out);
	DLLEXPORT bool getMinRectScreenPosition(Core::Engine* engine, EntityID entityID, float* out);
	DLLEXPORT bool getWorldPosition(Core::Engine* engine, EntityID entityID, float* out);
	DLLEXPORT bool getLocalPosition(Core::Engine* engine, EntityID entityID, float* out);
	DLLEXPORT bool setLocalPosition(Core::Engine* engine, EntityID entityID, float x, float y);
	DLLEXPORT bool setWorldPosition(Core::Engine* engine, EntityID entityID, float x, float y);
	DLLEXPORT bool overridePosition(Core::Engine* engine, EntityID entityID);
	DLLEXPORT bool removePositionOverride(Core::Engine* engine, EntityID entityID);
	DLLEXPORT bool isPositionOverriden(Core::Engine* engine, EntityID entityID);

	// Reflection general
	DLLEXPORT bool hasAnnotation(TypeID typeID, const char* annotation);
	DLLEXPORT void getTypeName(TypeID typeID, char* out, std::size_t outSize);
	DLLEXPORT void onUpdate(void* instance, TypeID typeID, std::size_t propIndex);
	DLLEXPORT void getDerivedTypeIDs(TypeID typeID, TypeID* out, std::size_t outSize);
	DLLEXPORT void getAllReflectedTypes(TypeID* out, std::size_t outSize);
	DLLEXPORT TypeID getTypeIDFromName(const char* name);
	DLLEXPORT TypeID getPrefabComponentTypeID();
	DLLEXPORT std::size_t getDerivedTypeIDsCount(TypeID typeID);
	DLLEXPORT std::size_t getAllReflectedTypesCount();
	
	// Reflection properties
	DLLEXPORT std::size_t getPropertiesCount(TypeID typeID);
	DLLEXPORT void getPropertyInspectorData(TypeID typeID, std::size_t propIndex, void* instance, char* bufferPtr, std::size_t bufferSize);
	DLLEXPORT void setPropertyFieldValue(TypeID typeID, std::size_t propIndex, void* instance, std::size_t fieldIndex, char* buffer, std::size_t bufferSize);
	DLLEXPORT std::size_t getPropertyIndex(TypeID typeID, const char* propertyName);
	DLLEXPORT void getPropertyName(TypeID typeID, std::size_t propIndex, char* buffer, std::size_t bufferSize);
	
	// Prefab
	DLLEXPORT bool unpackPrefab(Core::Engine* engine, EntityID entityID);
	DLLEXPORT bool savePrefab(Core::Engine* engine, EntityID entityID, const char* path);
	DLLEXPORT bool updatePrefabs(Core::Engine* engine, const char* path);
	DLLEXPORT bool updatePrefab(Core::Engine* engine, EntityID entityID);
	DLLEXPORT bool createPrefabFromEntity(Core::Engine* engine, EntityID entityID, const char* path);
	DLLEXPORT bool overrideProperty(Core::Engine* engine, EntityID entityID, std::size_t typeID, std::size_t propIndex);
	DLLEXPORT bool removePropertyOverride(Core::Engine* engine, EntityID entityID, std::size_t typeID, std::size_t propIndex);
	DLLEXPORT bool isEntityPrefabRoot(Core::Engine* engine, EntityID entityID);
	DLLEXPORT bool isEntityAnOverride(Core::Engine* engine, EntityID entityID);
	DLLEXPORT void getPrefabFilePath(Core::Engine* engine, EntityID entityID, char* out, std::size_t outSize);
	DLLEXPORT void getPropertyOverrides(Core::Engine* engine, EntityID entityID, TypeID typeID, std::size_t* out, std::size_t outSize);
	DLLEXPORT void getPropertyOverridesAt(Core::Engine* engine, EntityID entityID, EntityID* entityIDOut, TypeID* typeIDOut, std::size_t* propIndexOut, std::size_t outSize);
	DLLEXPORT void getComponentOverrides(Core::Engine* engine, EntityID entityID, TypeID* out, std::size_t outSize);
	DLLEXPORT bool revertPrefab(Core::Engine* engine, EntityID entityID);
	DLLEXPORT EntityID createPrefabEntity(Core::Engine* engine, const char* path, float x, float y);
	DLLEXPORT EntityID createEntityFromPrefab(Core::Engine* engine, const char* path, float x, float y);
	DLLEXPORT EntityID getPrefabOverrideReceiver(Core::Engine* engine, EntityID entityID);
	DLLEXPORT std::size_t getPropertyOverridesCount(Core::Engine* engine, EntityID entityID, TypeID typeID);
	DLLEXPORT std::size_t getPropertyOverridesAtCount(Core::Engine* engine, EntityID entityID);
	DLLEXPORT std::size_t getComponentOverridesCount(Core::Engine* engine, EntityID entityID);

	// Serialization
	DLLEXPORT bool loadPropertyFromBuffer(Core::Engine* engine, EntityID entityID, TypeID typeID, char* serializedData, std::size_t dataSize);
	DLLEXPORT bool loadComponentFromBuffer(Core::Engine* engine, EntityID entityID, char* serializedData, std::size_t dataSize);
	DLLEXPORT bool loadEntityFromBuffer(Core::Engine* engine, char* serializedData, std::size_t dataSize);
	DLLEXPORT std::size_t writePropertyToBuffer(Core::Engine* engine, EntityID entityID, TypeID typeID, std::size_t propIndex, char* bufferPtr, std::size_t bufferSize);
	DLLEXPORT std::size_t writeComponentToBuffer(Core::Engine* engine, EntityID entityID, TypeID typeID, char* bufferPtr, std::size_t bufferSize);
	DLLEXPORT std::size_t writeEntityToBuffer(Core::Engine* engine, EntityID entityID, char* bufferPtr, std::size_t bufferSize);
	
	// Entity
	DLLEXPORT bool addComponent(Core::Engine* engine, EntityID entityID, TypeID typeID);
	DLLEXPORT bool removeComponent(Core::Engine* engine, EntityID entityID, TypeID typeID);
	DLLEXPORT bool hasComponent(Core::Engine* engine, EntityID entityID, TypeID typeID);
	DLLEXPORT bool isEntityChild(Core::Engine* engine, EntityID entityID, EntityID parentID);
	DLLEXPORT bool setEntityParent(Core::Engine* engine, EntityID entityID, EntityID parentID);
	DLLEXPORT bool hasEntityParent(Core::Engine* engine, EntityID entityID);
	DLLEXPORT bool detachEntityParent(Core::Engine* engine, EntityID entityID);
	DLLEXPORT bool destroyEntity(Core::Engine* engine, EntityID entityID);
	DLLEXPORT bool getAllEntities(Core::Engine* engine, EntityID* out, std::size_t outSize);
	DLLEXPORT bool setEntityName(Core::Engine* engine, EntityID entityID, const char* name);
	DLLEXPORT void getEntityName(Core::Engine* engine, EntityID entityID, char* out, std::size_t outSize);
	DLLEXPORT void getComponents(Core::Engine* engine, EntityID entityID, void** outPtrs, TypeID* outTypeIDs, std::size_t outSize);
	DLLEXPORT EntityID createEntity(Core::Engine* engine);
	DLLEXPORT EntityID getEntityAtPos(Core::Engine* engine, float x, float y);
	DLLEXPORT EntityID getEntityParent(Core::Engine* engine, EntityID entityID);
	DLLEXPORT EntityID getEntityChild(Core::Engine* engine, EntityID entityID, std::size_t index);
	DLLEXPORT std::size_t getComponentsCount(Core::Engine* engine, EntityID entityID);
	DLLEXPORT std::size_t getEntityChildCount(Core::Engine* engine, EntityID entityID);
	DLLEXPORT std::size_t getEntityImmediateChildCount(Core::Engine* engine, EntityID entityID);
	DLLEXPORT std::size_t getAllEntitiesCount(Core::Engine* engine);

	// Callback
	DLLEXPORT void setEntitiesChangedCallback(Core::Engine* engine, EntitiesChangedCallbackFun callback);
	DLLEXPORT void setCallbackPtr(Core::Engine* engine, void* ptr);
}