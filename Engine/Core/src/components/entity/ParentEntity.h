#ifndef PARENT_ENTITY_H
#define PARENT_ENTITY_H

#include "entity/component/IComponentData.h"
#include "entity/handle/Handle.h"
#include "ParentEntity.generated.h"

namespace Core {

	/* Gives an Entity a reference to its parent */
	CLASS(hideInInspector) ParentEntity : public IComponentData{
		GENERATED_BODY()
	public:
		ParentEntity(Handle parent);
		ParentEntity();
		~ParentEntity();

		void setParent(Handle parent);
		/* Returns a refreshed handle to the parent */
		Handle getParent();

	private:
		PROPERTY()
		Handle parent;
	};
}
#endif