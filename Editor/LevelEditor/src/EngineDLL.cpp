#include "EngineDLL.h"
#include <iostream>
using namespace Editor;

#define CHAR_BUFFER_SIZE 256

EngineDLL::EngineDLL() : handle(NULL) {}
EngineDLL::~EngineDLL() {
    if (isLoaded()) {
        std::cout << "EngineDLL::WARNING Destructor called before unloading the Engine DLL. This may cause memory leaks." << std::endl;
        unload();
    }
}

template<typename T>
bool loadFunctionFromDLL(HINSTANCE& handle, T& var, const char* functionName) {
    var = (T)(GetProcAddress(handle, functionName));
    if (var == nullptr) {
        std::cout << "Failed to get address of " << functionName << " function from DLL at specified path" << std::endl;
        return false;
    }
    return true;
}

bool EngineDLL::load(const wchar_t* path) {
    if (handle != NULL) {
        std::cout << "Unable to load DLL because one is currently already loaded" << std::endl;
        return false;
    }
    handle = LoadLibrary(path);
    if (handle == NULL) {
        std::wcout << L"Failed to create handle for DLL at specified path: " << path << std::endl;
        return false;
    }

    if (!loadFunctionFromDLL(handle, createEngineFun, "createEngine")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, releaseEngineFun, "releaseEngine")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, createTemplateEntityFun, "createTemplateEntity")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, engineInitFun, "engineInit")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, engineTickFun, "engineTick")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getCameraPositionFun, "getCameraPosition")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setCameraPositionFun, "setCameraPosition")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setViewportSizeFun, "setViewportSize")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getEntityAtPosFun, "getEntityAtPos")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getRectSizeFun, "getRectSize")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getMinRectScreenPositionFun, "getMinRectScreenPosition")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getWorldPositionFun, "getWorldPosition")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getLocalPositionFun, "getLocalPosition")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setLocalPositionFun, "setLocalPosition")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setWorldPositionFun, "setWorldPosition")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, hasAnnotationFun, "hasAnnotation")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getTypeNameFun, "getTypeName")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, onUpdateFun, "onUpdate")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getDerivedTypeIDsFun, "getDerivedTypeIDs")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getAllReflectedTypesFun, "getAllReflectedTypes")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getDerivedTypeIDsCountFun, "getDerivedTypeIDsCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getAllReflectedTypesCountFun, "getAllReflectedTypesCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertiesCountFun, "getPropertiesCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyIDFun, "getPropertyID")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyTypeFun, "getPropertyType")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyFieldCountFun, "getPropertyFieldCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyFieldsFun, "getPropertyFields")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyNameFun, "getPropertyName")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyTypeNameFun, "getPropertyTypeName")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, loadSceneFun, "loadScene")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, unloadSceneFun, "unloadScene")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, saveSceneFun, "saveScene")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, createSceneFun, "createScene")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, destroyEntityFun, "destroyEntity")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getAllEntitiesFun, "getAllEntities")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getSceneNameFun, "getSceneName")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getSceneFilePathFun, "getSceneFilePath")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, hasSceneChangedFun, "hasSceneChanged")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getSceneCountFun, "getSceneCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getAllEntitiesCountFun, "getAllEntitiesCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setSceneAddedCallbackFun, "setSceneAddedCallback")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setSceneRemovedCallbackFun, "setSceneRemovedCallback")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setSceneCallbackPtrFun, "setSceneCallbackPtr")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getComponentsFun, "getComponents")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, isEntityChildFun, "isEntityChild")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setEntityParentFun, "setEntityParent")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, addComponentFun, "addComponent")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, removeComponentFun, "removeComponent")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, renameEntityFun, "renameEntity")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, isEntityNameAvailableFun, "isEntityNameAvailable")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, hasEntityParentFun, "hasEntityParent")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, detachEntityParentFun, "detachEntityParent")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getEntityNameFun, "getEntityName")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getEntityChildFun, "getEntityChild")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getComponentsCountFun, "getComponentsCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getEntityChildCountFun, "getEntityChildCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getEntityImmediateChildCountFun, "getEntityImmediateChildCount")) {
        unload();
        return false;
    }

    if (createEngine() == nullptr) {
        std::cout << "Failed to create Engine. Unloading DLL..." << std::endl;
        unload();
        return false;
    }
    altActivated = !altActivated;
    return true;
}

bool EngineDLL::unload() {
    if (handle == NULL) {
        std::cout << "Unable to unload DLL because none is currently loaded" << std::endl;
        return false;
    }
    releaseEngine();
    // Reset function ptrs
    createEngineFun = nullptr;
    releaseEngineFun = nullptr;
    createTemplateEntityFun = nullptr;
    engineInitFun = nullptr;
    engineTickFun = nullptr;
    getCameraPositionFun = nullptr;
    setCameraPositionFun = nullptr;
    setViewportSizeFun = nullptr;
    getEntityAtPosFun = nullptr;
    getRectSizeFun = nullptr;
    getMinRectScreenPositionFun = nullptr;
    getWorldPositionFun = nullptr;
    getLocalPositionFun = nullptr;
    setLocalPositionFun = nullptr;
    setWorldPositionFun = nullptr;
    hasAnnotationFun = nullptr;
    getTypeNameFun = nullptr;
    onUpdateFun = nullptr;
    getDerivedTypeIDsFun = nullptr;
    getAllReflectedTypesFun = nullptr;
    getDerivedTypeIDsCountFun = nullptr;
    getAllReflectedTypesCountFun = nullptr;
    getPropertiesCountFun = nullptr;
    getPropertyIDFun = nullptr;
    getPropertyTypeFun = nullptr;
    getPropertyFieldCountFun = nullptr;
    getPropertyFieldsFun = nullptr;
    getPropertyNameFun = nullptr;
    getPropertyTypeNameFun = nullptr;
    loadSceneFun = nullptr;
    unloadSceneFun = nullptr;
    saveSceneFun = nullptr;
    createSceneFun = nullptr;
    destroyEntityFun = nullptr;
    getAllEntitiesFun = nullptr;
    getSceneNameFun = nullptr;
    getSceneFilePathFun = nullptr;
    hasSceneChangedFun = nullptr;
    getSceneCountFun = nullptr;
    getAllEntitiesCountFun = nullptr;
    setSceneAddedCallbackFun = nullptr;
    setSceneRemovedCallbackFun = nullptr;
    setSceneCallbackPtrFun = nullptr;
    addComponentFun = nullptr;
    removeComponentFun = nullptr;
    renameEntityFun = nullptr;
    getComponentsFun = nullptr;
    isEntityChildFun = nullptr;
    setEntityParentFun = nullptr;
    isEntityNameAvailableFun = nullptr;
    hasEntityParentFun = nullptr;
    detachEntityParentFun = nullptr;
    getEntityNameFun = nullptr;
    getEntityChildFun = nullptr;
    getComponentsCountFun = nullptr;
    getEntityChildCountFun = nullptr;
    getEntityImmediateChildCountFun = nullptr;
    // End of reset function ptrs
    FreeLibrary(handle);
    handle = NULL;
    return true;
}

bool EngineDLL::isLoaded() const {
    return handle != NULL;
}

std::size_t EngineDLL::getEntitySceneIndex(EntityID entityID) {
    if (!isLoaded() || entityID == 0) return 0;
    std::size_t sceneCount = getSceneCount();
    for (std::size_t i = 0; i < sceneCount; i++) {
        std::vector<EntityID> entities = getAllEntities(i);
        for (EntityID& id : entities) {
            if (id == entityID) return i;
        }
    }
    std::cout << "EngineDLL::getEntitySceneIndex::ERROR Failed to find sceneIndex of Entity with ID: " << entityID << std::endl;
    return 0;
}

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
        properties[i].id = getPropertyID(typeID, i);
        properties[i].index = i;
        properties[i].name = getPropertyName(typeID, i);
        properties[i].renderer = static_cast<InspectorFieldRenderType>(getPropertyType(typeID, i));
        properties[i].typeName = getPropertyTypeName(typeID, i);
        properties[i].fieldPtrs = getPropertyFields(typeID, i, instance);
    }
    return properties;
}

std::size_t EngineDLLInterface::getPropertyID(TypeID typeID, std::size_t propIndex) {
    if (getPropertyIDFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyID::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyID::ERROR The function ptr is nullptr";
    }
    return getPropertyIDFun(typeID, propIndex);
}

std::size_t EngineDLLInterface::getPropertyType(TypeID typeID, std::size_t propIndex) {
    if (getPropertyTypeFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyType::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyType::ERROR The function ptr is nullptr";
    }
    return getPropertyTypeFun(typeID, propIndex);
}

std::size_t EngineDLLInterface::getPropertyFieldCount(TypeID typeID, std::size_t propIndex, void* instance) {
    if (getPropertyFieldCountFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyFieldCount::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyFieldCount::ERROR The function ptr is nullptr";
    }
    return getPropertyFieldCountFun(typeID, propIndex, instance);
}

std::vector<void*> EngineDLLInterface::getPropertyFields(TypeID typeID, std::size_t propIndex, void* instance) {
    if (getPropertyFieldsFun == nullptr) {
        std::cout << "EngineDLLInterface::getPropertyFields::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::getPropertyFields::ERROR The function ptr is nullptr";
    }
    std::size_t outSize = getPropertyFieldCount(typeID, propIndex, instance);
    std::vector<void*> fieldValuePtrs(outSize);
    getPropertyFieldsFun(typeID, propIndex, instance, &fieldValuePtrs[0], outSize);
    return fieldValuePtrs;
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

bool EngineDLLInterface::unloadScene(std::size_t sceneIndex) {
    if (unloadSceneFun == nullptr) {
        std::cout << "EngineDLLInterface::unloadScene::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::unloadScene::ERROR The function ptr is nullptr";
    }
    return unloadSceneFun(engine, sceneIndex);
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

void EngineDLLInterface::setSceneCallbackPtr(void* ptr) {
    if (setSceneCallbackPtrFun == nullptr) {
        std::cout << "EngineDLLInterface::setSceneCallbackPtr::ERROR The function ptr is nullptr" << std::endl;
        throw "EngineDLLInterface::setSceneCallbackPtr::ERROR The function ptr is nullptr";
    }
    setSceneCallbackPtrFun(engine, ptr);
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
    std::vector<void*> componentPtrs(outSize);
    std::vector<TypeID> componentTypeIDs(outSize);
    getComponentsFun(engine, entityID, &componentPtrs[0], &componentTypeIDs[0], outSize);
    std::vector<ComponentData> components(outSize);
    for (std::size_t i = 0; i < outSize; i++) {
        components[i].instance = componentPtrs[i];
        components[i].typeID = componentTypeIDs[i];
    }
    return components;
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
    return setEntityParentFun(engine, entityID, parentID);
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

std::wstring EngineDLL::getNextDLLName() {
    return altActivated ? L"Engine.dll" : L"Engine_2.dll";
}

std::wstring EngineDLL::getCurrentDLLName() {
    return !altActivated ? L"Engine.dll" : L"Engine_2.dll";
}

bool EngineDLL::isAltActivated() const {
    return altActivated;
}