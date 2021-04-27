#include "Core.h"
#include "entity/Entity.h"
#include "engine/Engine.h"
#include "components/graphics/Image.h"
#include "components/graphics/SpriteRenderer.h"
#include "components/RectTransform.h"
#include "components/entity/ParentEntity.h"
#include "components/entity/ChildManager.h"
#include "ReflectionPolymorph.generated.h"

#include <algorithm>
#include "maths/MatrixTransform.h"
#include "utils/string.h"
using namespace Core;

// Utils
template<typename T>
bool vectorIncludes(const std::vector<T>& vec, T value) {
	for (const T& e : vec) {
		if (e == value) return true;
	}
	return false;
}

void copyString(const std::string& src, char* dest, std::size_t destSize) {
	std::size_t size = std::min(src.size(), destSize - 1);
	strncpy_s(dest, destSize, src.c_str(), size);
	dest[size] = '\0';
}

InspectorFieldRenderType getRendererFromProp(const Mirror::Property& prop) {
	if (prop.type.isBool()) {
		return InspectorFieldRenderType::BOOL;
	}
	if (prop.type.isDecimal()) {
		return InspectorFieldRenderType::DECIMAL;
	}
	if (prop.type.isSignedNumber()) {
		if (prop.type.isChar())
			return InspectorFieldRenderType::SIGNED_CHAR;
		return InspectorFieldRenderType::SIGNED_NUMBER;
	}
	if (prop.type.isUnsignedNumber()) {
		if (prop.type.isChar())
			return InspectorFieldRenderType::UNSIGNED_CHAR;
		return InspectorFieldRenderType::UNSIGNED_NUMBER;
	}
	if (prop.type.isString()) {
		if (vectorIncludes<std::string>(prop.getAnnotationValue("Category"), "ImagePath"))
			return InspectorFieldRenderType::IMAGE_PATH;
		if (vectorIncludes<std::string>(prop.getAnnotationValue("Category"), "ShaderPath"))
			return InspectorFieldRenderType::SHADER_PATH;
		return InspectorFieldRenderType::STRING;
	}
	if (prop.type.isWideString()) {
		return InspectorFieldRenderType::WIDE_STRING;
	}
	if (prop.type.name == "Core::Vector2") {
		return InspectorFieldRenderType::VECTOR2;
	}
	if (prop.type.name == "Core::Color") {
		return InspectorFieldRenderType::COLOR;
	}
	if (prop.type.name == "Core::Font") {
		return InspectorFieldRenderType::FONT;
	}
	return InspectorFieldRenderType::NONE;
}

template<std::size_t I>
void createPropFieldsRecursive(PropertyFieldData& data) {} // End of recursion

template<std::size_t I, typename T, typename... Ts>
void createPropFieldsRecursive(PropertyFieldData& data, T& arg, Ts&... args) {
	data.ptrBuffer[I] = &arg;
	data.sizeBuffer[I] = sizeof(arg);
	createPropFieldsRecursive<I + 1>(data, args...);
}

template<typename... Ts>
PropertyFieldData createPropFields(Ts&... args) {
	PropertyFieldData data;
	data.count = sizeof...(args);
	createPropFieldsRecursive<0>(data, args...);
	return data;
}

PropertyFieldData getFieldDataFromProp(const Mirror::Property& prop, void* instance, TypeID typeID) {
	auto renderer = getRendererFromProp(prop);
	if (renderer == InspectorFieldRenderType::VECTOR2) {
		Vector2* vec = static_cast<Vector2*>(Mirror::getPointer(prop, instance, typeID));
		return createPropFields(vec->x, vec->y);
	}
	else if (renderer == InspectorFieldRenderType::COLOR) {
		Color* color = static_cast<Color*>(Mirror::getPointer(prop, instance, typeID));
		return createPropFields(color->r, color->g, color->b, color->a);
	}
	else if (renderer == InspectorFieldRenderType::FONT) {
		Font* font = static_cast<Font*>(Mirror::getPointer(prop, instance, typeID));
		return createPropFields(font->fileName, font->size);
	}
	else {
		Vector2* vec = static_cast<Vector2*>(Mirror::getPointer(prop, instance, typeID));
		PropertyFieldData data;
		data.count = 1;
		data.ptrBuffer[0] = Mirror::getPointer(prop, instance, typeID);
		data.sizeBuffer[0] = prop.type.size;
		return data;
	}
}
// End of Utils

Engine* createEngine() {
	return new Engine();
}

void releaseEngine(Engine* ptr) {
	delete ptr;
}

bool createTemplateEntity(Core::Engine* engine, std::size_t sceneIndex, const char* name, float x, float y, float width, float height) {
	SceneManager& sceneManager = engine->getSceneManager();
	ScenePtr scene = sceneManager.getScene(sceneIndex);
	if (scene == nullptr) return false;
	scene->createEntity(name,
		Image("resources/images/awesomeface.png"),
		RectTransform(x, y, width, height, 0.0f, Alignment::CENTER),
		SpriteRenderer()
	);
	return true;
}

bool engineInit(Core::Engine* engine, GLADloadproc ptr, int screenWidth, int screenHeight) {
	Window& window = engine->getGraphics().getWindow();
	window.setActive(true);
	window.setResolution(screenWidth, screenHeight);
	window.setBackgroundColor(0.15f, 0.15f, 0.15f);
	window.initGLAD(ptr);

	int result = engine->initiate(true);
	if (result != 0) { // Failed to initiate the engine
		return false;
	}
	return true;
}

void engineTick(Core::Engine* engine, float deltaTime) {
	engine->tick(deltaTime);
}

void getCameraPosition(Core::Engine* engine, float* out) {
	Camera& camera = engine->getGraphics().getCamera();
	Vector2 vec = camera.getPosition();
	out[0] = vec.x;
	out[1] = vec.y;
}

void setCameraPosition(Core::Engine* engine, float x, float y) {
	Camera& camera = engine->getGraphics().getCamera();
	camera.setPosition(x, y);
}

void setViewportSize(Core::Engine* engine, float width, float height) {
	engine->resizeViewport(width, height);
}

EntityID createEntity(Core::Engine* engine, std::size_t sceneIndex, const char* name) {
	SceneManager& sceneManager = engine->getSceneManager();
	ScenePtr scene = sceneManager.getScene(sceneIndex);
	if (scene == nullptr) return Entity::INVALID_ID;
	Entity entity = scene->createEmptyEntity(name);
	return entity.getID();
}

EntityID getEntityAtPos(Core::Engine* engine, float x, float y) {
	Input& input = engine->getInput();
	Entity entity = input.getEntityAtPos(x, y);
	return entity.getID();
}

bool getRectSize(Core::Engine* engine, EntityID entityID, float* out) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		Rect rect = rectTransform->getRect();
		out[0] = rect.w;
		out[1] = rect.h;
		return true;
	}
	return false;
}

bool getMinRectScreenPosition(Core::Engine* engine, EntityID entityID, float* out) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Camera& camera = engine->getGraphics().getCamera();
	Entity entity = Entity(entityID);

	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		Vector2 rectOffset = rectTransform->getRectOffset();
		Vector2 screenPosition = camera.getViewMatrix() * rectTransform->getPosition();
		Vector2 minPosition = Vector2(screenPosition.x + rectOffset.x, screenPosition.y + rectOffset.y);
		out[0] = minPosition.x;
		out[1] = minPosition.y;
		return true;
	}
	return false;
}

bool getWorldPosition(Core::Engine* engine, EntityID entityID, float* out) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		Vector2 vec = rectTransform->getPosition();
		out[0] = vec.x;
		out[1] = vec.y;
		return true;
	}
	return false;
}

bool getLocalPosition(Core::Engine* engine, EntityID entityID, float* out) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		Vector2 vec = rectTransform->getLocalPosition();
		out[0] = vec.x;
		out[1] = vec.y;
		return true;
	}
	return false;
}

bool setLocalPosition(Core::Engine* engine, EntityID entityID, float x, float y) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		rectTransform->setLocalPosition(Vector2(x, y));
		return true;
	}
	return false;
}

bool setWorldPosition(Core::Engine* engine, EntityID entityID, float x, float y) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (RectTransform* rectTransform = entityManager.getComponent<RectTransform>(entity)) {
		rectTransform->setPosition(Vector2(x, y));
		return true;
	}
	return false;
}

bool hasAnnotation(TypeID typeID, const char* annotation) {
	Mirror::Class type = Mirror::getType(typeID);
	return type.hasAnnotation(annotation);
}

void getTypeName(TypeID typeID, char* out, std::size_t outSize) {
	copyString(Mirror::getName(typeID), out, outSize);
}

void onUpdate(void* instance, TypeID typeID, std::size_t propIndex) {
	Mirror::Class type = Mirror::getType(typeID);
	assert(propIndex < type.properties.size());
	Mirror::onUpdate(instance, typeID, type.properties[propIndex]);
}

void getDerivedTypeIDs(TypeID typeID, TypeID* out, std::size_t outSize) {
	std::vector<std::size_t> typeIDs = Mirror::polyGetDerivedTypeIDs(typeID);
	std::size_t size = std::min(outSize, typeIDs.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = typeIDs[i];
	}
}

void getAllReflectedTypes(TypeID* out, std::size_t outSize) {
	std::vector<Mirror::ReflectedType> types = Mirror::getAllReflectedTypes();
	std::size_t size = std::min(outSize, types.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = types[i].typeID;
	}
}

TypeID getTypeIDFromName(const char* name) {
	if (name == nullptr) return 0; // Invalid ID
	return Mirror::getTypeID(name);
}

std::size_t getDerivedTypeIDsCount(TypeID typeID) {
	return Mirror::polyGetDerivedTypeIDs(typeID).size();
}

std::size_t getAllReflectedTypesCount() {
	return Mirror::getAllReflectedTypes().size();
}

std::size_t getPropertiesCount(TypeID typeID) {
	Mirror::Class type = Mirror::getType(typeID);
	return type.properties.size();
}

std::size_t getPropertyType(TypeID typeID, std::size_t propIndex) {
	Mirror::Class type = Mirror::getType(typeID);
	assert(propIndex < type.properties.size());
	Mirror::Property& prop = type.properties[propIndex];
	return static_cast<std::size_t>(getRendererFromProp(prop));
}

std::size_t getPropertyTypeSize(TypeID typeID, std::size_t propIndex) {
	Mirror::Class type = Mirror::getType(typeID);
	assert(propIndex < type.properties.size());
	Mirror::Property& prop = type.properties[propIndex];
	return prop.type.size;
}

std::size_t getPropertyFieldCount(TypeID typeID, std::size_t propIndex, void* instance) {
	Mirror::Class type = Mirror::getType(typeID);
	assert(propIndex < type.properties.size());
	Mirror::Property& prop = type.properties[propIndex];
	auto fieldData = getFieldDataFromProp(prop, instance, typeID);
	return fieldData.count;
}

void getPropertyFields(TypeID typeID, std::size_t propIndex, void* instance, void** out, std::size_t* outFieldSize, std::size_t outSize) {
	Mirror::Class type = Mirror::getType(typeID);
	assert(propIndex < type.properties.size());
	Mirror::Property& prop = type.properties[propIndex];
	auto fieldData = getFieldDataFromProp(prop, instance, typeID);
	std::size_t size = std::min(fieldData.count, outSize);
	for (std::size_t i = 0; i < size; i++) {
		out[i] = fieldData.ptrBuffer[i];
		outFieldSize[i] = fieldData.sizeBuffer[i];
	}
}

void getPropertyName(TypeID typeID, std::size_t propIndex, char* out, std::size_t outSize) {
	Mirror::Class type = Mirror::getType(typeID);
	assert(propIndex < type.properties.size());
	Mirror::Property& prop = type.properties[propIndex];
	copyString(prop.name, out, outSize);
}

void getPropertyTypeName(TypeID typeID, std::size_t propIndex, char* out, std::size_t outSize) {
	copyString(Mirror::getName(typeID), out, outSize);
}

bool loadScene(Core::Engine* engine, const char* path) {
	SceneManager& sceneManager = engine->getSceneManager();
	std::wstring decodedPath = utf8_decode(path);
	ScenePtr scene = sceneManager.loadScene(decodedPath.c_str());
	return scene != nullptr;
}

bool loadSceneBackup(Core::Engine* engine, const char* srcPath, const char* destPath) {
	SceneManager& sceneManager = engine->getSceneManager();
	std::wstring decodedSrcPath = utf8_decode(srcPath);
	std::wstring decodedDestPath = utf8_decode(destPath);
	ScenePtr scene = sceneManager.loadScene(decodedSrcPath.c_str(), decodedDestPath.c_str());
	return scene != nullptr;
}

bool unloadScene(Core::Engine* engine, std::size_t sceneIndex) {
	SceneManager& sceneManager = engine->getSceneManager();
	return sceneManager.unloadScene(sceneIndex);
}

bool saveSceneBackup(Core::Engine* engine, std::size_t sceneIndex, const char* path) {
	SceneManager& sceneManager = engine->getSceneManager();
	std::wstring decodedPath = utf8_decode(path);
	return sceneManager.saveScene(sceneIndex, decodedPath.c_str(), false);
}

bool saveScene(Core::Engine* engine, std::size_t sceneIndex) {
	SceneManager& sceneManager = engine->getSceneManager();
	return sceneManager.saveScene(sceneIndex);
}

bool createScene(Core::Engine* engine, const char* name, const char* path) {
	SceneManager& sceneManager = engine->getSceneManager();
	std::wstring decodedName = utf8_decode(name);
	std::wstring decodedPath = utf8_decode(path);
	ScenePtr scene = sceneManager.createScene(decodedName);
	if (scene == nullptr) return false;
	scene->awake();
	return sceneManager.saveScene(scene, decodedPath.c_str());
}

bool destroyEntity(Core::Engine* engine, std::size_t sceneIndex, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return false;
	SceneManager& sceneManager = engine->getSceneManager();
	Entity entity = Entity(entityID);
	ScenePtr scene = sceneManager.getScene(sceneIndex);
	if (scene == nullptr) return false;
	return scene->destroyEntity(entity);
}

bool getAllEntities(Core::Engine* engine, std::size_t sceneIndex, EntityID* out, std::size_t outSize) {
	SceneManager& sceneManager = engine->getSceneManager();
	ScenePtr scene = sceneManager.getScene(sceneIndex);
	if (scene == nullptr) return false;
	std::vector<Handle>& entities = scene->getAllEntities();
	std::size_t size = std::min(outSize, entities.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = entities[i].getEntity().getID();
	}
	return true;
}

bool getSceneName(Core::Engine* engine, std::size_t sceneIndex, char* out, std::size_t outSize) {
	SceneManager& sceneManager = engine->getSceneManager();
	ScenePtr scene = sceneManager.getScene(sceneIndex);
	if (scene == nullptr) return false;
	std::string encodedName = utf8_encode(scene->getName());
	copyString(encodedName, out, outSize);
	return true;
}

bool getSceneFilePath(Core::Engine* engine, std::size_t sceneIndex, char* out, std::size_t outSize) {
	SceneManager& sceneManager = engine->getSceneManager();
	if (const wchar_t* filePath = sceneManager.getSceneFilePath(sceneIndex)) {
		std::string encodedPath = utf8_encode(filePath);
		copyString(encodedPath, out, outSize);
		return true;
	}
	return false;
}

bool hasSceneChanged(Core::Engine* engine, std::size_t sceneIndex) {
	SceneManager& sceneManager = engine->getSceneManager();
	ScenePtr scene = sceneManager.getScene(sceneIndex);
	if (scene == nullptr) return false;
	return scene->hasEntitiesChanged();
}

std::size_t getSceneCount(Core::Engine* engine) {
	SceneManager& sceneManager = engine->getSceneManager();
	return sceneManager.getAllScenesCount();
}

std::size_t getAllEntitiesCount(Core::Engine* engine, std::size_t sceneIndex) {
	SceneManager& sceneManager = engine->getSceneManager();
	ScenePtr scene = sceneManager.getScene(sceneIndex);
	if (scene == nullptr) return 0;
	std::vector<Handle>& entities = scene->getAllEntities();
	return entities.size();
}

void setSceneAddedCallback(Core::Engine* engine, SceneAddedCallbackFun fun) {
	SceneManager& sceneManager = engine->getSceneManager();
	sceneManager.setSceneAddedCallback(fun);
}

void setSceneRemovedCallback(Core::Engine* engine, SceneRemovedCallbackFun fun) {
	SceneManager& sceneManager = engine->getSceneManager();
	sceneManager.setSceneRemovedCallback(fun);
}

void setCallbackPtr(Core::Engine* engine, void* ptr) {
	SceneManager& sceneManager = engine->getSceneManager();
	EntityManager& entityManager = engine->getEntityManager();
	sceneManager.setCallbackPtr(ptr);
	entityManager.setCallbackPtr(ptr);
}

void setEntityRenamedCallback(Core::Engine* engine, EntityRenamedCallbackFun callback) {
	EntityManager& entityManager = engine->getEntityManager();
	entityManager.setEntityRenamedCallback(callback);
}

bool isEntityChild(Core::Engine* engine, EntityID entityID, EntityID parentID) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	ParentEntity* component = entityManager.getComponent<ParentEntity>(entity);
	if (component == nullptr) return false;
	return parentID == component->getParent().getEntity().getID();
}

bool setEntityParent(Core::Engine* engine, std::size_t sceneIndex, EntityID entityID, EntityID parentID) {
	if (entityID == Entity::INVALID_ID) return false;
	SceneManager& sceneManager = engine->getSceneManager();
	Entity entity = Entity(entityID);
	Entity parentEntity = Entity(parentID);
	ScenePtr scene = sceneManager.getScene(sceneIndex);
	if (scene == nullptr) return false;
	Handle handle = scene->getEntityHandle(entity);
	Handle parentHandle = scene->getEntityHandle(parentEntity);
	scene->setParent(handle, parentHandle, true);
	return true;
}

bool isEntityNameAvailable(Core::Engine* engine, const char* name) {
	EntityManager& entityManager = engine->getEntityManager();
	return entityManager.isEntityNameAvailable(name);
}

bool hasEntityParent(Core::Engine* engine, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	return entityManager.hasComponent<ParentEntity>(entity);
}

bool detachEntityParent(Core::Engine* engine, std::size_t sceneIndex, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return false;
	SceneManager& sceneManager = engine->getSceneManager();
	Entity entity = Entity(entityID);
	ScenePtr scene = sceneManager.getScene(sceneIndex);
	if (scene == nullptr) return false;
	Handle handle = scene->getEntityHandle(entity);
	handle.removeParent(true);
	return true;
}

void getEntityName(Core::Engine* engine, EntityID entityID, char* out, std::size_t outSize) {
	if (entityID == Entity::INVALID_ID) {
		out[0] = '\0';
		return;
	}
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	std::string name = entityManager.getEntityName(entity);
	copyString(name, out, outSize);
}

EntityID getEntityFromName(Core::Engine* engine, const char* name) {
	if (name == nullptr) return Entity::INVALID_ID;
	EntityManager& entityManager = engine->getEntityManager();
	return entityManager.getEntity(name).getID();
}

EntityID getEntityParent(Core::Engine* engine, EntityID entityID) {
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	if (ParentEntity* parentEntity = entityManager.getComponent<ParentEntity>(entity)) {
		return parentEntity->getParent().getEntity().getID();
	}
	return Entity::INVALID_ID;
}

EntityID getEntityChild(Core::Engine* engine, EntityID entityID, std::size_t index) {
	if (entityID == Entity::INVALID_ID) return Entity::INVALID_ID;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	ChildManager* component = entityManager.getComponent<ChildManager>(entity);
	if (component == nullptr) return 0;
	return component->getChild(index).getEntity().getID();
}

std::size_t getEntityChildCount(Core::Engine* engine, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return 0;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	ChildManager* component = entityManager.getComponent<ChildManager>(entity);
	if (component == nullptr) return 0;
	std::size_t immediateChildCount = component->getChildCount();

	std::size_t childCount = immediateChildCount;
	for (std::size_t i = 0; i < immediateChildCount; i++) {
		childCount += component->getChild(i).getChildCount();
	}
	return childCount;
}

std::size_t getEntityImmediateChildCount(Core::Engine* engine, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return 0;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	ChildManager* component = entityManager.getComponent<ChildManager>(entity);
	if (component == nullptr) return 0;
	return component->getChildCount();
}


bool addComponent(Core::Engine* engine, EntityID entityID, std::size_t sceneIndex, TypeID typeID) {
	if (entityID == Entity::INVALID_ID) return false;
	SceneManager& sceneManager = engine->getSceneManager();
	ScenePtr scene = sceneManager.getScene(sceneIndex);
	Entity entity = Entity(entityID);
	scene->addComponent(entity, typeID); // TODO: Check if successful
	return true;
}

bool removeComponent(Core::Engine* engine, EntityID entityID, std::size_t sceneIndex, TypeID typeID) {
	if (entityID == Entity::INVALID_ID) return false;
	SceneManager& sceneManager = engine->getSceneManager();
	ScenePtr scene = sceneManager.getScene(sceneIndex);
	Entity entity = Entity(entityID);
	scene->removeComponent(entity, typeID); // TODO: Check if successful
	return true;
}

bool renameEntity(Core::Engine* engine, EntityID entityID, const char* name) {
	if (entityID == Entity::INVALID_ID) return false;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	return entityManager.renameEntity(entity, name);
}

void getComponents(Core::Engine* engine, EntityID entityID, void** outPtrs, TypeID* outTypeIDs, std::size_t outSize) {
	if (entityID == Entity::INVALID_ID) return;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	std::vector<Component*> components = entityManager.getComponents(entity);
	std::size_t size = std::min(components.size(), outSize);
	for (std::size_t i = 0; i < size; i++) {
		outPtrs[i] = components[i];
		outTypeIDs[i] = components[i]->getTypeID();
	}
}

std::size_t getComponentsCount(Core::Engine* engine, EntityID entityID) {
	if (entityID == Entity::INVALID_ID) return 0;
	EntityManager& entityManager = engine->getEntityManager();
	Entity entity = Entity(entityID);
	std::vector<Component*> components = entityManager.getComponents(entity);
	return components.size();
}