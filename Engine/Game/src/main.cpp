// Engine
#pragma comment(linker, "/export:createEngine")
#pragma comment(linker, "/export:releaseEngine")
#pragma comment(linker, "/export:createTemplateEntity")
#pragma comment(linker, "/export:engineInit")
#pragma comment(linker, "/export:engineTick")
#pragma comment(linker, "/export:engineEditorTick")
#pragma comment(linker, "/export:setViewportSize")
#pragma comment(linker, "/export:createEntity")
#pragma comment(linker, "/export:getEntityAtPos")
#pragma comment(linker, "/export:setAssetDirPath")
// Camera
#pragma comment(linker, "/export:setCameraPosition")
#pragma comment(linker, "/export:setCameraRotation")
#pragma comment(linker, "/export:setCameraScale")
#pragma comment(linker, "/export:getCameraPosition")
#pragma comment(linker, "/export:getCameraRotation")
#pragma comment(linker, "/export:getCameraScale")
// Transform / RectTransform
#pragma comment(linker, "/export:getRectSize")
#pragma comment(linker, "/export:getMinRectScreenPosition")
#pragma comment(linker, "/export:getWorldPosition")
#pragma comment(linker, "/export:getLocalPosition")
#pragma comment(linker, "/export:setLocalPosition")
#pragma comment(linker, "/export:setWorldPosition")
// Reflection general
#pragma comment(linker, "/export:hasAnnotation")
#pragma comment(linker, "/export:getTypeName")
#pragma comment(linker, "/export:onUpdate")
#pragma comment(linker, "/export:getDerivedTypeIDs")
#pragma comment(linker, "/export:getAllReflectedTypes")
#pragma comment(linker, "/export:getTypeIDFromName")
#pragma comment(linker, "/export:getDerivedTypeIDsCount")
#pragma comment(linker, "/export:getAllReflectedTypesCount")
// Reflection properties
#pragma comment(linker, "/export:getPropertiesCount")
#pragma comment(linker, "/export:getPropertyType")
#pragma comment(linker, "/export:getPropertyTypeSize")
#pragma comment(linker, "/export:getPropertyFieldCount")
#pragma comment(linker, "/export:getPropertyFields")
#pragma comment(linker, "/export:getPropertyName")
#pragma comment(linker, "/export:getPropertyTypeName")
// Scene
#pragma comment(linker, "/export:loadScene")
#pragma comment(linker, "/export:loadSceneBackup")
#pragma comment(linker, "/export:unloadScene")
#pragma comment(linker, "/export:saveScene")
#pragma comment(linker, "/export:saveSceneBackup")
#pragma comment(linker, "/export:createScene")
#pragma comment(linker, "/export:destroyEntity")
#pragma comment(linker, "/export:getAllEntities")
#pragma comment(linker, "/export:getSceneName")
#pragma comment(linker, "/export:getSceneFilePath")
#pragma comment(linker, "/export:hasSceneChanged")
#pragma comment(linker, "/export:getSceneCount")
#pragma comment(linker, "/export:getAllEntitiesCount")
// Entity
#pragma comment(linker, "/export:addComponent")
#pragma comment(linker, "/export:removeComponent")
#pragma comment(linker, "/export:renameEntity")
#pragma comment(linker, "/export:getComponents")
#pragma comment(linker, "/export:isEntityChild")
#pragma comment(linker, "/export:setEntityParent")
#pragma comment(linker, "/export:isEntityNameAvailable")
#pragma comment(linker, "/export:hasEntityParent")
#pragma comment(linker, "/export:detachEntityParent")
#pragma comment(linker, "/export:getEntityName")
#pragma comment(linker, "/export:getEntityParent")
#pragma comment(linker, "/export:getEntityFromName")
#pragma comment(linker, "/export:getEntityChild")
#pragma comment(linker, "/export:getComponentsCount")
#pragma comment(linker, "/export:getEntityChildCount")
#pragma comment(linker, "/export:getEntityImmediateChildCount")
// Callback
#pragma comment(linker, "/export:setSceneAddedCallback")
#pragma comment(linker, "/export:setSceneRemovedCallback")
#pragma comment(linker, "/export:setEntityRenamedCallback")
#pragma comment(linker, "/export:setCallbackPtr")


#include "engine/Engine.h"
#include <cstddef>
using namespace Core;



/* Used by EXE */
int main(int argc, char* argv[]) {
	Engine engine;
	std::size_t init = engine.initiate();
	if (init != 0) return init;
	// TODO: Load scene 
	return engine.start();
}