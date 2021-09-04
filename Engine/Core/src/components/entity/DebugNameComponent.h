#ifndef DEBUG_NAME_COMPONENT_H
#define DEBUG_NAME_COMPONENT_H

#include "entity/component/IComponentData.h"
#include <string>
#include "DebugNameComponent.generated.h"

namespace Core {
	/* Useful for debugging */
	CLASS(hideInInspector) DebugNameComponent : public IComponentData {
		GENERATED_BODY()
	public:
		DebugNameComponent(const char* name);
		DebugNameComponent();
		~DebugNameComponent();

		void setName(std::string name);
		std::string getName() const;
	private:
		PROPERTY()
		std::string name;
	};
}

#endif