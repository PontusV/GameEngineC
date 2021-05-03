#include "EngineDLL.h"
#include <iostream>
using namespace Editor;

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
    if (!loadFunctionFromDLL(handle, setAssetDirPathFun, "setAssetDirPath")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, createEntityFun, "createEntity")) {
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
    if (!loadFunctionFromDLL(handle, getTypeIDFromNameFun, "getTypeIDFromName")) {
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
    if (!loadFunctionFromDLL(handle, getPropertyTypeFun, "getPropertyType")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyTypeSizeFun, "getPropertyTypeSize")) {
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
    if (!loadFunctionFromDLL(handle, loadSceneBackupFun, "loadSceneBackup")) {
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
    if (!loadFunctionFromDLL(handle, saveSceneBackupFun, "saveSceneBackup")) {
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
    if (!loadFunctionFromDLL(handle, setEntityRenamedCallbackFun, "setEntityRenamedCallback")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setCallbackPtrFun, "setCallbackPtr")) {
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
    if (!loadFunctionFromDLL(handle, getEntityParentFun, "getEntityParent")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getEntityFromNameFun, "getEntityFromName")) {
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
    setAssetDirPathFun = nullptr;
    createEntityFun = nullptr;
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
    getTypeIDFromNameFun = nullptr;
    getDerivedTypeIDsCountFun = nullptr;
    getAllReflectedTypesCountFun = nullptr;
    getPropertiesCountFun = nullptr;
    getPropertyTypeFun = nullptr;
    getPropertyTypeSizeFun = nullptr;
    getPropertyFieldCountFun = nullptr;
    getPropertyFieldsFun = nullptr;
    getPropertyNameFun = nullptr;
    getPropertyTypeNameFun = nullptr;
    loadSceneFun = nullptr;
    loadSceneBackupFun = nullptr;
    unloadSceneFun = nullptr;
    saveSceneFun = nullptr;
    saveSceneBackupFun = nullptr;
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
    setEntityRenamedCallbackFun = nullptr;
    setCallbackPtrFun = nullptr;
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
    getEntityParentFun = nullptr;
    getEntityFromNameFun = nullptr;
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

std::wstring EngineDLL::getNextDLLName() {
    return altActivated ? L"Engine.dll" : L"Engine_2.dll";
}

std::wstring EngineDLL::getCurrentDLLName() {
    return !altActivated ? L"Engine.dll" : L"Engine_2.dll";
}

bool EngineDLL::isAltActivated() const {
    return altActivated;
}