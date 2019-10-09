#ifndef COMPONENT_ENTRY_H
#define COMPONENT_ENTRY_H
#include "Component.h"
#include "EntityHandle.h"
#include "ComponentEntry.generated.h"
namespace Core {
	CLASS() ComponentEntry : public Component {
		GENERATED_BODY()
	public:
		ComponentEntry(EntityHandle owner, ComponentTypeID componentTypeID);
		~ComponentEntry();

		void removeComponent();

		ComponentTypeID componentTypeID;
		EntityHandle owner;
	};
}
#endif