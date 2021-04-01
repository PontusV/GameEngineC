#pragma once
#include "dllexport.h"
#include "Engine.h"
#include "EntityHandle.h"
#include "Component.h"
#include <cstddef>

#define MAX_FIELD_COUNT 10

extern "C" {
	namespace Core {
		struct Vec2 {
			float x = 0;
			float y = 0;
		};
		enum DLLEXPORT InspectorFieldRenderType {
			NONE = 0,
			BOOL,
			DECIMAL,
			SIGNED_CHAR,
			UNSIGNED_CHAR,
			SIGNED_NUMBER,
			UNSIGNED_NUMBER,
			STRING,
			WIDE_STRING,
			IMAGE_PATH,
			SHADER_PATH,
			COLOR,
			VECTOR2
		};
		struct DLLEXPORT ReflectedTypeData {
			std::size_t typeID;
			char typeName[256];
		};
		struct DLLEXPORT ReflectedFieldData {
			void* dataPtr;
		};
		struct DLLEXPORT ReflectedPropertyData {
			std::size_t index;
			std::size_t id;
			char name[256];
			char typeName[256];
			InspectorFieldRenderType renderer;
			std::size_t fieldCount;
			ReflectedFieldData fieldBuffer[MAX_FIELD_COUNT];
		};
		struct DLLEXPORT DLLInterface {
			/* Creates a new Engine allocated in heap memory. Note: has to call release to free memory */
			IEngine* createEngine();
			/* Creates an Entity with basic Image and size at the given position */
			void createTemplateEntity(IScene* scene, const char* name, float x, float y, float width = 350, float height = 350);
			/* Creates a new Handle allocated in heap memory. Note: has to call release to free memory */
			IEntityHandle* createEntityHandle(IScene* scene, Entity entity);
			/* Returns true if the Entity has Transform component attached */
			bool hasTransform(IEntityHandle* entity);
			/* Returns the min position of rect of the Entity, in screen space */
			Vec2 getMinRectScreenPosition(ICamera* camera, IEntityHandle* entity);
			/* Returns the position of the Entity, in world space */
			Vec2 getPosition(IEntityHandle* entity);
			/* Returns the position of the Entity, in local space */
			Vec2 getLocalPosition(IEntityHandle* entity);
			/* Returns the size of the Entity */
			Vec2 getSize(IEntityHandle* entity);
			/* Sets the position of the Entity, in world space. Returns true if successful */
			bool setPosition(IEntityHandle* entity, Vec2 position);
			/* Sets the position of the Entity, in local space. Returns true if successful */
			bool setLocalPosition(IEntityHandle* entity, Vec2 position);
			/* Sets the size of the Entity. Returns true if successful */
			bool setSize(IEntityHandle* entity, Vec2 size);

			// ------------------------------------- REFLECTION -------------------------------------

			void onUpdate(IComponent* component, std::size_t typeID, std::size_t propIndex);
			void getTypeName(std::size_t typeID, char* out, std::size_t size);
			std::size_t getPropertiesCount(std::size_t typeID);
			void getProperties(std::size_t typeID, void* instance, ReflectedPropertyData* out, std::size_t count);
			bool hasAnnotation(std::size_t typeID, const char* annotation);
			std::size_t getDerivedTypeIDsCount(std::size_t typeID);
			void getDerivedTypeIDs(std::size_t typeID, std::size_t* out, std::size_t count);
			std::size_t getAllReflectedTypesCount();
			void getAllReflectedTypes(ReflectedTypeData* out, std::size_t count);
		};
	}

	DLLEXPORT Core::DLLInterface* createDLLInterface();
	DLLEXPORT void releaseDLLInterface(Core::DLLInterface* ptr);
}