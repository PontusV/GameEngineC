#ifndef DISABLED_COMPONENT_H
#define DISABLED_COMPONENT_H

#include "entity/component/IComponentData.h"
#include "DisabledComponent.generated.h"

namespace Core {
	/* Marks the owner Entity as disabled. The Entity will, by default, not be processed by systems */
	CLASS(hideInInspector) DisabledComponent : public IComponentData {
		GENERATED_BODY()
	public:
		DisabledComponent() {}
		~DisabledComponent() {}
	};
}

#endif