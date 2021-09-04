#ifndef COMPONENT_H
#define COMPONENT_H
#include <ReflectionParser/ReflectionMacros.h>
#include "entity/Entity.h"
#include "IComponentData.generated.h"

typedef std::size_t ComponentTypeID;

namespace Core {
	CLASS() IComponentData {
		GENERATED_BODY()
	protected:
		IComponentData();
	public:
		IComponentData(const IComponentData& other);
		IComponentData& operator=(const IComponentData& other);
		virtual ~IComponentData() = 0; // Abstract
	};
}

#endif