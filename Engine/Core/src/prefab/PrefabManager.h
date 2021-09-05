#ifndef PREFAB_MANAGER_H
#define PREFAB_MANAGER_H

#include "entity/Entity.h"
#include "entity/handle/Handle.h"
#include "entity/component/IComponentData.h"
#include "entity/component/ComponentGroup.h"
#include "components/RectTransform.h"
#include "components/entity/PrefabComponent.h"
#include <cstddef>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <queue>

/* Manages all prefabs */
namespace Core {

	class EntityManager;
	class EntityHandle;
	class SerializerArchive;
	class DeserializerArchive;
	class PrefabDeserializerArchive;
	typedef std::vector<std::pair<std::string, std::size_t>> EntityRemapLoadInfo;
	typedef std::vector<std::pair<std::size_t, std::string>> EntityRemapSaveInfo;

	struct PrefabContent {
		EntityRemapLoadInfo entityRemapInfo;
		std::vector<Handle> content; // Handles to the immediate children of the prefab root
		bool success = false;
	};

	class PrefabManager {
	public:
		PrefabManager(EntityManager* entityManager);
		~PrefabManager();

	public:
		void tick();

		/* Updates all Entities connected to the prefab. Returns true if successful */
		bool updatePrefab(std::string filePath);
		/* Updates the given prefab. Returns true if successful */
		bool updatePrefab(Handle rootPrefabHandle);
		/* Removes the given entities connection to the prefab */
		bool unpackPrefab(Handle rootPrefabHandle);
		/* Saves the Entity to the specified file path as the root of a prefab. Note: Will not convert the specified Entity to a prefab */
		bool savePrefab(std::string filePath, Handle rootHandle, EntityRemapSaveInfo& entityRemapInfo);
		/* Creates a prefab from an existing entity. Saves the prefab to the specified file path. Returns true if successful */
		bool createPrefabFromEntity(Handle entityHandle, std::string filePath);
		/* Creates an Entity from prefab. By default gives the root Entity a transform. */
		Handle createEntityFromPrefab(std::string filePath, float x = 0, float y = 0);

		/* Removes all overrides for the children of the given root handle */
		bool clearOverrides(Handle rootHandle);
		bool overrideProperty(Handle owner, std::size_t typeID, std::string propertyName);
		bool removePropertyOverride(Handle owner, std::size_t typeID, std::string propertyName);
		bool overrideComponent(Handle owner, std::size_t typeID);
		bool removeComponentOverride(Handle owner, std::size_t typeID);

		Handle getPrefabOverrideReceiver(Handle entity);
		bool isEntityPrefabRoot(Handle entity);
		/* Checks the closest PrefabComponent in the parent chain. Returns true if Entity is not connected/managed by the prefab. */
		bool isEntityAnOverride(Handle entity);
		/* Returns all property overrides for the given Entity. Retrieves overrides from PrefabComponents from the whole parent chain */
		std::vector<PrefabPropertyOverride> getPropertyOverrides(Handle entity, std::size_t typeID);
		/* Returns all component overrides for the given Entity. Retrieves overrides from PrefabComponents from the whole parent chain */
		std::vector<PrefabComponentOverride> getComponentOverrides(Handle entity);
		/* Returns all property overrides at the level of the given Entity. Retrieves overrides from PrefabComponents upwards through the whole parent chain */
		std::vector<PrefabPropertyOverride> getPropertyOverridesAt(Handle handle);
		/* Returns all component overrides at the level of the given Entity. Retrieves overrides from PrefabComponents upwards through the whole parent chain */
		std::vector<PrefabComponentOverride> getComponentOverridesAt(Handle handle);

	private:
		/* Loads the prefab at the specified file path */
		PrefabContent loadPrefab(std::string filePath, Entity rootEntity);
		/* Serializes the prefab and all its children to ostream */
		static void serialize(Handle rootEntity, SerializerArchive& archive);
		/* Deserializes the prefab and all children from istream*/
		static Handle deserialize(DeserializerArchive& archive, std::queue<EntityHandle>& entityHandles);
		Handle deserializeAndUpdate(PrefabDeserializerArchive& archive, std::queue<EntityHandle>& entityHandles, std::vector<PrefabPropertyOverride>& propertyOverrides, std::vector<PrefabComponentOverride>& componentOverrides);

		/*
		* @param rootHandle - an entity handle to the root of the prefab
		* @param prevEntityRemapInfo - previous remap info since last update
		* @param archive - deserializer archive
		*/
		EntityRemapLoadInfo updatePrefabFromRoot(Handle rootPrefabHandle, PrefabComponent& prefabComponent, PrefabDeserializerArchive& archive);

	private:
		ComponentGroup<RectTransform, PrefabComponent> prefabEntities;
		EntityManager* entityManager;
	};
}
#endif