#include "EngineDLLInterface.h"
#include <iostream>
using namespace Editor;

#define CHAR_BUFFER_SIZE 256

EnginePtr EngineDLLInterface::createEngine() {
    if (createEngineFun == nullptr) {
        std::cout << "EngineDLLInterface::createEngine::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::createEngine::ERROR The function ptr is nullptr";
    }
    if (engine) {
        std::cout << "EngineDLLInterface::createEngine::ERROR Failed to create Engine since one already exists!" << std::endl;
        return engine;
    }
    engine = createEngineFun();
    return engine;
}

void EngineDLLInterface::releaseEngine() {
    if (releaseEngineFun == nullptr) {
        std::cout << "EngineDLLInterface::releaseEngine::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::releaseEngine::ERROR The function ptr is nullptr";
    }
    if (!engine) {
        std::cout << "EngineDLLInterface::releaseEngine::ERROR Failed to release Engine since none currently exists!" << std::endl;
    }
    releaseEngineFun(engine);
    engine = nullptr;
}

bool EngineDLLInterface::createTemplateEntity(std::size_t sceneIndex, const char* name, float x, float y, float width, float height) {
    if (createTemplateEntityFun == nullptr) {
        std::cout << "EngineDLLInterface::createTemplateEntity::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::createTemplateEntity::ERROR The function ptr is nullptr";
    }
    return createTemplateEntityFun(engine, sceneIndex, name, x, y, width, height);
}

bool EngineDLLInterface::engineInit(GLADloadproc ptr, int screenWidth, int screenHeight) {
    if (engineInitFun == nullptr) {
        std::cout << "EngineDLLInterface::engineInit::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::engineInit::ERROR The function ptr is nullptr";
    }
    return engineInitFun(engine, ptr, screenWidth, screenHeight);
}

void EngineDLLInterface::engineTick(float deltaTime) {
    if (engineTickFun == nullptr) {
        std::cout << "EngineDLLInterface::engineTick::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::engineTick::ERROR The function ptr is nullptr";
    }
    engineTickFun(engine, deltaTime);
}

ImVec2 EngineDLLInterface::getCameraPosition() {
    if (getCameraPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::getCameraPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getCameraPosition::ERROR The function ptr is nullptr";
    }
    float buffer[2];
    getCameraPositionFun(engine, buffer);
    return ImVec2(buffer[0], buffer[1]);
}
void EngineDLLInterface::setCameraPosition(float x, float y) {
    if (setCameraPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::setCameraPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setCameraPosition::ERROR The function ptr is nullptr";
    }
    setCameraPositionFun(engine, x, y);
}

void EngineDLLInterface::setViewportSize(float width, float height) {
    if (setViewportSizeFun == nullptr) {
        std::cout << "EngineDLLInterface::setViewportSize::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setViewportSize::ERROR The function ptr is nullptr";
    }
    setViewportSizeFun(engine, width, height);
}

EntityID EngineDLLInterface::createEntity(std::size_t sceneIndex, const char* name) {
    if (createEntityFun == nullptr) {
        std::cout << "EngineDLLInterface::createEntity::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::createEntity::ERROR The function ptr is nullptr";
    }
    return createEntityFun(engine, sceneIndex, name);
}

EntityID EngineDLLInterface::getEntityAtPos(float x, float y) {
    if (getEntityAtPosFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityAtPos::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityAtPos::ERROR The function ptr is nullptr";
    }
    return getEntityAtPosFun(engine, x, y);
}

ImVec2 EngineDLLInterface::getRectSize(EntityID entityID) {
    if (getRectSizeFun == nullptr) {
        std::cout << "EngineDLLInterface::getRectSize::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getRectSize::ERROR The function ptr is nullptr";
    }
    float buffer[2];
    getRectSizeFun(engine, entityID, buffer);
    return ImVec2(buffer[0], buffer[1]);
}

ImVec2 EngineDLLInterface::getMinRectScreenPosition(EntityID entityID) {
    if (getMinRectScreenPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::getMinRectScreenPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getMinRectScreenPosition::ERROR The function ptr is nullptr";
    }
    float buffer[2];
    getMinRectScreenPositionFun(engine, entityID, buffer);
    return ImVec2(buffer[0], buffer[1]);
}

ImVec2 EngineDLLInterface::getWorldPosition(EntityID entityID) {
    if (getWorldPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::getWorldPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getWorldPosition::ERROR The function ptr is nullptr";
    }
    float buffer[2];
    getWorldPositionFun(engine, entityID, buffer);
    return ImVec2(buffer[0], buffer[1]);
}

ImVec2 EngineDLLInterface::getLocalPosition(EntityID entityID) {
    if (getLocalPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::getLocalPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getLocalPosition::ERROR The function ptr is nullptr";
    }
    float buffer[2];
    getLocalPositionFun(engine, entityID, buffer);
    return ImVec2(buffer[0], buffer[1]);
}

bool EngineDLLInterface::setLocalPosition(EntityID entityID, float x, float y) {
    if (setLocalPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::setLocalPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setLocalPosition::ERROR The function ptr is nullptr";
    }
    return setLocalPositionFun(engine, entityID, x, y);
}

bool EngineDLLInterface::setWorldPosition(EntityID entityID, float x, float y) {
    if (setWorldPositionFun == nullptr) {
        std::cout << "EngineDLLInterface::setWorldPosition::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setWorldPosition::ERROR The function ptr is nullptr";
    }
    return setWorldPositionFun(engine, entityID, x, y);
}

bool EngineDLLInterface::hasAnnotation(TypeID typeID, const char* annotation) {
    if (hasAnnotationFun == nullptr) {
        std::cout << "EngineDLLInterface::hasAnnotation::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::hasAnnotation::ERROR The function ptr is nullptr";
    }
    return hasAnnotationFun(typeID, annotation);
}

std::string EngineDLLInterface::getTypeName(TypeID typeID) {
    if (getTypeNameFun == nullptr) {
        std::cout << "EngineDLLInterface::getTypeName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getTypeName::ERROR The function ptr is nullptr";
    }
    char buffer[CHAR_BUFFER_SIZE];
    getTypeNameFun(typeID, buffer, CHAR_BUFFER_SIZE);
    return std::string(buffer);
}

void EngineDLLInterface::onUpdate(void* instance, TypeID typeID, std::size_t propIndex) {
    if (onUpdateFun == nullptr) {
        std::cout << "EngineDLLInterface::onUpdate::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::onUpdate::ERROR The function ptr is nullptr";
    }
    onUpdateFun(instance, typeID, propIndex);
}

std::vector<TypeID> EngineDLLInterface::getDerivedTypeIDs(TypeID typeID) {
    if (getDerivedTypeIDsFun == nullptr) {
        std::cout << "EngineDLLInterface::getDerivedTypeIDs::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getDerivedTypeIDs::ERROR The function ptr is nullptr";
    }
    std::size_t outSize = getDerivedTypeIDsCount(typeID);
    std::vector<TypeID> types(outSize);
    getDerivedTypeIDsFun(typeID, &types[0], outSize);
    return types;
}

std::vector<ReflectedTypeData> EngineDLLInterface::getAllReflectedTypes() {
    if (getAllReflectedTypesFun == nullptr) {
        std::cout << "EngineDLLInterface::getAllReflectedTypes::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getAllReflectedTypes::ERROR The function ptr is nullptr";
    }
    std::size_t outSize = getAllReflectedTypesCount();
    std::vector<TypeID> typeIDs(outSize);
    getAllReflectedTypesFun(&typeIDs[0], outSize);
    std::vector<ReflectedTypeData> types(outSize);
    for (std::size_t i = 0; i < outSize; i++) {
        types[i].typeID = typeIDs[i];
        types[i].typeName = getTypeName(typeIDs[i]);
    }
    return types; // TODO: Cache result. Result may only change between reloads
}

TypeID EngineDLLInterface::getTypeIDFromName(const char* name) {
    if (getTypeIDFromNameFun == nullptr) {
        std::cout << "EngineDLLInterface::getTypeIDFromName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getTypeIDFromName::ERROR The function ptr is nullptr";
    }
    return getTypeIDFromNameFun(name);
}

std::size_t EngineDLLInterface::getDerivedTypeIDsCount(TypeID typeID) {
    if (getDerivedTypeIDsCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getDerivedTypeIDsCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getDerivedTypeIDsCount::ERROR The function ptr is nullptr";
    }
    return getDerivedTypeIDsCountFun(typeID);
}

std::size_t EngineDLLInterface::getAllReflectedTypesCount() {
    if (getAllReflectedTypesCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getAllReflectedTypesCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getAllReflectedTypesCount::ERROR The function ptr is nullptr";
    }
    return getAllReflectedTypesCountFun();
}

std::size_t EngineDLLInterface::getPropertiesCount(TypeID typeID) {
    if (getPropertiesCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertiesCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertiesCount::ERROR The function ptr is nullptr";
    }
    return getPropertiesCountFun(typeID);
}

std::vector<ReflectedPropertyData> EngineDLLInterface::getProperties(TypeID typeID, void* instance) {
    std::size_t size = getPropertiesCount(typeID);
    std::vector<ReflectedPropertyData> properties(size);
    for (std::size_t i = 0; i < size; i++) {
        properties[i].index = i;
        properties[i].name = getPropertyName(typeID, i);
        properties[i].renderer = static_cast<InspectorFieldRenderType>(getPropertyType(typeID, i));
        properties[i].typeName = getPropertyTypeName(typeID, i);
        properties[i].typeSize = getPropertyTypeSize(typeID, i);
        properties[i].fields = getPropertyFields(typeID, i, instance);
    }
    return properties;
}

std::size_t EngineDLLInterface::getPropertyType(TypeID typeID, std::size_t propIndex) {
    if (getPropertyTypeFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyType::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyType::ERROR The function ptr is nullptr";
    }
    return getPropertyTypeFun(typeID, propIndex);
}

std::size_t EngineDLLInterface::getPropertyTypeSize(TypeID typeID, std::size_t propIndex) {
    if (getPropertyTypeSizeFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyTypeSize::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyTypeSize::ERROR The function ptr is nullptr";
    }
    return getPropertyTypeSizeFun(typeID, propIndex);
}

std::size_t EngineDLLInterface::getPropertyFieldCount(TypeID typeID, std::size_t propIndex, void* instance) {
    if (getPropertyFieldCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyFieldCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyFieldCount::ERROR The function ptr is nullptr";
    }
    return getPropertyFieldCountFun(typeID, propIndex, instance);
}

std::vector<ReflectedFieldData> EngineDLLInterface::getPropertyFields(TypeID typeID, std::size_t propIndex, void* instance) {
    if (getPropertyFieldsFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyFields::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyFields::ERROR The function ptr is nullptr";
    }
    std::size_t outSize = getPropertyFieldCount(typeID, propIndex, instance);
    std::vector<ReflectedFieldData> fields(outSize);
    std::vector<void*> fieldValuePtrs(outSize);
    std::vector<std::size_t> fieldValueSizes(outSize);
    getPropertyFieldsFun(typeID, propIndex, instance, &fieldValuePtrs[0], &fieldValueSizes[0], outSize);
    for (std::size_t i = 0; i < outSize; i++) {
        fields[i].ptr = fieldValuePtrs[i];
        fields[i].size = fieldValueSizes[i];
    }
    return fields;
}

std::string EngineDLLInterface::getPropertyName(TypeID typeID, std::size_t propIndex) {
    if (getPropertyNameFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyName::ERROR The function ptr is nullptr";
    }
    char buffer[CHAR_BUFFER_SIZE];
    getPropertyNameFun(typeID, propIndex, buffer, CHAR_BUFFER_SIZE);
    return std::string(buffer);
}

std::string EngineDLLInterface::getPropertyTypeName(TypeID typeID, std::size_t propIndex) {
    if (getPropertyTypeNameFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyTypeName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyTypeName::ERROR The function ptr is nullptr";
    }
    char buffer[CHAR_BUFFER_SIZE];
    getPropertyTypeNameFun(typeID, propIndex, buffer, CHAR_BUFFER_SIZE);
    return std::string(buffer);
}

bool EngineDLLInterface::loadScene(const char* path) {
    if (loadSceneFun == nullptr) {
        std::cout << "EngineDLLInterface::loadScene::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::loadScene::ERROR The function ptr is nullptr";
    }
    return loadSceneFun(engine, path);
}

bool EngineDLLInterface::loadSceneBackup(const char* srcPath, const char* destPath) {
    if (loadSceneBackupFun == nullptr) {
        std::cout << "EngineDLLInterface::loadSceneBackup::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::loadSceneBackup::ERROR The function ptr is nullptr";
    }
    return loadSceneBackupFun(engine, srcPath, destPath);
}

bool EngineDLLInterface::unloadScene(std::size_t sceneIndex) {
    if (unloadSceneFun == nullptr) {
        std::cout << "EngineDLLInterface::unloadScene::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::unloadScene::ERROR The function ptr is nullptr";
    }
    return unloadSceneFun(engine, sceneIndex);
}

bool EngineDLLInterface::saveSceneBackup(std::size_t sceneIndex, const char* path) {
    if (saveSceneBackupFun == nullptr) {
        std::cout << "EngineDLLInterface::saveSceneBackup::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::saveSceneBackup::ERROR The function ptr is nullptr";
    }
    return saveSceneBackupFun(engine, sceneIndex, path);
}

bool EngineDLLInterface::saveScene(std::size_t sceneIndex) {
    if (saveSceneFun == nullptr) {
        std::cout << "EngineDLLInterface::saveScene::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::saveScene::ERROR The function ptr is nullptr";
    }
    return saveSceneFun(engine, sceneIndex);
}

bool EngineDLLInterface::createScene(const char* name, const char* path) {
    if (createSceneFun == nullptr) {
        std::cout << "EngineDLLInterface::createScene::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::createScene::ERROR The function ptr is nullptr";
    }
    return createSceneFun(engine, name, path);
}

bool EngineDLLInterface::destroyEntity(std::size_t sceneIndex, EntityID entityID) {
    if (destroyEntityFun == nullptr) {
        std::cout << "EngineDLLInterface::destroyEntity::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::destroyEntity::ERROR The function ptr is nullptr";
    }
    return destroyEntityFun(engine, sceneIndex, entityID);
}

std::vector<EntityID> EngineDLLInterface::getAllEntities(std::size_t sceneIndex) {
    if (getAllEntitiesFun == nullptr) {
        std::cout << "EngineDLLInterface::getAllEntities::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getAllEntities::ERROR The function ptr is nullptr";
    }
    std::size_t outSize = getAllEntitiesCount(sceneIndex);
    std::vector<EntityID> entities(outSize);
    if (getAllEntitiesFun(engine, sceneIndex, &entities[0], outSize)) {
        return entities;
    }
    return std::vector<EntityID>();
}

std::string EngineDLLInterface::getSceneName(std::size_t sceneIndex) {
    if (getSceneNameFun == nullptr) {
        std::cout << "EngineDLLInterface::getSceneName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getSceneName::ERROR The function ptr is nullptr";
    }
    char buffer[CHAR_BUFFER_SIZE];
    if (getSceneNameFun(engine, sceneIndex, buffer, CHAR_BUFFER_SIZE)) {
        return std::string(buffer);
    }
    return std::string();
}

std::string EngineDLLInterface::getSceneFilePath(std::size_t sceneIndex) {
    if (getSceneFilePathFun == nullptr) {
        std::cout << "EngineDLLInterface::getSceneFilePath::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getSceneFilePath::ERROR The function ptr is nullptr";
    }
    char buffer[CHAR_BUFFER_SIZE];
    if (getSceneFilePathFun(engine, sceneIndex, buffer, CHAR_BUFFER_SIZE)) {
        return std::string(buffer);
    }
    return std::string();
}

bool EngineDLLInterface::hasSceneChanged(std::size_t sceneIndex) {
    if (hasSceneChangedFun == nullptr) {
        std::cout << "EngineDLLInterface::hasSceneChanged::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::hasSceneChanged::ERROR The function ptr is nullptr";
    }
    return hasSceneChangedFun(engine, sceneIndex);
}

std::size_t EngineDLLInterface::getSceneCount() {
    if (getSceneCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getSceneCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getSceneCount::ERROR The function ptr is nullptr";
    }
    return getSceneCountFun(engine);
}

std::size_t EngineDLLInterface::getAllEntitiesCount(std::size_t sceneIndex) {
    if (getAllEntitiesCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getAllEntitiesCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getAllEntitiesCount::ERROR The function ptr is nullptr";
    }
    return getAllEntitiesCountFun(engine, sceneIndex);
}

void EngineDLLInterface::setSceneAddedCallback(SceneAddedCallbackFun fun) {
    if (setSceneAddedCallbackFun == nullptr) {
        std::cout << "EngineDLLInterface::setSceneAddedCallback::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setSceneAddedCallback::ERROR The function ptr is nullptr";
    }
    setSceneAddedCallbackFun(engine, fun);
}

void EngineDLLInterface::setSceneRemovedCallback(SceneRemovedCallbackFun fun) {
    if (setSceneRemovedCallbackFun == nullptr) {
        std::cout << "EngineDLLInterface::setSceneRemovedCallback::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setSceneRemovedCallback::ERROR The function ptr is nullptr";
    }
    setSceneRemovedCallbackFun(engine, fun);
}

void EngineDLLInterface::setEntityRenamedCallback(EntityRenamedCallbackFun fun) {
    if (setEntityRenamedCallbackFun == nullptr) {
        std::cout << "EngineDLLInterface::setEntityRenamedCallback::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setEntityRenamedCallback::ERROR The function ptr is nullptr";
    }
    setEntityRenamedCallbackFun(engine, fun);
}

void EngineDLLInterface::setCallbackPtr(void* ptr) {
    if (setCallbackPtrFun == nullptr) {
        std::cout << "EngineDLLInterface::setCallbackPtr::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setCallbackPtr::ERROR The function ptr is nullptr";
    }
    setCallbackPtrFun(engine, ptr);
}

bool EngineDLLInterface::addComponent(EntityID entityID, std::size_t sceneIndex, TypeID typeID) {
    if (addComponentFun == nullptr) {
        std::cout << "EngineDLLInterface::addComponent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::addComponent::ERROR The function ptr is nullptr";
    }
    return addComponentFun(engine, entityID, sceneIndex, typeID);
}

bool EngineDLLInterface::removeComponent(EntityID entityID, std::size_t sceneIndex, TypeID typeID) {
    if (removeComponentFun == nullptr) {
        std::cout << "EngineDLLInterface::removeComponent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::removeComponent::ERROR The function ptr is nullptr";
    }
    return removeComponentFun(engine, entityID, sceneIndex, typeID);
}

bool EngineDLLInterface::renameEntity(EntityID entityID, const char* name) {
    if (renameEntityFun == nullptr) {
        std::cout << "EngineDLLInterface::renameEntity::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::renameEntity::ERROR The function ptr is nullptr";
    }
    return renameEntityFun(engine, entityID, name);
}

std::vector<ComponentData> EngineDLLInterface::getComponents(EntityID entityID) {
    if (getComponentsFun == nullptr) {
        std::cout << "EngineDLLInterface::getComponents::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getComponents::ERROR The function ptr is nullptr";
    }
    std::size_t outSize = getComponentsCount(entityID);
    std::vector<ComponentData> components(outSize);
    if (outSize > 0) {
        std::vector<void*> componentPtrs(outSize);
        std::vector<TypeID> componentTypeIDs(outSize);
        getComponentsFun(engine, entityID, &componentPtrs[0], &componentTypeIDs[0], outSize);
        for (std::size_t i = 0; i < outSize; i++) {
            components[i].instance = componentPtrs[i];
            components[i].typeID = componentTypeIDs[i];
        }
    }
    return components;
}

ComponentData EngineDLLInterface::getComponent(EntityID entityID, TypeID typeID) {
    std::vector<ComponentData> components = getComponents(entityID);
    for (const ComponentData& component : components) {
        if (component.typeID == typeID) {
            return component;
        }
    }
    std::cout << "EngineDLLInterface::getComponent::ERROR Found no matching component with typeID: " << typeID << std::endl;
    throw "EngineDLLInterface::getComponent::ERROR Found no matching component";
}

bool EngineDLLInterface::isEntityChild(EntityID entityID, EntityID parentID) {
    if (isEntityChildFun == nullptr) {
        std::cout << "EngineDLLInterface::isEntityChild::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::isEntityChild::ERROR The function ptr is nullptr";
    }
    return isEntityChildFun(engine, entityID, parentID);
}

bool EngineDLLInterface::setEntityParent(std::size_t sceneIndex, EntityID entityID, EntityID parentID) {
    if (setEntityParentFun == nullptr) {
        std::cout << "EngineDLLInterface::setEntityParent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setEntityParent::ERROR The function ptr is nullptr";
    }
    return setEntityParentFun(engine, sceneIndex, entityID, parentID);
}

bool EngineDLLInterface::isEntityNameAvailable(const char* name) {
    if (isEntityNameAvailableFun == nullptr) {
        std::cout << "EngineDLLInterface::isEntityNameAvailable::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::isEntityNameAvailable::ERROR The function ptr is nullptr";
    }
    return isEntityNameAvailableFun(engine, name);
}

bool EngineDLLInterface::hasEntityParent(EntityID entityID) {
    if (hasEntityParentFun == nullptr) {
        std::cout << "EngineDLLInterface::hasEntityParent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::hasEntityParent::ERROR The function ptr is nullptr";
    }
    return hasEntityParentFun(engine, entityID);
}

bool EngineDLLInterface::detachEntityParent(std::size_t sceneIndex, EntityID entityID) {
    if (detachEntityParentFun == nullptr) {
        std::cout << "EngineDLLInterface::detachEntityParent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::detachEntityParent::ERROR The function ptr is nullptr";
    }
    return detachEntityParentFun(engine, sceneIndex, entityID);
}

std::string EngineDLLInterface::getEntityName(EntityID entityID) {
    if (getEntityNameFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityName::ERROR The function ptr is nullptr";
    }
    char buffer[CHAR_BUFFER_SIZE];
    getEntityNameFun(engine, entityID, buffer, CHAR_BUFFER_SIZE);
    return std::string(buffer);
}

EntityID EngineDLLInterface::getEntityParent(EntityID entityID) {
    if (getEntityParentFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityParent::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityParent::ERROR The function ptr is nullptr";
    }
    return getEntityParentFun(engine, entityID);
}

EntityID EngineDLLInterface::getEntityFromName(const char* name) {
    if (getEntityFromNameFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityFromName::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityFromName::ERROR The function ptr is nullptr";
    }
    return getEntityFromNameFun(engine, name);
}

EntityID EngineDLLInterface::getEntityChild(EntityID entityID, std::size_t index) {
    if (getEntityChildFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityChild::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityChild::ERROR The function ptr is nullptr";
    }
    return getEntityChildFun(engine, entityID, index);
}

std::size_t EngineDLLInterface::getComponentsCount(EntityID entityID) {
    if (getComponentsCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getComponentsCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getComponentsCount::ERROR The function ptr is nullptr";
    }
    return getComponentsCountFun(engine, entityID);
}

std::size_t EngineDLLInterface::getEntityChildCount(EntityID entityID) {
    if (getEntityChildCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityChildCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityChildCount::ERROR The function ptr is nullptr";
    }
    return getEntityChildCountFun(engine, entityID);
}

std::size_t EngineDLLInterface::getEntityImmediateChildCount(EntityID entityID) {
    if (getEntityImmediateChildCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getEntityImmediateChildCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getEntityImmediateChildCount::ERROR The function ptr is nullptr";
    }
    return getEntityImmediateChildCountFun(engine, entityID);
}