#pragma once
#include "dllexport.h"
#include "EntityHandle.h"
#include "IEntity.h"
#include <cwchar>
#include <cstddef>

namespace Core {
	extern "C" class DLLEXPORT IScene {
	public:
		/* Compares this scene with another */
		int compare(IScene* other) {
			return wcscmp(getName(), other->getName());
		}

		/* Creates an Entity without any components and attaches it to the scene and adds it to the EntityManager */
		virtual Entity createEmptyEntity(const char* name) = 0;
		/* Returns the number of entities attached to this scene */
		virtual std::size_t getAllEntitiesCount() = 0;
		/* Returns all entities attached to this scene */
		virtual void getAllIEntities(IEntityHandle** out, std::size_t count) = 0;
		/* Returns the name of this scene */
		virtual const wchar_t* getName() = 0;
		/* Calls awake on all Entities contained by this Scene. Future Entities and Components will be awaken once added. Does nothing if the Scene has already been awaken. */
		virtual void awake() = 0;
		virtual bool destroyEntity(Entity entity) = 0;

		virtual bool hasEntitiesChanged() = 0;
	};
}