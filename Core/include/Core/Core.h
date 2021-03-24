#pragma once
#include "dllexport.h"
#include "Engine.h"
#include "EntityHandle.h"

namespace Core {
	extern "C" {
		struct Vec2 {
			float x = 0;
			float y = 0;
		};
		/* Creates a new Engine allocated in heap memory. Note: has to call release to free memory */
		DLLEXPORT IEngine* createEngine();
		/* Creates an Entity with basic Image and size at the given position */
		DLLEXPORT void createTemplateEntity(IScene* scene, const char* name, float x, float y, float width=350, float height=350);
		/* Creates a new Handle allocated in heap memory. Note: has to call release to free memory */
		DLLEXPORT IEntityHandle* createEntityHandle(IScene* scene, Entity entity);
		/* Returns true if the Entity has Transform component attached */
		DLLEXPORT bool hasTransform(IEntityHandle* entity);
		/* Returns the min position of rect of the Entity, in screen space */
		DLLEXPORT Vec2 getMinRectScreenPosition(ICamera* camera, IEntityHandle* entity);
		/* Returns the position of the Entity, in world space */
		DLLEXPORT Vec2 getPosition(IEntityHandle* entity);
		/* Returns the position of the Entity, in local space */
		DLLEXPORT Vec2 getLocalPosition(IEntityHandle* entity);
		/* Returns the size of the Entity */
		DLLEXPORT Vec2 getSize(IEntityHandle* entity);
		/* Sets the position of the Entity, in world space. Returns true if successful */
		DLLEXPORT bool setPosition(IEntityHandle* entity, Vec2 position);
		/* Sets the position of the Entity, in local space. Returns true if successful */
		DLLEXPORT bool setLocalPosition(IEntityHandle* entity, Vec2 position);
		/* Sets the size of the Entity. Returns true if successful */
		DLLEXPORT bool setSize(IEntityHandle* entity, Vec2 size);
	}
}