#include "PrefabManager.h"
#include "engine/Serializer.h"
#include "entity/EntitySerializer.h"
#include "entity/EntityManager.h"
#include "entity/handle/EntityHandle.h"
#include "entity/component/ComponentLoader.h"
#include "components/entity/ParentEntity.h"
#include "components/entity/PrefabComponent.h"
#include "components/Transform.h"
#include "utils/GUID.h"
#include "ReflectionPolymorph.generated.h"
#include <algorithm>
using namespace Core;

constexpr const char* PREFAB_ROOT_GUID = "PREFAB_ROOT";


PrefabManager::PrefabManager(EntityManager* entityManager) : entityManager(entityManager){}
PrefabManager::~PrefabManager() {}


/* Returns entities from root as Entities connected to a prefab. All components will count as connected. A child will not be added to the vector if it does not have a remap in the given entityRemapInfo */
std::vector<ConnectedEntity> getEntitiesFromRootAsConnected(Handle rootHandle, EntityRemapSaveInfo entityRemapInfo) {
	std::vector<ConnectedEntity> connectedEntities;
	std::size_t childCount = rootHandle.getChildCount();
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = rootHandle.getChild(i);
		auto it = std::find_if(entityRemapInfo.begin(), entityRemapInfo.end(), [&child](const std::pair<std::size_t, std::string>& pair) { return child.getEntity().getID() == pair.first; });
		if (it == entityRemapInfo.end()) continue;
		ConnectedEntity& connectedEntity = connectedEntities.emplace_back();
		auto componentTypes = child.getComponentTypes();
		connectedEntity.entity = child.getEntity();
		connectedEntity.guid = it->second;
		connectedEntity.componentTypeIDs.reserve(componentTypes.size());
		for (auto componentType : componentTypes) {
			connectedEntity.componentTypeIDs.push_back(componentType.type.getTypeID());
		}
	}
	return connectedEntities;
}

EntityRemapSaveInfo reverseEntityRemapInfo(EntityRemapLoadInfo entityRemapInfo) {
	EntityRemapSaveInfo result;
	std::for_each(entityRemapInfo.begin(), entityRemapInfo.end(),
		[&result](const std::pair<std::string, std::size_t>& p) {
			result.push_back(std::make_pair(p.second, p.first));
		});
	return result;
}

EntityRemapLoadInfo reverseEntityRemapInfo(EntityRemapSaveInfo entityRemapInfo) {
	EntityRemapLoadInfo result;
	std::for_each(entityRemapInfo.begin(), entityRemapInfo.end(),
		[&result](const std::pair<std::size_t, std::string>& p) {
			result.push_back(std::make_pair(p.second, p.first));
		});
	return result;
}

std::string createGUID(const std::vector<std::pair<std::size_t, std::string>>& map) {
	std::string guid = GUID::create();
	if (std::find_if(map.begin(), map.end(), [&guid](const std::pair<std::size_t, std::string>& pair) { return pair.second == guid; }) == map.end()) {
		return guid;
	}
	return createGUID(map);
}

/* Ensures validation of EntityRemapSaveInfo for the given Entity IDs. Creates new GUID for new Entities and removes unused remaps */
void ensureEntityRemapInfo(EntityRemapSaveInfo& entityRemapInfo, std::size_t rootEntityID, const std::vector<std::size_t>& entityIDs) {
	// Removes unused remaps
	for (auto it = entityRemapInfo.begin(); it != entityRemapInfo.end();) {
		if (it->first != rootEntityID && std::find(entityIDs.begin(), entityIDs.end(), it->first) == entityIDs.end()) {
			it = entityRemapInfo.erase(it);
		}
		else {
			it++;
		}
	}
	// Add new remaps
	bool containsRootEntityID = std::find_if(entityRemapInfo.begin(), entityRemapInfo.end(), [&rootEntityID](const std::pair<std::size_t, std::string>& pair) { return pair.first == rootEntityID; }) != entityRemapInfo.end();
	if (!containsRootEntityID) {
		entityRemapInfo.push_back(std::pair<std::size_t, std::string>(rootEntityID, std::string(PREFAB_ROOT_GUID)));
	}
	for (const std::size_t& entityID : entityIDs) {
		bool containsEntityID = std::find_if(entityRemapInfo.begin(), entityRemapInfo.end(), [&entityID](const std::pair<std::size_t, std::string>& pair) { return pair.first == entityID; }) != entityRemapInfo.end();
		if (!containsEntityID) {
			std::string entityGUID = createGUID(entityRemapInfo);
			entityRemapInfo.push_back(std::pair<std::size_t, std::string>(entityID, entityGUID));
		}
	}
	// Reorder the vector to match the order of the given entityIDs
	assert(entityRemapInfo.size() == entityIDs.size() + 1);
	auto rootIt = std::find_if(entityRemapInfo.begin(), entityRemapInfo.end(), [&rootEntityID](const std::pair<std::size_t, std::string>& pair) { return pair.first == rootEntityID; });
	if (rootIt != entityRemapInfo.end() && rootIt != entityRemapInfo.begin()) {
		std::iter_swap(rootIt, entityRemapInfo.begin());
	}
	if (entityIDs.size() > 1) {
		for (std::size_t i = 0; i < entityIDs.size() - 1; i++) {
			const std::size_t& entityID = entityIDs[i];
			auto it = std::find_if(entityRemapInfo.begin() + i + 2, entityRemapInfo.end(), [&entityID](const std::pair<std::size_t, std::string>& pair) { return pair.first == entityID; });
			if (it != entityRemapInfo.end()) {
				std::iter_swap(entityRemapInfo.begin() + i + 1, it);
			}
		}
	}
}

template<typename Archive>
bool serializeEntityRemapInfo(Handle rootHandle, EntityRemapSaveInfo& entityRemapInfo, Archive& archive) {
	if (!rootHandle.refresh()) {
		std::cout << "PrefabManager::serializeEntityRemapInfo::ERROR Invalid root handle" << std::endl;
		return false;
	}
	// Ensures valid entityRemapInfo
	std::size_t childCount = rootHandle.getChildCount();
	std::vector<std::size_t> entityIDs;
	entityIDs.reserve(childCount);
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = rootHandle.getChild(i);
		entityIDs.push_back(child.getEntity().getID());
	}
	ensureEntityRemapInfo(entityRemapInfo, rootHandle.getEntity().getID(), entityIDs);
	// Saves child count
	std::size_t mapSize = entityRemapInfo.size();
	if (mapSize != childCount + 1) {
		std::cout << "PrefabManager::serializeEntityRemapInfo::ERROR mapsize does not match childCount" << std::endl;
		throw std::runtime_error("PrefabManager::serializeEntityRemapInfo::ERROR mapsize does not match childCount");
	}
	archive(mapSize);
	// Saves GUIDs
	for (const auto& pair : entityRemapInfo) {
		std::string entityGUID = pair.second;
		archive(entityGUID);
	}
	return true;
}

template<typename Archive>
EntityRemapLoadInfo deserializeEntityRemapInfo(Archive& archive) {
	EntityRemapLoadInfo entityRemapInfo;
	std::size_t mapSize;
	archive(mapSize);
	// Loads GUIDs
	entityRemapInfo.reserve(mapSize);
	for (std::size_t i = 0; i < mapSize; i++) {
		std::string entityGUID;
		archive(entityGUID);
		entityRemapInfo.push_back(std::pair<std::string, std::size_t>(entityGUID, 0)); // 0 is temp
	}
	return entityRemapInfo;
}

void PrefabManager::tick() {
	std::size_t size = prefabEntities.size();
	for (std::size_t i = 0; i < size; i++) {
		PrefabComponent& prefabComponent = prefabEntities.get<PrefabComponent>(i);
		if (prefabComponent.isDirty()) {
			prefabComponent.setDirty(false);
			updatePrefab(prefabEntities.createHandle(i, entityManager));
		}
	}
}

bool PrefabManager::updatePrefab(Handle prefabHandle) {
	PrefabComponent* prefabComponent = prefabHandle.getComponent<PrefabComponent>();
	if (prefabComponent == nullptr) {
		std::cout << "PrefabManager::updatePrefab::ERROR Entity " << prefabHandle.getEntity().getID() << " is not a prefab root" << std::endl;
		return false;
	}

	auto filePath = prefabComponent->getFilePath();
	std::ifstream file;
	try {
		file.open(filePath, std::ios::in | std::ios::binary);
	}
	catch (std::system_error&) {
		std::cout << "PrefabManager::updatePrefab::ERROR Failed to load file: " << filePath << ". File does not exist" << std::endl;
		return false;
	}
	if (file.fail()) {
		std::cout << "PrefabManager::updatePrefab::ERROR Failed to load file: " << filePath << ". File open failed" << std::endl;
		return false;
	}
	PrefabDeserializerArchive archive(file, entityManager);
	auto connectedEntities = updatePrefabFromRoot(prefabHandle, *prefabComponent, archive);
	if (prefabComponent = prefabHandle.getComponent<PrefabComponent>()) {
		prefabComponent->setConnectedEntities(connectedEntities);
		prefabComponent->removeDanglingOverrides(); // cleanup
	}
	else {
		std::cout << "PrefabManager::updatePrefab::ERROR Failed to find PrefabComponent for Entity " << prefabHandle.getEntity().getID() << " after update" << std::endl;
	}

	file.close();
	return true;
}

bool PrefabManager::updatePrefab(std::string filePath) {
	std::size_t size = prefabEntities.size();
	if (size == 0) return true;
	// Load prefab file
	std::ifstream file;
	try {
		file.open(filePath, std::ios::in | std::ios::binary);
	}
	catch (std::system_error&) {
		std::cout << "PrefabManager::updatePrefab::ERROR Failed to load file: " << filePath << ". File does not exist" << std::endl;
		return false;
	}
	if (file.fail()) {
		std::cout << "PrefabManager::updatePrefab::ERROR Failed to load file: " << filePath << ". File open failed" << std::endl;
		return false;
	}
	PrefabDeserializerArchive archive(file, entityManager);
	for (std::size_t i = 0; i < size; i++) {
		PrefabComponent& prefabComponent = prefabEntities.get<PrefabComponent>(i);
		Handle prefabHandle = prefabEntities.createHandle(i, entityManager);
		if (prefabComponent.getFilePath() == filePath) {
			// WIP: Update
			// Update property values
			auto connectedEntities = updatePrefabFromRoot(prefabHandle, prefabComponent, archive);
			if (PrefabComponent* component = prefabHandle.getComponent<PrefabComponent>()) {
				component->setConnectedEntities(connectedEntities);
				component->removeDanglingOverrides(); // cleanup
			}
			else {
				std::cout << "PrefabManager::updatePrefab::ERROR Failed to find PrefabComponent for Entity " << prefabHandle.getEntity().getID() << " after update" << std::endl;
			}
			archive.resetToStart();
		}
	}
	file.close();
	return true;
}

bool PrefabManager::unpackPrefab(Handle prefabHandle) {
	if (!prefabHandle.isValid()) {
		std::cout << "PrefabManager::unpackPrefab::ERROR Failed to unpack prefab. Invalid handle to prefab" << std::endl;
		return false;
	}
	if (!prefabHandle.hasComponent<PrefabComponent>()) {
		std::cout << "PrefabManager::unpackPrefab::ERROR Failed to unpack prefab. The given entity handle is not a prefab" << std::endl;
		return false;
	}
	return prefabHandle.removeComponentImmediate(PrefabComponent::getClassTypeID());
}

bool PrefabManager::createPrefabFromEntity(Handle handle, std::string filePath) {
	EntityHandle entityHandle = handle;
	if (!entityHandle.isValid()) {
		std::cout << "PrefabManager::createPrefabFromEntity::ERROR The given entity handle is invalid" << std::endl;
		return false;
	}
	if (entityHandle.hasComponent<PrefabComponent>()) {
		std::cout << "PrefabManager::createPrefabFromEntity::ERROR Failed to create prefab from Entity. The given entity handle is already a prefab" << std::endl;
		return false;
	}
	PrefabComponent component;
	component.setFilePath(filePath);
	EntityRemapSaveInfo entityRemapInfo;
	if (!savePrefab(filePath, entityHandle, entityRemapInfo)) {
		return false;
	}
	component.setConnectedEntities(getEntitiesFromRootAsConnected(handle, entityRemapInfo));
	entityHandle.addComponent(component);
	return true;
}

Handle PrefabManager::createEntityFromPrefab(std::string filePath, float x, float y) {
	Entity root = entityManager->createEntity(
		Transform(x, y)
	);
	PrefabContent contentData = loadPrefab(filePath, root);
	if (!contentData.success) {
		std::cout << "PrefabManger::createEntityFromPrefab::ERROR Failed to load prefab at path: " << filePath << std::endl;
		if (!entityManager->destroyEntity(root)) {
			std::cout << "PrefabManger::createEntityFromPrefab::ERROR Failed to cleanup root" << std::endl;
		}
		return Handle(); // Invalid handle
	}
	PrefabComponent prefabComponent(filePath);
	prefabComponent.setConnectedEntities(contentData.connectedEntities);
	auto location = entityManager->addComponent(root, prefabComponent);
	Handle rootHandle(root, entityManager, location);
	for (Handle& immediateChild : contentData.content) {
		immediateChild.setParent(rootHandle);
	}
	return rootHandle;
}

/* Helper */
bool createPropertyOverride(Handle ownerHandle, std::size_t typeID, std::string propertyName, PrefabPropertyOverride& out) {
	auto type = Mirror::getType(typeID);
	auto prop = type.getProperty(propertyName);
	if (prop.name.empty()) {
		std::cout << "PrefabManager::createPropertyOverride::ERROR Invalid property name: " << propertyName << std::endl;
		return false;
	}
	ComponentHandle componentHandle(typeID, ownerHandle);
	out = PrefabPropertyOverride(componentHandle, prop.name);
	return true;
}

bool PrefabManager::revertPrefab(Handle rootHandle) {
	std::cout << "Reverting prefab with EntityID " << rootHandle.getEntity().getID() << std::endl;
	std::vector<Entity> childEntities;
	std::vector<Entity> newEntities;
	PrefabComponent* prefabComponent = rootHandle.getComponent<PrefabComponent>();
	if (prefabComponent == nullptr) {
		std::cout << "PrefabManager::revertPrefab::ERROR Unable to revert given Entity since it is not a prefab root" << std::endl;
		return false;
	}
	std::size_t childCount = rootHandle.getChildCount();
	childCount = rootHandle.getChildCount();
	auto connectedEntities = prefabComponent->getConnectedEntities();
	// Clears Entity add overrides
	for (std::size_t i = 0; i < childCount; i++) {
		Handle child = rootHandle.getChild(i);
		Entity entity = child.getEntity();
		bool overriden = std::find_if(connectedEntities.begin(), connectedEntities.end(), [&entity](const ConnectedEntity& connectedEntity) { return connectedEntity.entity == entity; }) == connectedEntities.end();
		if (!overriden) {
			childEntities.push_back(entity);
		}
		else {
			child.destroy(); // Note: Destroying immediately may break iteration of children
		}
	}
	// Clears Entity remove overrides
	for (auto it = connectedEntities.begin(); it != connectedEntities.end();) {
		if (it->guid == PREFAB_ROOT_GUID) {
			it++;
			continue;
		}
		bool overriden = std::find_if(childEntities.begin(), childEntities.end(), [&it](const Entity& entity) { return entity == it->entity; }) == childEntities.end();
		if (overriden) {
			it = connectedEntities.erase(it);
		}
		else {
			it++;
		}
	}
	// Clears Component add overrides
	for (const auto& connectedEntity : connectedEntities) {
		if (connectedEntity.guid == PREFAB_ROOT_GUID) continue;
		auto location = entityManager->getLocationDetailed(connectedEntity.entity);
		for (auto& componentType : entityManager->getComponentTypes(connectedEntity.entity)) {
			//if (componentType.type.getTypeID() == ChildManager::getClassTypeID()) continue; // Note: ChildManager component needs to be removed by update instead to ensure a clean update
			bool overriden = std::find(connectedEntity.componentTypeIDs.begin(), connectedEntity.componentTypeIDs.end(), componentType.type.getTypeID()) == connectedEntity.componentTypeIDs.end();
			if (overriden) {
				location = entityManager->removeComponent(location, componentType.type.getTypeID());
			}
		}
	}
	// Clears Component remove overrides
	for (const auto& connectedEntity : connectedEntities) {
		if (connectedEntity.guid == PREFAB_ROOT_GUID) continue;
		auto location = entityManager->getLocationDetailed(connectedEntity.entity);
		auto componentTypes = entityManager->getComponentTypes(connectedEntity.entity);
		for (auto& componentTypeID : connectedEntity.componentTypeIDs) {
			bool overriden = std::find_if(componentTypes.begin(), componentTypes.end(), [&componentTypeID](const IComponentTypeInfo& componentType) { return componentType.type.getTypeID() == componentTypeID; }) == componentTypes.end();
			if (overriden) {
				location = entityManager->addComponent(location, componentTypeID);
			}
		}
	}
	// Clears property overrides
	for (PrefabComponent* prefabComponent : rootHandle.getComponentsUpwards<PrefabComponent>()) {
		auto propertyOverrides = prefabComponent->getPropertyOverrides();
		for (auto& propertyOverride : propertyOverrides) {
			if (std::find(childEntities.begin(), childEntities.end(), propertyOverride.targetComponent.getOwner().getEntity()) != childEntities.end()) {
				prefabComponent->removePropertyOverride(propertyOverride);
			}
		}
	}
	prefabComponent->setConnectedEntities(connectedEntities);
	if (!updatePrefab(rootHandle)) {
		std::cout << "PrefabManager::revertPrefab::ERROR Failed to update prefab after revert" << std::endl;
	}
	/*
	// Clears Component add overrides
	if (PrefabComponent* prefabComponent = rootHandle.getComponent<PrefabComponent>()) {
		for (const auto& connectedEntity : prefabComponent->getConnectedEntities()) {
			if (connectedEntity.guid == PREFAB_ROOT_GUID) continue;
			auto location = entityManager->getLocationDetailed(connectedEntity.entity);
			for (auto& componentType : entityManager->getComponentTypes(connectedEntity.entity)) {
				bool overriden = std::find(connectedEntity.componentTypeIDs.begin(), connectedEntity.componentTypeIDs.end(), componentType.type.getTypeID()) == connectedEntity.componentTypeIDs.end();
				if (overriden) {
					location = entityManager->removeComponent(location, componentType.type.getTypeID());
				}
			}
		}
	}
	*/
	return true;
}

bool PrefabManager::overrideProperty(Handle ownerHandle, std::size_t typeID, std::string propertyName) {
	Handle receiver = getPrefabOverrideReceiver(ownerHandle);
	PrefabComponent* prefabComponent = receiver.getComponent<PrefabComponent>();
	if (prefabComponent == nullptr) return false;

	PrefabPropertyOverride propertyOverride;
	if (createPropertyOverride(ownerHandle, typeID, propertyName, propertyOverride)) {
		return prefabComponent->addPropertyOverride(propertyOverride);
	}
	return false;
}

bool PrefabManager::removePropertyOverride(Handle ownerHandle, std::size_t typeID, std::string propertyName) {
	std::vector<PrefabComponent*> prefabComponents = ownerHandle.getComponentsInParents<PrefabComponent>();
	bool success = false;
	for (PrefabComponent* prefabComponent : prefabComponents) {
		PrefabPropertyOverride propertyOverride;
		if (createPropertyOverride(ownerHandle, typeID, propertyName, propertyOverride)) {
			bool result = prefabComponent->removePropertyOverride(propertyOverride);
			success = success || result;
		}
	}
	return success;
}

bool PrefabManager::isEntityAnOverride(Handle entity) {
	PrefabComponent* prefabComponent = entity.getComponentInParents<PrefabComponent>();
	if (prefabComponent == nullptr) {
		return false;
	}
	auto connectedEntities = prefabComponent->getConnectedEntities();
	return std::find_if(connectedEntities.begin(), connectedEntities.end(), [&entity](const ConnectedEntity& connectedEntity) { return connectedEntity.entity.getID() == entity.getEntity().getID(); }) == connectedEntities.end();
}

bool PrefabManager::savePrefab(std::string filePath, Handle rootHandle, EntityRemapSaveInfo& entityRemapInfo) {
	if (!rootHandle.isValid()) {
		std::cout << "PrefabManager::savePrefab::ERROR Failed to save prefab. Invalid handle to prefab" << std::endl;
		return false;
	}
	std::ofstream file;
	try {
		file.open(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
	}
	catch (std::system_error&) {
		std::cout << "PrefabManager::savePrefab::ERROR Failed to load file: " << filePath << ". File does not exist" << std::endl;
		return false;
	}
	if (file.fail()) {
		std::cout << "PrefabManager::savePrefab::ERROR Failed to load file: " << filePath << ". File open failed" << std::endl;
		return false;
	}
	SerializerArchive archive(file);
	std::cout << "Saving prefab: " << filePath << std::endl;

	// Save EntityRemapInfo
	serializeEntityRemapInfo(rootHandle, entityRemapInfo, archive);
	archive.setEntityRemapInfo(entityRemapInfo);
	// Save content
	std::size_t childCount = rootHandle.getChildCount();
	std::size_t immediateChildCount = rootHandle.getImmediateChildCount();
	archive(childCount);
	archive(immediateChildCount);
	for (std::size_t i = 0; i < childCount; i++) {
		EntityHandle child = rootHandle.getChild(i);
		serializeEntity(child, archive);
	}
	file.close();
	if (PrefabComponent* component = rootHandle.getComponent<PrefabComponent>()) {
		component->setConnectedEntities(getEntitiesFromRootAsConnected(rootHandle, entityRemapInfo));
	}
	revertPrefab(rootHandle); // Note: Called to remove property overrides

	std::wcout << L"Successfully saved prefab" << std::endl;
	return true;
}

PrefabContent PrefabManager::loadPrefab(std::string filePath, Entity rootEntity) {
	PrefabContent result;
	std::vector<Handle>& content = result.content;
	std::ifstream file;
	try {
		file.open(filePath, std::ios::in | std::ios::binary);
	}
	catch (std::system_error&) {
		std::cout << "PrefabManager::loadPrefab::ERROR Failed to load file: " << filePath << ". File does not exist" << std::endl;
		return result;
	}
	if (file.fail()) {
		std::cout << "PrefabManager::loadPrefab::ERROR Failed to load file: " << filePath << ". File open failed" << std::endl;
		return result;
	}
	DeserializerArchive archive(file, entityManager);
	// Loads EntityRemapInfo and assigns new EntityIDs
	auto entityRemapInfo = deserializeEntityRemapInfo(archive);
	std::vector<EntityHandle> handles;
	handles.reserve(entityRemapInfo.size());
	for (auto& map : entityRemapInfo) {
		if (map.first == PREFAB_ROOT_GUID) {
			map.second = rootEntity.getID();
		}
		else {
			Entity entity = entityManager->createEntity();
			handles.push_back(EntityHandle(entity, entityManager));
			map.second = entity.getID();
		}
	}
	archive.setEntityRemapInfo(entityRemapInfo);
	result.entityRemapInfo = entityRemapInfo;

	// Load content
	std::size_t childCount = 0;
	std::size_t immediateChildCount = 0;
	archive(childCount);
	archive(immediateChildCount);
	content.reserve(immediateChildCount);
	for (std::size_t i = 0; i < immediateChildCount; i++) {
		deserializeEntity(handles[i], archive);
		content.push_back(handles[i]);
	}
	for (std::size_t i = immediateChildCount; i < childCount; i++) {
		deserializeEntity(handles[i], archive);
	}
	file.close();
	for (auto& remap : entityRemapInfo) {
		ConnectedEntity connectedEntity;
		connectedEntity.entity = Entity(remap.second);
		connectedEntity.guid = remap.first;
		for (auto& componentType : entityManager->getComponentTypes(Entity(remap.second))) {
			connectedEntity.componentTypeIDs.push_back(componentType.type.getTypeID());
		}
		result.connectedEntities.push_back(connectedEntity);
	}
	result.success = true;
	return result;
}

/* Remove child references that are invalid or have parent set to something else */
void cleanChildRefs(Handle& handle) {
	if (ChildManager* component = handle.getComponent<ChildManager>()) {
		while (component->onChildRemoved(Handle()));
		std::size_t immediateChildCount = component->getChildCount();
		for (std::size_t i = 0; i < immediateChildCount;) {
			Handle child = component->getChild(i);
			if (child.isImmediateParent(handle.getEntity())) {
				cleanChildRefs(child);
				i++;
			}
			else {
				component->onChildRemoved(child);
				immediateChildCount--;
			}
		}
		if (immediateChildCount == 0) {
			handle.removeComponentImmediate(ChildManager::getClassTypeID());
		}
	}
}

std::vector<ConnectedEntity> PrefabManager::updatePrefabFromRoot(Handle rootHandle, PrefabComponent& prefabComponent, PrefabDeserializerArchive& archive) {
	std::cout << "Updating prefab with EntityID " << rootHandle.getEntity().getID() << " at path " << prefabComponent.getFilePath() << std::endl;
	std::vector<ConnectedEntity> result;
	std::vector<ConnectedEntity> connectedEntities = prefabComponent.getConnectedEntities();
	auto propertyOverrides = getPropertyOverridesAt(rootHandle);
	// Creates new remap and creates new Entities from prefab
	auto newEntityRemapInfo = deserializeEntityRemapInfo(archive);
	std::vector<EntityHandle> handles;
	std::vector<Handle> newEntities;
	for (auto& map : newEntityRemapInfo) {
		if (map.first == PREFAB_ROOT_GUID) {
			map.second = rootHandle.getEntity().getID();
		}
		else {
			auto it = std::find_if(connectedEntities.begin(), connectedEntities.end(), [&map](const ConnectedEntity& connectedEntity) { return connectedEntity.guid == map.first; });
			EntityHandle handle;
			if (it == connectedEntities.end()) {
				Entity entity = entityManager->createEntity();
				handle = EntityHandle(entity, entityManager);
				newEntities.push_back(handle);
			}
			else {
				Entity& entity = it->entity;
				handle = EntityHandle(entity, entityManager);
				if (!handle.isParent(rootHandle.getEntity())) {
					handle = Handle(Entity(0), entityManager);
				}
			}
			handles.push_back(handle);
			map.second = handle.getEntity().getID();
		}
	}
	archive.setEntityRemapInfo(newEntityRemapInfo);

	// Load or update content
	std::size_t childCount = 0;
	std::size_t immediateChildCount = 0;
	archive(childCount);
	archive(immediateChildCount);
	if (childCount + 1 != newEntityRemapInfo.size()) {
		std::cout << "PrefabManager::updatePrefabFromRoot::ERROR Loaded childcount + 1 does not match loaded entityRemapInfo size" << std::endl;
		throw std::runtime_error("PrefabManager::updatePrefabFromRoot::ERROR Loaded childcount + 1 does not match loaded entityRemapInfo size");
	}
	for (std::size_t i = 0; i < childCount; i++) {
		auto connectedComponents = deserializeAndUpdate(archive, handles[i], propertyOverrides, connectedEntities);
		ConnectedEntity& connectedEntity = result.emplace_back();
		connectedEntity.entity = handles[i].getEntity();
		connectedEntity.componentTypeIDs = connectedComponents;
		auto it = std::find_if(newEntityRemapInfo.begin(), newEntityRemapInfo.end(), [&connectedEntity](const std::pair<std::string, std::size_t>& pair) { return pair.second == connectedEntity.entity.getID(); });
		connectedEntity.guid = it->first;
	}
	// Destroys all new Entities with an invalid parent and ensures new Entities are a child ref to their parent
	for (auto& handle : handles) {
		// Destroys new Entity if parent is invalid
		Handle parentHandle = handle.getParent();
		if (!parentHandle.refresh()) {
			handle.destroyImmediate();
			continue;
		}
		// Adds child ref if parent doesn't have ref
		if (!parentHandle.isImmediateChild(handle.getEntity())) {
			ChildManager* childManager = parentHandle.getComponent<ChildManager>();
			if (childManager == nullptr) {
				childManager = static_cast<ChildManager*>(parentHandle.addComponentImmediate(ChildManager::getClassTypeID()));
			}
			childManager->onChildAdded(handle);
		}
	}

	// Remove child reference to Entity which have been overriden as destroyed or have parent set to something else
	cleanChildRefs(rootHandle);

	// Destroys Entities which have been removed in the prefab
	for (const auto& connectedEntity : connectedEntities) {
		auto it = std::find_if(newEntityRemapInfo.begin(), newEntityRemapInfo.end(), [&connectedEntity](const std::pair<std::string, std::size_t>& pair) { return pair.first == connectedEntity.guid; });
		if (it == newEntityRemapInfo.end()) {
			entityManager->destroyEntity(connectedEntity.entity); // Note: May chain and destroy children but attempting to destroy a nonexistent Entity does not throw an error, at the time of writing this
		}
	}

	return result;
}

Handle PrefabManager::getPrefabOverrideReceiver(Handle handle) {
	Handle parent = handle.getParent();
	while (parent.refresh()) {
		if (parent.hasComponent<PrefabComponent>()) {
			return parent;
		}
		parent = parent.getParent();
	}
	return Handle();
	/*Handle parent = handle.getParent();
	Handle receiver;
	while (parent.isValid()) {
		if (parent.hasComponent<PrefabComponent>()) {
			receiver = parent;
		}
		Handle newParent = parent.getParent();
		if (newParent.hasParent()) { // Makes sure newParent is not a root (aka scene)
			parent = newParent;
		}
		else {
			parent = Handle();
		}
	}
	return receiver;*/
}

bool PrefabManager::isEntityPrefabRoot(Handle entity) {
	return entity.hasComponent<PrefabComponent>();
}

std::vector<PrefabPropertyOverride> PrefabManager::getPropertyOverrides(Handle handle, std::size_t typeID) {
	std::vector<PrefabPropertyOverride> overrides = getPropertyOverridesAt(handle);
	std::vector<PrefabPropertyOverride> result;
	for (const PrefabPropertyOverride& override : overrides) {
		if (override.targetComponent.getOwner().getEntity() == handle.getEntity() && override.targetComponent.getComponentTypeID() == typeID) {
			result.push_back(override);
		}
	}
	return result;
}

std::vector<std::size_t> PrefabManager::getComponentOverrides(Handle handle) {
	std::vector<std::size_t> result;
	if (PrefabComponent* prefabComponent = handle.getComponentInParents<PrefabComponent>()) {
		auto connectedEntities = prefabComponent->getConnectedEntities();
		auto it = std::find_if(connectedEntities.begin(), connectedEntities.end(), [&handle](const ConnectedEntity& connectedEntity) { return connectedEntity.entity == handle.getEntity(); });
		if (it != connectedEntities.end()) {
			auto componentTypes = handle.getComponentTypes();
			// Adds add component overrides
			for (const auto& componentType : componentTypes) {
				bool overriden = std::find_if(it->componentTypeIDs.begin(), it->componentTypeIDs.end(), [&componentType](const std::size_t& componentTypeID) { return componentTypeID == componentType.type.getTypeID(); }) == it->componentTypeIDs.end();
				if (overriden) {
					result.push_back(componentType.type.getTypeID());
				}
			}
			// Adds remove component overrides
			for (auto& componentTypeID : it->componentTypeIDs) {
				bool overriden = std::find_if(componentTypes.begin(), componentTypes.end(), [&componentTypeID](const IComponentTypeInfo& componentType) { return componentTypeID == componentType.type.getTypeID(); }) == componentTypes.end();
				if (overriden) {
					result.push_back(componentTypeID);
				}
			}
		}
	}
	return result;
}

std::vector<PrefabPropertyOverride> PrefabManager::getPropertyOverridesAt(Handle handle) {
	std::vector<PrefabPropertyOverride> result;
	for (PrefabComponent* prefabComponent : handle.getComponentsUpwards<PrefabComponent>()) {
		prefabComponent->removeDanglingOverrides(); // Cleanup
		auto overrides = prefabComponent->getPropertyOverrides();
		result.insert(result.end(), overrides.begin(), overrides.end());
	}
	return result;
}

std::vector<std::size_t> PrefabManager::deserializeAndUpdate(PrefabDeserializerArchive& archive, EntityHandle& entityHandle, std::vector<PrefabPropertyOverride>& propertyOverrides, const std::vector<ConnectedEntity>& connectedEntities) {
	std::vector<std::size_t> result;
	std::size_t componentAmount;
	archive(componentAmount);												// Component amount

	std::vector<std::size_t> overridenComponentTypeIDs;
	auto it = std::find_if(connectedEntities.begin(), connectedEntities.end(), [&entityHandle](const ConnectedEntity& connectedEntity) { return connectedEntity.entity == entityHandle.getEntity(); });
	if (it != connectedEntities.end()) {
		for (const auto& componentType : entityHandle.getComponentTypes()) {
			bool overriden = std::find_if(it->componentTypeIDs.begin(), it->componentTypeIDs.end(), [&componentType](const std::size_t& componentTypeID) { return componentTypeID == componentType.type.getTypeID(); }) == it->componentTypeIDs.end();
			if (overriden) {
				overridenComponentTypeIDs.push_back(componentType.type.getTypeID());
			}
		}
	}
	for (std::size_t i = 0; i < componentAmount; i++) {						// Loop through Components
		std::string typeName;												// TODO: Use SerializationID instead
		archive(typeName);													// Component Type name
		auto type = Mirror::getType(typeName);
		std::size_t typeID = type.typeID;
		if (entityHandle.isValid() && !type.hasAnnotation("alwaysOverriden") && std::find(overridenComponentTypeIDs.begin(), overridenComponentTypeIDs.end(), typeID) == overridenComponentTypeIDs.end()) {
			result.push_back(typeID);
			std::vector<std::string> overridenProperties;
			if (entityHandle.hasComponent(typeID)) {
				// Updates component
				for (const auto& override : propertyOverrides) {
					if (override.targetComponent.getOwner().getEntity() == entityHandle.getEntity() && override.targetComponent.getComponentTypeID() == typeID) {
						overridenProperties.push_back(override.targetPropertyName);
					}
				}
				for (auto& prop : type.properties) {
					if (!prop.hasAnnotation("alwaysOverriden")) continue;
					auto it = std::find_if(propertyOverrides.begin(), propertyOverrides.end(), [&prop](const PrefabPropertyOverride& override) { return override.targetPropertyName == prop.name; });
					if (it == propertyOverrides.end()) {
						PrefabPropertyOverride propOverride;
						createPropertyOverride(entityHandle, typeID, prop.name, propOverride);
						propertyOverrides.push_back(propOverride);
					}
				}
				archive.setOverridenProperties(overridenProperties);
				ComponentLoader::updateComponentFromFile(archive, typeName, entityHandle);	// Component Data
			}
			else {
				// Adds new component
				archive.setOverridenProperties(overridenProperties);
				ComponentLoader::addComponentFromFile(archive, typeName, entityHandle);	// Component Data
			}
		}
		else {
			std::cout << "Skipping component" << std::endl;
			// Skips component
			archive.skipProperties();
		}
	}
	return result;
}