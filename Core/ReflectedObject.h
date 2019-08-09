#ifndef REFLECTED_OBJECT_H
#define REFLECTED_OBJECT_H
#include "ReflectionMacros.h"
#include "ReflectedObject.generated.h"
namespace Core {
	/* Interface for all reflected types. */
	CLASS() ReflectedObject {
		GENERATED_BODY()
	public:
		virtual ~ReflectedObject() = 0;
	protected:
		ReflectedObject() {}
	};
}
#endif