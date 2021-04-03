#include "Core.h"
#include "engine/Engine.h"
#include "components/RectTransform.h"
#include "components/Transform.h"
#include "entity/handle/Handle.h"
#include "graphics/Camera.h"
#include "maths/MatrixTransform.h"
using namespace Core;


IEngine* DLLInterface::createEngine() {
	return new Engine();
}

void DLLInterface::createTemplateEntity(IScene* scene, const char* name, float x, float y, float width, float height) {
	static_cast<Scene*>(scene)->createEntity(name,
		Image("resources/images/awesomeface.png"),
		RectTransform(x, y, width, height, 0.0f, Alignment::CENTER),
		SpriteRenderer()
	);
}

IEntityHandle* DLLInterface::createEntityHandle(IScene* scene, Entity entity) {
	return new Handle(entity, static_cast<Scene*>(scene));
}

bool DLLInterface::hasTransform(IEntityHandle* entity) {
	return static_cast<Handle*>(entity)->hasComponent<Transform>();
}

Vec2 DLLInterface::getMinRectScreenPosition(ICamera* camera, IEntityHandle* entity) {
	RectTransform* transform = static_cast<Handle*>(entity)->getComponent<RectTransform>();
	if (!transform) return Vec2{ 0, 0 };
	const Vector2& targetSize = transform->getSize();
	Vector2 position = static_cast<Camera*>(camera)->getViewMatrix() * transform->getPosition();
	Vector2 minPosition = Vector2(position.x + transform->getRectOffset().x, position.y + transform->getRectOffset().y);
	return Vec2{ minPosition.x, minPosition.y };
}

Vec2 DLLInterface::getPosition(IEntityHandle* entity) {
	Transform* transform = static_cast<Handle*>(entity)->getComponent<Transform>();
	if (!transform) return Vec2{ 0, 0 };
	const Vector2& pos = transform->getPosition();
	return Vec2{ pos.x, pos.y };
}

Vec2 DLLInterface::getLocalPosition(IEntityHandle* entity) {
	Transform* transform = static_cast<Handle*>(entity)->getComponent<Transform>();
	if (!transform) return Vec2{ 0, 0 };
	const Vector2& pos = transform->getLocalPosition();
	return Vec2{ pos.x, pos.y };
}

Vec2 DLLInterface::getSize(IEntityHandle* entity) {
	RectTransform* transform = static_cast<Handle*>(entity)->getComponent<RectTransform>();
	if (!transform) return Vec2{ 0, 0 };
	const Vector2& size = transform->getSize();
	return Vec2{size.x, size.y};
}

bool DLLInterface::setPosition(IEntityHandle* entity, Vec2 position) {
	Transform* transform = static_cast<Handle*>(entity)->getComponent<Transform>();
	if (!transform) return false;
	transform->setPosition(Vector2(position.x, position.y));
	return true;
}

bool DLLInterface::setLocalPosition(IEntityHandle* entity, Vec2 position) {
	Transform* transform = static_cast<Handle*>(entity)->getComponent<Transform>();
	if (!transform) return false;
	transform->setLocalPosition(Vector2(position.x, position.y));
	return true;
}

bool DLLInterface::setSize(IEntityHandle* entity, Vec2 size) {
	RectTransform* transform = static_cast<Handle*>(entity)->getComponent<RectTransform>();
	if (!transform) return false;
	transform->setSize(Vector2(size.x, size.y));
	return true;
}

// ------------------------------------- REFLECTION -------------------------------------

#include <ReflectionParser/ReflectionTypes.h>
#include "ReflectionPolymorph.generated.h"
#include <cstring>
#include <algorithm>


template<typename T>
bool vectorIncludes(const std::vector<T>& vec, T value) {
	for (const T& e : vec) {
		if (e == value) return true;
	}
	return false;
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

void copyString(const std::string& src, char* dest, std::size_t destSize) {
	std::size_t size = std::min(src.size(), destSize - 1);
	strncpy_s(dest, destSize, src.c_str(), size);
	dest[size] = '\0';
}

ReflectedPropertyData convertPropertyData(const Mirror::Property& prop, std::size_t index, void* instance, std::size_t typeID) {
	ReflectedPropertyData data;
	data.index = index;
	copyString(prop.name, data.name, 256);
	copyString(prop.type.name, data.typeName, 256);
	data.id = 0; // WIP

	data.renderer = getRendererFromProp(prop);

	if (data.renderer == InspectorFieldRenderType::VECTOR2) {
		Vector2* vec = static_cast<Vector2*>(Mirror::getPointer(prop, instance, typeID));
		data.fieldCount = 2;
		data.fieldBuffer[0].dataPtr = &vec->x;
		data.fieldBuffer[1].dataPtr = &vec->y;
	}
	else if (data.renderer == InspectorFieldRenderType::COLOR) {
		Color* color = static_cast<Color*>(Mirror::getPointer(prop, instance, typeID));
		data.fieldCount = 4;
		data.fieldBuffer[0].dataPtr = &color->r;
		data.fieldBuffer[1].dataPtr = &color->g;
		data.fieldBuffer[2].dataPtr = &color->b;
		data.fieldBuffer[3].dataPtr = &color->a;
	}
	else if (data.renderer == InspectorFieldRenderType::FONT) {
		Font* font = static_cast<Font*>(Mirror::getPointer(prop, instance, typeID));
		data.fieldCount = 2;
		data.fieldBuffer[0].dataPtr = &font->fileName;
		data.fieldBuffer[1].dataPtr = &font->size;
	}
	else {
		Vector2* vec = static_cast<Vector2*>(Mirror::getPointer(prop, instance, typeID));
		data.fieldCount = 1;
		data.fieldBuffer[0].dataPtr = Mirror::getPointer(prop, instance, typeID);
	}
	return data;
}

void DLLInterface::onUpdate(IComponent* component, std::size_t typeID, std::size_t propIndex) {
	Mirror::Class type = Mirror::getType(typeID);
	Mirror::onUpdate(component, typeID, type.properties[propIndex]);
}

/*void* Core::getPointer(IComponent* component, std::size_t typeID, std::size_t propIndex) {
	Mirror::Class type = Mirror::getType(typeID);
	return Mirror::getPointer(type.properties[propIndex], component, typeID);
}*/

void DLLInterface::getTypeName(std::size_t typeID, char* out, std::size_t size) {
	copyString(Mirror::getName(typeID), out, size);
}

std::size_t DLLInterface::getPropertiesCount(std::size_t typeID) {
	Mirror::Class type = Mirror::getType(typeID);
	return type.properties.size();
}

void DLLInterface::getProperties(std::size_t typeID, void* instance, ReflectedPropertyData* out, std::size_t count) {
	Mirror::Class type = Mirror::getType(typeID);
	std::size_t size = std::min(count, type.properties.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = convertPropertyData(type.properties[i], i, instance, typeID);
	}
}

bool DLLInterface::hasAnnotation(std::size_t typeID, const char* annotation) {
	Mirror::Class type = Mirror::getType(typeID);
	return type.hasAnnotation(annotation);
}

std::size_t DLLInterface::getDerivedTypeIDsCount(std::size_t typeID) {
	return Mirror::polyGetDerivedTypeIDs(typeID).size();
}

void DLLInterface::getDerivedTypeIDs(std::size_t typeID, std::size_t* out, std::size_t count) {
	std::vector<std::size_t> typeIDs = Mirror::polyGetDerivedTypeIDs(typeID);
	std::size_t size = std::min(count, typeIDs.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i] = typeIDs[i];
	}
}

std::size_t DLLInterface::getAllReflectedTypesCount() {
	return Mirror::getAllReflectedTypes().size();
}

void DLLInterface::getAllReflectedTypes(ReflectedTypeData* out, std::size_t count) {
	std::vector<Mirror::ReflectedType> types = Mirror::getAllReflectedTypes();
	std::size_t size = std::min(count, types.size());
	for (std::size_t i = 0; i < size; i++) {
		out[i].typeID = types[i].typeID;
		copyString(types[i].typeName, out[i].typeName, 256);
	}
}
// ------------------------------------- END -------------------------------------

DLLInterface* createDLLInterface() {
	return new DLLInterface();
}

void releaseDLLInterface(DLLInterface* ptr) {
	delete ptr;
}