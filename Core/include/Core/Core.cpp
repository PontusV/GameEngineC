#include "Core.h"
#include "engine/Engine.h"
#include "components/RectTransform.h"
#include "components/Transform.h"
#include "entity/handle/Handle.h"
#include "graphics/Camera.h"
#include "maths/MatrixTransform.h"
using namespace Core;


IEngine* Core::createEngine() {
	return new Engine();
}

void Core::createTemplateEntity(IScene* scene, const char* name, float x, float y, float width, float height) {
	static_cast<Scene*>(scene)->createEntity(name,
		Image("resources/images/awesomeface.png"),
		RectTransform(x, y, width, height, 0.0f, Alignment::CENTER),
		SpriteRenderer()
	);
}

IEntityHandle* Core::createEntityHandle(IScene* scene, Entity entity) {
	return new Handle(entity, static_cast<Scene*>(scene));
}

bool Core::hasTransform(IEntityHandle* entity) {
	return static_cast<Handle*>(entity)->hasComponent<Transform>();
}

Vec2 Core::getMinRectScreenPosition(ICamera* camera, IEntityHandle* entity) {
	RectTransform* transform = static_cast<Handle*>(entity)->getComponent<RectTransform>();
	if (!transform) return Vec2{ 0, 0 };
	const Vector2& targetSize = transform->getSize();
	Vector2 position = static_cast<Camera*>(camera)->getViewMatrix() * transform->getPosition();
	Vector2 minPosition = Vector2(position.x + transform->getRectOffset().x, position.y + transform->getRectOffset().y);
	return Vec2{ minPosition.x, minPosition.y };
}

Vec2 Core::getPosition(IEntityHandle* entity) {
	Transform* transform = static_cast<Handle*>(entity)->getComponent<Transform>();
	if (!transform) return Vec2{ 0, 0 };
	const Vector2& pos = transform->getPosition();
	return Vec2{ pos.x, pos.y };
}

Vec2 Core::getLocalPosition(IEntityHandle* entity) {
	Transform* transform = static_cast<Handle*>(entity)->getComponent<Transform>();
	if (!transform) return Vec2{ 0, 0 };
	const Vector2& pos = transform->getLocalPosition();
	return Vec2{ pos.x, pos.y };
}

Vec2 Core::getSize(IEntityHandle* entity) {
	RectTransform* transform = static_cast<Handle*>(entity)->getComponent<RectTransform>();
	if (!transform) return Vec2{ 0, 0 };
	const Vector2& size = transform->getSize();
	return Vec2{size.x, size.y};
}

bool Core::setPosition(IEntityHandle* entity, Vec2 position) {
	Transform* transform = static_cast<Handle*>(entity)->getComponent<Transform>();
	if (!transform) return false;
	transform->setPosition(Vector2(position.x, position.y));
	return true;
}

bool Core::setLocalPosition(IEntityHandle* entity, Vec2 position) {
	Transform* transform = static_cast<Handle*>(entity)->getComponent<Transform>();
	if (!transform) return false;
	transform->setLocalPosition(Vector2(position.x, position.y));
	return true;
}

bool Core::setSize(IEntityHandle* entity, Vec2 size) {
	RectTransform* transform = static_cast<Handle*>(entity)->getComponent<RectTransform>();
	if (!transform) return false;
	transform->setSize(Vector2(size.x, size.y));
	return true;
}