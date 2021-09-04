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


PrefabManager::PrefabManager(EntityManager* entityManager) : entityManager(entityManager){}
PrefabManager::~PrefabManager() {}


EntityRemapSaveInfo reverseEntityRemapInfo(EntityRemapLoadInfo entityRemapInfo) {
	EntityRemapSaveInfo result;
	std::for_each(entityRemapInfo.begin(), entityRemapInfo.end(),
		[&result](const std::pair<std::string, std::size_t>& p) {
			result.insert(std::make_pair(p.second, p.first));
		});
	return result;
}

EntityRemapLoadInfo reverseEntityRemapInfo(EntityRemapSaveInfo entityRemapInfo) {
	EntityRemapLoadInfo result;
	std::for_each(entityRemapInfo.begin(), entityRemapInfo.end(),
		[&result](const std::pair<std::size_t, std::string>& p) {
			result.insert(std::make_pair(p.second, p.first));
		});
	return result;
}

std::string createGUID(const std::map<std::size_t, std::string>& map) {
	std::string guid = GUID::create();
	if (std::find_if(map.begin(), map.end(), [&guid](const std::pair<std::size_t, std::string>& pair) { return pair.second == guid; }) == map.end()) {
		return guid;
	}
	return createGUID(map);
}

/* Ensures validation of EntityRemapSaveInfo for the given Entity IDs. Creates new GUID for new Entities and removes unused remaps */
void ensureEntityRemapInfo(EntityRemapSaveInfo& entityRemapInfo, const std::vector<std::size_t>& entityIDs) {
	// Removes unused remaps
	for (auto it = entityRemapInfo.begin(); it != entityRemapInfo.end();) {
		if (std::find(entityIDs.begin(), entityIDs.end(), it->first) == entityIDs.end()) {
			it = entityRemapInfo.erase(it);
		}
		else {
			it++;
		}
	}
	// Add new remaps
	for (const std::size_t& entityID : entityIDs) {
		if (!entityRemapInfo.contains(entityID)) {
			std::string entityGUID = createGUID(entityRemapInfo);
			entityRemapInfo.insert(std::pair<std::size_t, std::string>(entityID, entityGUID));
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
	ensureEntityRemapInfo(entityRemapInfo, entityIDs);
	// Saves child count
	std::size_t mapSize = entityRemapInfo.size();
	if (mapSize != childCount) {
		std::cout << "PrefabManager::serializeEntityRemapInfo::ERROR Mapsize does not match childCount" << std::endl;
		throw std::runtime_error("PrefabManager::serializeEntityRemapInfo::ERROR Mapsize does not match childCount");
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
	for (std::size_t i = 0; i < mapSize; i++) {
		std::string entityGUID;
		archive(entityGUID);
		entityRemapInfo.insert(std::pair<std::string, std::size_t>(entityGUID, 0)); // 0 is temp
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
	if (prefabComponent == nullptr) return false;

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
	PrefabDeserializerArchive archive(file);
	auto newEntityRemapInfo = updatePrefabFromRoot(prefabHandle, *prefabComponent, archive);
	prefabComponent->setEntityRemapInfo(newEntityRemapInfo);
	prefabComponent->removeDanglingOverrides(); // cleanup

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
	PrefabDeserializerArchive archive(file);
	for (std::size_t i = 0; i < size; i++) {
		PrefabComponent& prefabComponent = prefabEntities.get<PrefabComponent>(i);
		Handle prefabHandle = prefabEntities.createHandle(i, entityManager);
		if (prefabComponent.getFilePath() == filePath) {
			// WIP: Update
			// Update property values
			auto newEntityRemapInfo = updatePrefabFromRoot(prefabHandle, prefabComponent, archive);
			prefabComponent.setEntityRemapInfo(newEntityRemapInfo);
			prefabComponent.removeDanglingOverrides(); // cleanup
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
	prefabHandle.removeComponent(PrefabComponent::getClassTypeID());
	return true;
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
	component.setEntityRemapInfo(reverseEntityRemapInfo(entityRemapInfo));
	entityHandle.addComponent(component);
	return true;
}

Handle PrefabManager::createEntityFromPrefab(std::string filePath, float x, float y) {
	PrefabContent contentData = loadPrefab(filePath);
	PrefabComponent prefabComponent(filePath);
	prefabComponent.setEntityRemapInfo(contentData.entityRemapInfo);
	Entity root = entityManager->createEntity(
		Transform(x, y),
		prefabComponent
	);
	Handle rootHandle(root, entityManager);
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

bool PrefabManager::clearOverrides(Handle rootHandle) {
	std::vector<Entity> allChildEntities;
	std::size_t childCount = rootHandle.getChildCount();
	allChildEntities.reserve(childCount);
	for (std::size_t i = 0; i < childCount; i++) {
		allChildEntities.push_back(rootHandle.getChild(i).getEntity());
	}
	std::vector<PrefabComponent*> prefabComponents = rootHandle.getComponentsUpwards<PrefabComponent>();
	for (PrefabComponent* prefabComponent : prefabComponents) {
		// Clears property overrides
		auto propertyOverrides = prefabComponent->getPropertyOverrides();
		for (auto& propertyOverride : propertyOverrides) {
			if (std::find(allChildEntities.begin(), allChildEntities.end(), propertyOverride.targetComponent.getOwner().getEntity()) != allChildEntities.end()) {
				prefabComponent->removePropertyOverride(propertyOverride);
			}
		}
		// Clear component overrides
		auto componentOverrides = prefabComponent->getComponentOverrides();
		for (auto& componentOverride : componentOverrides) {
			if (std::find(allChildEntities.begin(), allChildEntities.end(), componentOverride.targetEntity.getEntity()) != allChildEntities.end()) {
				prefabComponent->removeComponentOverride(componentOverride);
			}
		} 
	}
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
	for (PrefabComponent* prefabComponent : prefabComponents) {
		PrefabPropertyOverride propertyOverride;
		if (createPropertyOverride(ownerHandle, typeID, propertyName, propertyOverride)) {
			return prefabComponent->removePropertyOverride(propertyOverride);
		}
	}
	return true;
}

bool PrefabManager::overrideComponent(Handle ownerHandle, std::size_t typeID) {
	Handle receiver = getPrefabOverrideReceiver(ownerHandle);
	PrefabComponent* prefabComponent = receiver.getComponent<PrefabComponent>();
	if (prefabComponent == nullptr) return false;

	return prefabComponent->addComponentOverride(PrefabComponentOverride(ownerHandle, typeID));
}

bool PrefabManager::removeComponentOverride(Handle ownerHandle, std::size_t typeID) {
	std::vector<PrefabComponent*> prefabComponents = ownerHandle.getComponentsInParents<PrefabComponent>();
	for (PrefabComponent* prefabComponent : prefabComponents) {
		prefabComponent->removeComponentOverride(PrefabComponentOverride(ownerHandle, typeID));
	}
	return true;
}

bool PrefabManager::isEntityAnOverride(Handle entity) {
	PrefabComponent* prefabComponent = entity.getComponentInParents<PrefabComponent>();
	if (prefabComponent == nullptr) {
		return false;
	}
	auto entityRemapInfo = prefabComponent->getEntityRemapInfo();
	return std::find_if(entityRemapInfo.begin(), entityRemapInfo.end(), [&entity](const std::pair<std::string, std::size_t>& pair) { return pair.second == entity.getEntity().getID(); }) == entityRemapInfo.end();
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
	std::size_t immediateChildCount = rootHandle.getImmediateChildCount();
	archive(immediateChildCount);
	for (std::size_t i = 0; i < immediateChildCount; i++) {
		Handle immediateChild = rootHandle.getChild(i);
		serialize(immediateChild, archive);
	}
	file.close();
	clearOverrides(rootHandle);

	std::wcout << L"Successfully saved prefab" << std::endl;
	return true;
}

PrefabContent PrefabManager::loadPrefab(std::string filePath) {
	std::cout << "Loading prefab: " << filePath << std::endl;
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
	DeserializerArchive archive(file);
	// Loads EntityRemapInfo and assigns new EntityIDs
	auto entityRemapInfo = deserializeEntityRemapInfo(archive);
	std::queue<EntityHandle> handles;
	for (auto& map : entityRemapInfo) {
		Entity entity = entityManager->createEntity();
		handles.push(EntityHandle(entity, entityManager));
		map.second = entity.getID();
	}
	archive.setEntityRemapInfo(entityRemapInfo);
	result.entityRemapInfo = entityRemapInfo;

	// Load content
	std::size_t immediateChildCount = 0;
	archive(immediateChildCount);
	content.reserve(immediateChildCount);
	for (std::size_t i = 0; i < immediateChildCount; i++) {
		Handle handle = deserialize(archive, handles);
		content.push_back(handle);
	}
	file.close();
	return result;
}

EntityRemapLoadInfo PrefabManager::updatePrefabFromRoot(Handle rootHandle, PrefabComponent& prefabComponent, PrefabDeserializerArchive& archive) {
	EntityRemapLoadInfo prevEntityRemapInfo = prefabComponent.getEntityRemapInfo();
	auto propertyOverrides = getPropertyOverridesAt(rootHandle);
	auto componentOverrides = getComponentOverridesAt(rootHandle);
	// Creates new remap and creates new Entities from prefab
	auto newEntityRemapInfo = deserializeEntityRemapInfo(archive);
	std::queue<EntityHandle> handles;
	std::vector<Handle> newEntities;
	for (auto& map : newEntityRemapInfo) {
		auto it = prevEntityRemapInfo.find(map.first);
		Entity entity;
		if (it == prevEntityRemapInfo.end()) {
			entity = entityManager->createEntity();
			newEntities.push_back(EntityHandle(entity, entityManager));
		}
		else {
			entity = Entity(it->second);
			if (!rootHandle.isChild(entity)) {
				entity = Entity(0); // Invalid Entity in case the Entity has been overriden as destroyed. Any references to the destroyed Enti
			}
		}
		handles.push(EntityHandle(entity, entityManager));
		map.second = entity.getID();
	}
	archive.setEntityRemapInfo(newEntityRemapInfo);
	// Destroys Entities which have been removed in the prefab
	for (auto& map : prevEntityRemapInfo) {
		auto it = newEntityRemapInfo.find(map.first);
		if (it == newEntityRemapInfo.end()) {
			entityManager->destroyEntity(Entity(map.second)); // Note: May chain and destroy children but attempting to destroy a nonexistent Entity does not throw an error, at the time of writing this
		}
	}

	// Load or update content
	std::size_t immediateChildCount = 0;
	archive(immediateChildCount);
	for (std::size_t i = 0; i < immediateChildCount; i++) {
		deserializeAndUpdate(archive, handles, propertyOverrides, componentOverrides);
	}

	// Destroys all new Entities with an invalid parent
	for (auto& handle : newEntities) {
		if (!handle.getParent().isValid()) {
			handle.destroy();
		}
	}

	return newEntityRemapInfo;
}

Handle PrefabManager::getPrefabOverrideReceiver(Handle handle) {
	Handle parent = handle.getParent();
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
	return receiver;
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

std::vector<PrefabComponentOverride> PrefabManager::getComponentOverrides(Handle handle) {
	std::vector<PrefabComponentOverride> overrides = getComponentOverridesAt(handle);
	std::vector<PrefabComponentOverride> result;
	for (const PrefabComponentOverride& override : overrides) {
		if (override.targetEntity.getEntity() == handle.getEntity()) {
			result.push_back(override);
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

std::vector<PrefabComponentOverride> PrefabManager::getComponentOverridesAt(Handle handle) {
	std::vector<PrefabComponentOverride> result;
	for (PrefabComponent* prefabComponent : handle.getComponentsUpwards<PrefabComponent>()) {
		prefabComponent->removeDanglingOverrides(); // Cleanup
		auto overrides = prefabComponent->getComponentOverrides();
		result.insert(result.end(), overrides.begin(), overrides.end());
	}
	return result;
}

void PrefabManager::serialize(Handle handle, SerializerArchive& archive) {
	serializeEntity(handle, archive);

	std::size_t childAmount = handle.getImmediateChildCount();
	archive(childAmount);										// Child amount
	for (std::size_t i = 0; i < childAmount; i++) {				// Loop through Children
		Handle child = handle.getChild(i);
		serialize(child, archive);								// Chain serialize
	}
}

Handle PrefabManager::deserialize(DeserializerArchive& archive, std::queue<EntityHandle>& entityHandles) {
	EntityHandle entityHandle = entityHandles.front();
	deserializeEntity(entityHandle, archive);
	entityHandles.pop();

	std::size_t childAmount;
	archive(childAmount);													// Child amount
	for (std::size_t i = 0; i < childAmount; i++) {							// Loop through Children
		deserialize(archive, entityHandles);								// Chain deserialize
	}
	return entityHandle;
}

Handle PrefabManager::deserializeAndUpdate(PrefabDeserializerArchive& archive, std::queue<EntityHandle>& entityHandles, std::vector<PrefabPropertyOverride>& propertyOverrides, std::vector<PrefabComponentOverride>& componentOverrides) {
	EntityHandle entityHandle = entityHandles.front();

	std::size_t componentAmount;
	archive(componentAmount);												// Component amount

	std::vector<std::size_t> overridenComponentTypeIDs;
	for (const auto& override : componentOverrides) {
		if (override.targetEntity.getEntity() == entityHandle.getEntity()) {
			overridenComponentTypeIDs.push_back(override.typeID);
		}
	}
	for (std::size_t i = 0; i < componentAmount; i++) {						// Loop through Components
		std::string typeName;												// TODO: Use SerializationID instead
		archive(typeName);													// Component Type name
		std::size_t typeID = Mirror::getTypeID(typeName);
		if (std::find(overridenComponentTypeIDs.begin(), overridenComponentTypeIDs.end(), typeID) == overridenComponentTypeIDs.end()) {
			std::vector<std::string> overridenProperties;
			if (entityHandle.hasComponent(typeID)) {
				// Updates component
				for (const auto & override : propertyOverrides) {
					if (override.targetComponent.getOwner().getEntity() == entityHandle.getEntity() && override.targetComponent.getComponentTypeID() == typeID) {
						overridenProperties.push_back(override.targetPropertyName);
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
			// Skips component
			archive.skipProperties();
		}
	}
	entityHandles.pop();

	std::size_t childAmount;
	archive(childAmount);													// Child amount
	for (std::size_t i = 0; i < childAmount; i++) {							// Loop through Children
		deserializeAndUpdate(archive, entityHandles, propertyOverrides, componentOverrides);	// Chain deserialize
	}
	return entityHandle;
}