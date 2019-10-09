#ifndef PARENT_ENTITY_H
#define PARENT_ENTITY_H

#include "Component.h"
#include "Handle.h"
#include "ParentEntity.generated.h"

namespace Core {

	CLASS(hideInInspector) ParentEntity : public Component {
		GENERATED_BODY()
	public:
		ParentEntity(Handle parent);
		ParentEntity();
		~ParentEntity();

		void setParent(Handle parent);
		Handle getParent();

	private:
		PROPERTY()
		Handle parent;
	};
}
#endif