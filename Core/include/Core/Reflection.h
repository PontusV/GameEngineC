#pragma once
#include "dllexport.h"
#include "Component.h"
#include <cstddef>

#define MAX_FIELD_COUNT 10

namespace Core {
	extern "C" {
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

		DLLEXPORT void onUpdate(IComponent* component, std::size_t typeID, std::size_t propIndex);
		//DLLEXPORT void* getPointer(IComponent* component, std::size_t typeID, std::size_t propIndex);
		DLLEXPORT void getTypeName(std::size_t typeID, char* out, std::size_t size);
		DLLEXPORT std::size_t getPropertiesCount(std::size_t typeID);
		DLLEXPORT void getProperties(std::size_t typeID, void* instance, ReflectedPropertyData* out, std::size_t count);
		//DLLEXPORT std::size_t getAnnotationsCount(std::size_t typeID);
		//DLLEXPORT void getAnnotations(std::size_t typeID, const char** out, std::size_t count);
		DLLEXPORT bool hasAnnotation(std::size_t typeID, const char* annotation);
		DLLEXPORT std::size_t getDerivedTypeIDsCount(std::size_t typeID);
		DLLEXPORT void getDerivedTypeIDs(std::size_t typeID, std::size_t* out, std::size_t count);
		DLLEXPORT std::size_t getAllReflectedTypesCount();
		DLLEXPORT void getAllReflectedTypes(ReflectedTypeData* out, std::size_t count);
	}
}