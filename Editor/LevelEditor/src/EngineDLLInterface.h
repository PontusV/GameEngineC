#ifndef ENGINE_DLL_INTERFACE_H
#define ENGINE_DLL_INTERFACE_H
#include <cstddef>
#include <string>
#include <vector>
#include <array>
#include "imgui/imgui.h"

#define DLL_INTERFACE_READ_BUFFER_SIZE 100000

typedef std::size_t EntityID;
typedef std::size_t TypeID;
typedef void* EnginePtr;
typedef void (*EntitiesChangedCallbackFun)(void*, std::size_t);
typedef void* (*GLADloadproc)(const char* name);

// Engine
typedef void* (*CreateEngineFun)();
typedef void* (*ReleaseEngineFun)(EnginePtr);
typedef bool (*EngineInitFun)(EnginePtr, GLADloadproc, int, int);
typedef void (*EngineTickFun)(EnginePtr, float);
typedef void (*EngineEditorTickFun)(EnginePtr, float);
typedef void (*SetViewportSizeFun)(EnginePtr, float, float);
typedef void (*SetAssetDirPathFun)(const char*);
typedef bool (*LoadGameStateFun)(EnginePtr engine, const char*);
typedef bool (*SaveGameStateFun)(EnginePtr engine, const char*);
typedef EntityID (*CreateTemplateEntityFun)(EnginePtr, const char*, float, float, float, float);

// Camera
typedef void (*SetCameraPositionFun)(EnginePtr, float, float);
typedef void (*SetCameraRotationFun)(EnginePtr, float);
typedef void (*SetCameraScaleFun)(EnginePtr, float);
typedef void (*GetCameraPositionFun)(EnginePtr, float*);
typedef float (*GetCameraRotationFun)(EnginePtr);
typedef float (*GetCameraScaleFun)(EnginePtr);

// Transform / RectTransform
typedef bool (*GetRectSizeFun)(EnginePtr, EntityID, float*);
typedef bool (*GetMinRectScreenPositionFun)(EnginePtr, EntityID, float*);
typedef bool (*GetWorldPositionFun)(EnginePtr, EntityID, float*);
typedef bool (*GetLocalPositionFun)(EnginePtr, EntityID, float*);
typedef bool (*SetLocalPositionFun)(EnginePtr, EntityID, float, float);
typedef bool (*SetWorldPositionFun)(EnginePtr, EntityID, float, float);
typedef bool (*OverridePositionFun)(EnginePtr, EntityID);
typedef bool (*RemovePositionOverrideFun)(EnginePtr, EntityID);
typedef bool (*IsPositionOverridenFun)(EnginePtr, EntityID);

// Reflection general
typedef bool (*HasAnnotationFun)(TypeID, const char*);
typedef void (*GetTypeNameFun)(TypeID, char*, std::size_t);
typedef void (*OnUpdateFun)(void*, TypeID, std::size_t);
typedef void (*GetDerivedTypeIDsFun)(TypeID, TypeID*, std::size_t);
typedef void (*GetAllReflectedTypesFun)(TypeID*, std::size_t);
typedef TypeID (*GetTypeIDFromNameFun)(const char*);
typedef TypeID (*GetPrefabComponentTypeIDFun)();
typedef std::size_t(*GetDerivedTypeIDsCountFun)(TypeID);
typedef std::size_t(*GetAllReflectedTypesCountFun)();

// Reflection properties
typedef std::size_t(*GetPropertiesCountFun)(TypeID);
typedef std::size_t(*GetPropertyIndexFun)(TypeID, const char*);
typedef void(*GetPropertyNameFun)(TypeID, std::size_t, char*, std::size_t);
typedef void(*GetPropertyInspectorDataFun)(TypeID, std::size_t, void*, char*, std::size_t);
typedef void(*SetPropertyFieldValueFun)(TypeID, std::size_t, void*, std::size_t, char*, std::size_t);

// Prefab
typedef bool (*UnpackPrefabFun)(EnginePtr, EntityID);
typedef bool (*SavePrefabFun)(EnginePtr, EntityID, const char*);
typedef bool (*UpdatePrefabsFun)(EnginePtr, const char*);
typedef bool (*UpdatePrefabFun)(EnginePtr, EntityID);
typedef bool (*ClearPropertyOverridesFun)(EnginePtr, EntityID, TypeID);
typedef bool (*CreatePrefabFromEntityFun)(EnginePtr, EntityID, const char*);
typedef bool (*OverridePropertyFun)(EnginePtr, EntityID, std::size_t, std::size_t);
typedef bool (*RemovePropertyOverrideFun)(EnginePtr, EntityID, std::size_t, std::size_t);
typedef bool (*RemoveDestroyedEntityOverrideFun)(EnginePtr, EntityID, const char*);
typedef bool (*IsEntityPrefabRootFun)(EnginePtr, EntityID);
typedef bool (*IsEntityAnOverrideFun)(EnginePtr, EntityID);
typedef bool (*RevertPrefabFun)(EnginePtr, EntityID);
typedef void (*GetPrefabFilePathFun)(EnginePtr, EntityID, char*, std::size_t);
typedef void (*GetPropertyOverridesFun)(EnginePtr, EntityID, TypeID, std::size_t*, std::size_t);
typedef void (*GetPropertyOverridesAtFun)(EnginePtr, EntityID, EntityID*, TypeID*, std::size_t*, std::size_t);
typedef void (*GetComponentOverridesFun)(EnginePtr, EntityID, TypeID*, std::size_t);
typedef EntityID (*CreatePrefabEntityFun)(EnginePtr, const char*, float, float);
typedef EntityID (*CreateEntityFromPrefabFun)(EnginePtr, const char*, float, float);
typedef EntityID (*GetPrefabOverrideReceiverFun)(EnginePtr, EntityID);
typedef std::size_t (*GetDestroyedEntityOverridesFun)(EnginePtr, EntityID, char*, std::size_t);
typedef std::size_t (*GetPropertyOverridesCountFun)(EnginePtr, EntityID, TypeID);
typedef std::size_t (*GetPropertyOverridesAtCountFun)(EnginePtr, EntityID);
typedef std::size_t (*GetComponentOverridesCountFun)(EnginePtr, EntityID);

// Serialization
typedef bool (*LoadPropertyFromBufferFun)(EnginePtr, EntityID, TypeID, const char*, std::size_t);
typedef bool (*LoadComponentFromBufferFun)(EnginePtr, EntityID, const char*, std::size_t);
typedef bool (*LoadEntityFromBufferFun)(EnginePtr, const char*, std::size_t);
typedef std::size_t(*WritePropertyToBufferFun)(EnginePtr, EntityID, TypeID, std::size_t, char*, std::size_t);
typedef std::size_t(*WriteComponentToBufferFun)(EnginePtr, EntityID, TypeID, char*, std::size_t);
typedef std::size_t(*WriteEntityToBufferFun)(EnginePtr, EntityID, char*, std::size_t);

// Entity
typedef bool (*OverrideParentComponentFun)(EnginePtr, EntityID);
typedef bool (*RemoveParentComponentOverrideFun)(EnginePtr, EntityID);
typedef bool (*IsParentComponentOverridenFun)(EnginePtr, EntityID);
typedef bool (*DestroyEntityFun)(EnginePtr, EntityID);
typedef bool (*AddComponentFun)(EnginePtr, EntityID, TypeID);
typedef bool (*RemoveComponentFun)(EnginePtr, EntityID, TypeID);
typedef bool (*HasComponentFun)(EnginePtr, EntityID, TypeID);
typedef bool (*SetEntityNameFun)(EnginePtr, EntityID, const char*);
typedef bool (*IsEntityChildFun)(EnginePtr, EntityID, EntityID);
typedef bool (*SetEntityParentFun)(EnginePtr, EntityID, EntityID);
typedef bool (*HasEntityParentFun)(EnginePtr, EntityID);
typedef bool (*DetachEntityParentFun)(EnginePtr, EntityID);
typedef bool (*GetAllEntitiesFun)(EnginePtr, EntityID*, std::size_t);
typedef void (*GetComponentsFun)(EnginePtr, EntityID, void**, TypeID*, std::size_t);
typedef void (*GetEntityNameFun)(EnginePtr, EntityID, char*, std::size_t);
typedef std::size_t(*GetComponentsCountFun)(EnginePtr, EntityID);
typedef std::size_t(*GetEntityChildCountFun)(EnginePtr, EntityID);
typedef std::size_t(*GetEntityImmediateChildCountFun)(EnginePtr, EntityID);
typedef std::size_t(*GetAllEntitiesCountFun)(EnginePtr);
typedef EntityID(*GetEntityParentFun)(EnginePtr, EntityID);
typedef EntityID(*GetEntityChildFun)(EnginePtr, EntityID, std::size_t);
typedef EntityID(*CreateEntityFun)(EnginePtr, const char*);
typedef EntityID(*GetEntityAtPosFun)(EnginePtr, float, float);

// Callback
typedef void (*SetEntitiesChangedCallbackFun)(EnginePtr, EntitiesChangedCallbackFun);
typedef void (*SetCallbackPtrFun)(EnginePtr, void*);

namespace Editor {
	enum class InspectorFieldType {
		NONE = 0,
		BOOL = 1,
		DECIMAL = 2,
		SIGNED_NUMBER = 3,
		UNSIGNED_NUMBER = 4,
		STRING = 5,
		WIDE_STRING = 6,
		IMAGE_PATH = 7,
		SHADER_PATH = 8,
		FONT_PATH = 9
	};
	struct ReflectedTypeData {
		std::size_t typeID;
		std::string typeName;
	};
	struct ComponentData {
		TypeID typeID;
		void* instance;
	};
	struct PropertyField {
		void* ptr;
		std::string name;
		std::size_t typeSize;
		InspectorFieldType type;
	};
	struct PropertyData {
		std::size_t index;
		std::string name;
		bool overriden;
		std::size_t arraySize;
		std::string typeName;
		std::vector<PropertyField> fields;
	};
	struct ComponentOverride {
		EntityID entityID;
		TypeID typeID;
	};
	struct PropertyOverride {
		EntityID entityID;
		TypeID typeID;
		std::string propertyName;
	};

	class EngineDLLInterface {
	public:
		// Engine
		EnginePtr createEngine();
		void releaseEngine();
		bool engineInit(GLADloadproc ptr, int screenWidth, int screenHeight);
		bool loadGameState(const char* path);
		bool saveGameState(const char* path);
		void engineTick(float deltaTime);
		void engineEditorTick(float deltaTime);
		void setViewportSize(float width, float height);
		void setAssetDirPath(const char* path);
		EntityID createTemplateEntity(const char* name, float x, float y, float width, float height);
		EntityID getEntityAtPos(float x, float y);

		// Camera
		void setCameraPosition(float x, float y);
		void setCameraRotation(float scale);
		void setCameraScale(float scale);
		ImVec2 getCameraPosition();
		float getCameraRotation();
		float getCameraScale();

		// Transform / RectTransform
		ImVec2 getRectSize(EntityID entityID);
		ImVec2 getMinRectScreenPosition(EntityID entityID);
		ImVec2 getWorldPosition(EntityID entityID);
		ImVec2 getLocalPosition(EntityID entityID);
		bool setLocalPosition(EntityID entityID, float x, float y);
		bool setWorldPosition(EntityID entityID, float x, float y);
		bool overridePosition(EntityID entityID);
		bool removePositionOverride(EntityID entityID);
		bool isPositionOverriden(EntityID entityID);

		// Reflection general
		bool hasAnnotation(TypeID typeID, const char* annotation);
		std::string getTypeName(TypeID typeID);
		void onUpdate(void* instance, TypeID typeID, std::size_t propIndex);
		std::vector<TypeID> getDerivedTypeIDs(TypeID typeID);
		std::vector<ReflectedTypeData> getAllReflectedTypes();
		TypeID getTypeIDFromName(const char* name);
		TypeID getPrefabComponentTypeID();
		std::size_t getDerivedTypeIDsCount(TypeID typeID);
		std::size_t getAllReflectedTypesCount();

		// Reflection properties
		std::size_t getPropertiesCount(TypeID typeID);
		std::size_t getPropertyIndex(TypeID typeID, const char* propName);
		std::string getPropertyName(TypeID typeID, std::size_t propIndex);
		void getPropertyInspectorData(TypeID typeID, std::size_t propIndex, void* instance, char* buffer, std::size_t bufferSize);
		void setPropertyFieldValue(TypeID typeID, std::size_t propIndex, void* instance, std::size_t fieldIndex, char* buffer, std::size_t bufferSize);
		std::vector<PropertyData> getProperties(EntityID entityID, TypeID typeID, void* instance);

		// Prefab
		bool unpackPrefab(EntityID entityID);
		bool savePrefab(EntityID entityID, const char* path);
		bool updatePrefabs(const char* path);
		bool updatePrefab(EntityID entityID);
		bool clearPropertyOverrides(EntityID entityID, TypeID typeID);
		bool createPrefabFromEntity(EntityID entityID, const char* path);
		bool overrideProperty(EntityID entityID, std::size_t typeID, std::size_t propIndex);
		bool removePropertyOverride(EntityID entityID, std::size_t typeID, std::size_t propIndex);
		bool removeDestroyedEntityOverride(EntityID entityID, const char* guid);
		bool isEntityPrefabRoot(EntityID entityID);
		bool isEntityAnOverride(EntityID entityID);
		bool isComponentOverriden(EntityID entityID, TypeID typeID);
		bool isPropertyOverriden(EntityID entityID, TypeID typeID, std::string propertyName);
		bool revertPrefab(EntityID entityID);
		std::string getPrefabFilePath(EntityID entityID);
		std::vector<PropertyOverride> getPropertyOverrides(EntityID entityID, TypeID typeID);
		std::vector<PropertyOverride> getPropertyOverridesAt(EntityID entityID);
		std::vector<ComponentOverride> getComponentOverrides(EntityID entityID);
		std::vector<std::string> getDestroyedEntityOverrides(EntityID entityID);
		EntityID getNearestPrefabRootEntityID(EntityID entityID);
		EntityID createPrefabEntity(const char* path, float x, float y);
		EntityID createEntityFromPrefab(const char* path, float x, float y);
		EntityID getPrefabOverrideReceiver(EntityID entityID);
		std::size_t getPropertyOverridesCount(EntityID entityID, TypeID typeID);
		std::size_t getPropertyOverridesAtCount(EntityID entityID);
		std::size_t getComponentOverridesCount(EntityID entityID);

		// Serialization
		bool loadPropertyFromBuffer(EntityID entityID, TypeID typeID, const char* serializedData, std::size_t dataSize);
		bool loadComponentFromBuffer(EntityID entityID, const char* serializedData, std::size_t dataSize);
		bool loadEntityFromBuffer(const char* serializedData, std::size_t dataSize);
		std::string writePropertyToBuffer(EntityID entityID, TypeID typeID, std::size_t propIndex);
		std::string writeComponentToBuffer(EntityID entityID, TypeID typeID);
		std::string writeEntityToBuffer(EntityID entityID);

		// Entity
		bool overrideParentComponent(EntityID entityID);
		bool removeParentComponentOverride(EntityID entityID);
		bool isParentComponentOverriden(EntityID entityID);
		bool destroyEntity(EntityID entityID);
		bool addComponent(EntityID entityID, TypeID typeID);
		bool removeComponent(EntityID entityID, TypeID typeID);
		bool hasComponent(EntityID entityID, TypeID typeID);
		bool setEntityName(EntityID entityID, const char* name);
		std::vector<ComponentData> getComponents(EntityID entityID);
		ComponentData getComponent(EntityID entityID, TypeID typeID);
		bool isEntityChild(EntityID entityID, EntityID parentID);
		bool setEntityParent(EntityID entityID, EntityID parentID);
		bool hasEntityParent(EntityID entityID);
		bool detachEntityParent(EntityID entityID);
		std::string getEntityName(EntityID entityID);
		EntityID createEntity(const char* name);
		EntityID getEntityParent(EntityID entityID);
		EntityID getEntityChild(EntityID entityID, std::size_t index);
		EntityID getRootEntityID(EntityID entityID);
		std::vector<EntityID> getAllEntities();
		std::size_t getComponentsCount(EntityID entityID);
		std::size_t getEntityChildCount(EntityID entityID);
		std::size_t getEntityImmediateChildCount(EntityID entityID);
		std::size_t getAllEntitiesCount();

		// Callback
		void setEntitiesChangedCallback(EntitiesChangedCallbackFun fun);
		void setCallbackPtr(void* ptr);

	protected:
		// Function ptrs
		// Engine
		CreateEngineFun createEngineFun;
		ReleaseEngineFun releaseEngineFun;
		CreateTemplateEntityFun createTemplateEntityFun;
		EngineInitFun engineInitFun;
		EngineTickFun engineTickFun;
		EngineEditorTickFun engineEditorTickFun;
		SetViewportSizeFun setViewportSizeFun;
		SetAssetDirPathFun setAssetDirPathFun;
		LoadGameStateFun loadGameStateFun;
		SaveGameStateFun saveGameStateFun;

		// Camera
		SetCameraPositionFun setCameraPositionFun;
		SetCameraRotationFun setCameraRotationFun;
		SetCameraScaleFun setCameraScaleFun;
		GetCameraPositionFun getCameraPositionFun;
		GetCameraRotationFun getCameraRotationFun;
		GetCameraScaleFun getCameraScaleFun;

		// Transform / RectTransform
		GetRectSizeFun getRectSizeFun;
		GetMinRectScreenPositionFun getMinRectScreenPositionFun;
		GetWorldPositionFun getWorldPositionFun;
		GetLocalPositionFun getLocalPositionFun;
		SetLocalPositionFun setLocalPositionFun;
		SetWorldPositionFun setWorldPositionFun;
		OverridePositionFun overridePositionFun;
		RemovePositionOverrideFun removePositionOverrideFun;
		IsPositionOverridenFun isPositionOverridenFun;

		// Reflection general
		HasAnnotationFun hasAnnotationFun;
		GetTypeNameFun getTypeNameFun;
		OnUpdateFun onUpdateFun;
		GetDerivedTypeIDsFun getDerivedTypeIDsFun;
		GetAllReflectedTypesFun getAllReflectedTypesFun;
		GetTypeIDFromNameFun getTypeIDFromNameFun;
		GetPrefabComponentTypeIDFun getPrefabComponentTypeIDFun;
		GetDerivedTypeIDsCountFun getDerivedTypeIDsCountFun;
		GetAllReflectedTypesCountFun getAllReflectedTypesCountFun;

		// Reflection properties
		GetPropertiesCountFun getPropertiesCountFun;
		GetPropertyIndexFun getPropertyIndexFun;
		GetPropertyNameFun getPropertyNameFun;
		GetPropertyInspectorDataFun getPropertyInspectorDataFun;
		SetPropertyFieldValueFun setPropertyFieldValueFun;

		// Prefab
		UnpackPrefabFun unpackPrefabFun;
		SavePrefabFun savePrefabFun;
		UpdatePrefabsFun updatePrefabsFun;
		UpdatePrefabFun updatePrefabFun;
		ClearPropertyOverridesFun clearPropertyOverridesFun;
		CreatePrefabFromEntityFun createPrefabFromEntityFun;
		CreatePrefabEntityFun createPrefabEntityFun;
		CreateEntityFromPrefabFun createEntityFromPrefabFun;
		OverridePropertyFun overridePropertyFun;
		RemovePropertyOverrideFun removePropertyOverrideFun;
		IsEntityPrefabRootFun isEntityPrefabRootFun;
		IsEntityAnOverrideFun isEntityAnOverrideFun;
		GetPropertyOverridesFun getPropertyOverridesFun;
		GetPropertyOverridesAtFun getPropertyOverridesAtFun;
		GetComponentOverridesFun getComponentOverridesFun;
		RevertPrefabFun revertPrefabFun;
		GetPrefabFilePathFun getPrefabFilePathFun;
		GetPrefabOverrideReceiverFun getPrefabOverrideReceiverFun;
		RemoveDestroyedEntityOverrideFun removeDestroyedEntityOverrideFun;
		GetDestroyedEntityOverridesFun getDestroyedEntityOverridesFun;
		GetPropertyOverridesCountFun getPropertyOverridesCountFun;
		GetPropertyOverridesAtCountFun getPropertyOverridesAtCountFun;
		GetComponentOverridesCountFun getComponentOverridesCountFun;

		// Serialization
		LoadPropertyFromBufferFun loadPropertyFromBufferFun;
		LoadComponentFromBufferFun loadComponentFromBufferFun;
		LoadEntityFromBufferFun loadEntityFromBufferFun;
		WritePropertyToBufferFun writePropertyToBufferFun;
		WriteComponentToBufferFun writeComponentToBufferFun;
		WriteEntityToBufferFun writeEntityToBufferFun;

		// Entity
		OverrideParentComponentFun overrideParentComponentFun;
		RemoveParentComponentOverrideFun removeParentComponentOverrideFun;
		IsParentComponentOverridenFun isParentComponentOverridenFun;
		DestroyEntityFun destroyEntityFun;
		AddComponentFun addComponentFun;
		RemoveComponentFun removeComponentFun;
		HasComponentFun hasComponentFun;
		SetEntityNameFun setEntityNameFun;
		IsEntityChildFun isEntityChildFun;
		SetEntityParentFun setEntityParentFun;
		HasEntityParentFun hasEntityParentFun;
		DetachEntityParentFun detachEntityParentFun;
		GetAllEntitiesFun getAllEntitiesFun;
		GetComponentsFun getComponentsFun;
		GetEntityNameFun getEntityNameFun;
		GetComponentsCountFun getComponentsCountFun;
		GetEntityChildCountFun getEntityChildCountFun;
		GetEntityImmediateChildCountFun getEntityImmediateChildCountFun;
		GetAllEntitiesCountFun getAllEntitiesCountFun;
		GetEntityParentFun getEntityParentFun;
		GetEntityChildFun getEntityChildFun;
		CreateEntityFun createEntityFun;
		GetEntityAtPosFun getEntityAtPosFun;

		// Callback
		SetEntitiesChangedCallbackFun setEntitiesChangedCallbackFun;
		SetCallbackPtrFun setCallbackPtrFun;
	private:
		std::array<char, DLL_INTERFACE_READ_BUFFER_SIZE> readBuffer;
		EnginePtr engine;
	};
}
#endif