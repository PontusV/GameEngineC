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
    if (!loadFunctionFromDLL(handle, engineEditorTickFun, "engineEditorTick")) {
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
    if (!loadFunctionFromDLL(handle, loadGameStateFun, "loadGameState")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, saveGameStateFun, "saveGameState")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setCameraPositionFun, "setCameraPosition")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setCameraRotationFun, "setCameraRotation")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setCameraScaleFun, "setCameraScale")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getCameraPositionFun, "getCameraPosition")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getCameraRotationFun, "getCameraRotation")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getCameraScaleFun, "getCameraScale")) {
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
    if (!loadFunctionFromDLL(handle, overridePositionFun, "overridePosition")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, removePositionOverrideFun, "removePositionOverride")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, isPositionOverridenFun, "isPositionOverriden")) {
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
    if (!loadFunctionFromDLL(handle, getPrefabComponentTypeIDFun, "getPrefabComponentTypeID")) {
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
    if (!loadFunctionFromDLL(handle, getPropertyIndexFun, "getPropertyIndex")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyNameFun, "getPropertyName")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyInspectorDataFun, "getPropertyInspectorData")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setPropertyFieldValueFun, "setPropertyFieldValue")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, unpackPrefabFun, "unpackPrefab")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, savePrefabFun, "savePrefab")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, updatePrefabsFun, "updatePrefabs")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, updatePrefabFun, "updatePrefab")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, createPrefabFromEntityFun, "createPrefabFromEntity")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, createPrefabEntityFun, "createPrefabEntity")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, createEntityFromPrefabFun, "createEntityFromPrefab")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, overridePropertyFun, "overrideProperty")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, removePropertyOverrideFun, "removePropertyOverride")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, isEntityPrefabRootFun, "isEntityPrefabRoot")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, isEntityAnOverrideFun, "isEntityAnOverride")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyOverridesFun, "getPropertyOverrides")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyOverridesAtFun, "getPropertyOverridesAt")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getComponentOverridesFun, "getComponentOverrides")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, revertPrefabFun, "revertPrefab")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPrefabFilePathFun, "getPrefabFilePath")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPrefabOverrideReceiverFun, "getPrefabOverrideReceiver")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyOverridesCountFun, "getPropertyOverridesCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getPropertyOverridesAtCountFun, "getPropertyOverridesAtCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getComponentOverridesCountFun, "getComponentOverridesCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, loadPropertyFromBufferFun, "loadPropertyFromBuffer")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, loadComponentFromBufferFun, "loadComponentFromBuffer")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, loadEntityFromBufferFun, "loadEntityFromBuffer")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, writePropertyToBufferFun, "writePropertyToBuffer")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, writeComponentToBufferFun, "writeComponentToBuffer")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, writeEntityToBufferFun, "writeEntityToBuffer")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, destroyEntityFun, "destroyEntity")) {
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
    if (!loadFunctionFromDLL(handle, setEntityNameFun, "setEntityName")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, hasComponentFun, "hasComponent")) {
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
    if (!loadFunctionFromDLL(handle, hasEntityParentFun, "hasEntityParent")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, detachEntityParentFun, "detachEntityParent")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setEntityNameFun, "setEntityName")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getAllEntitiesFun, "getAllEntities")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getComponentsFun, "getComponents")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getEntityNameFun, "getEntityName")) {
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
    if (!loadFunctionFromDLL(handle, getAllEntitiesCountFun, "getAllEntitiesCount")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getEntityParentFun, "getEntityParent")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, getEntityChildFun, "getEntityChild")) {
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
    if (!loadFunctionFromDLL(handle, setEntitiesChangedCallbackFun, "setEntitiesChangedCallback")) {
        unload();
        return false;
    }
    if (!loadFunctionFromDLL(handle, setCallbackPtrFun, "setCallbackPtr")) {
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
    engineEditorTickFun = nullptr;
    setViewportSizeFun = nullptr;
    setAssetDirPathFun = nullptr;
    loadGameStateFun = nullptr;
    saveGameStateFun = nullptr;
    setCameraPositionFun = nullptr;
    setCameraRotationFun = nullptr;
    setCameraScaleFun = nullptr;
    getCameraPositionFun = nullptr;
    getCameraRotationFun = nullptr;
    getCameraScaleFun = nullptr;
    getRectSizeFun = nullptr;
    getMinRectScreenPositionFun = nullptr;
    getWorldPositionFun = nullptr;
    getLocalPositionFun = nullptr;
    setLocalPositionFun = nullptr;
    setWorldPositionFun = nullptr;
    overridePositionFun = nullptr;
    removePositionOverrideFun = nullptr;
    isPositionOverridenFun = nullptr;
    hasAnnotationFun = nullptr;
    getTypeNameFun = nullptr;
    onUpdateFun = nullptr;
    getDerivedTypeIDsFun = nullptr;
    getAllReflectedTypesFun = nullptr;
    getTypeIDFromNameFun = nullptr;
    getPrefabComponentTypeIDFun = nullptr;
    getDerivedTypeIDsCountFun = nullptr;
    getAllReflectedTypesCountFun = nullptr;
    getPropertiesCountFun = nullptr;
    getPropertyIndexFun = nullptr;
    getPropertyNameFun = nullptr;
    getPropertyInspectorDataFun = nullptr;
    setPropertyFieldValueFun = nullptr;
    unpackPrefabFun = nullptr;
    savePrefabFun = nullptr;
    updatePrefabsFun = nullptr;
    updatePrefabFun = nullptr;
    createPrefabFromEntityFun = nullptr;
    createPrefabEntityFun = nullptr;
    createEntityFromPrefabFun = nullptr;
    overridePropertyFun = nullptr;
    removePropertyOverrideFun = nullptr;
    isEntityPrefabRootFun = nullptr;
    isEntityAnOverrideFun = nullptr;
    getPropertyOverridesFun = nullptr;
    getPropertyOverridesAtFun = nullptr;
    getComponentOverridesFun = nullptr;
    revertPrefabFun = nullptr;
    getPrefabFilePathFun = nullptr;
    getPrefabOverrideReceiverFun = nullptr;
    getPropertyOverridesCountFun = nullptr;
    getPropertyOverridesAtCountFun = nullptr;
    getComponentOverridesCountFun = nullptr;
    loadPropertyFromBufferFun = nullptr;
    loadComponentFromBufferFun = nullptr;
    loadEntityFromBufferFun = nullptr;
    writePropertyToBufferFun = nullptr;
    writeComponentToBufferFun = nullptr;
    writeEntityToBufferFun = nullptr;
    destroyEntityFun = nullptr;
    addComponentFun = nullptr;
    removeComponentFun = nullptr;
    setEntityNameFun = nullptr;
    hasComponentFun = nullptr;
    isEntityChildFun = nullptr;
    setEntityParentFun = nullptr;
    hasEntityParentFun = nullptr;
    detachEntityParentFun = nullptr;
    setEntityNameFun = nullptr;
    getAllEntitiesFun = nullptr;
    getComponentsFun = nullptr;
    getEntityNameFun = nullptr;
    getComponentsCountFun = nullptr;
    getEntityChildCountFun = nullptr;
    getEntityImmediateChildCountFun = nullptr;
    getAllEntitiesCountFun = nullptr;
    getEntityParentFun = nullptr;
    getEntityChildFun = nullptr;
    createEntityFun = nullptr;
    getEntityAtPosFun = nullptr;
    setEntitiesChangedCallbackFun = nullptr;
    setCallbackPtrFun = nullptr;
    // End of reset function ptrs
    FreeLibrary(handle);
    handle = NULL;
    return true;
}

bool EngineDLL::isLoaded() const {
    return handle != NULL;
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