#ifndef CHILD_MANAGER_H
#define CHILD_MANAGER_H

#include "entity/component/IComponentData.h"
#include "entity/handle/Handle.h"
#include "entity/Entity.h"
#include <vector>
#include <cstddef>
#include "ChildManager.generated.h"

namespace Core {

	/* Gives an Entity a reference to its children */
	CLASS(hideInInspector) ChildManager : public IComponentData {
		GENERATED_BODY()
	public:
		ChildManager() {}
		~ChildManager();

		/* Adds entity to collection if no reference to the child already exists; otherwise the reference held to the child will be replaced with the parameter value. TODO: Create an unsafe and faster option (no checks) */
		bool onChildAdded(Handle entity);
		bool onChildRemoved(Handle entity);

		std::size_t getChildCount();
		Handle getChild(std::size_t index);
		std::vector<Handle>& getChildren();

	private:
		PROPERTY()
		std::vector<Handle> children;
	};
}
#endif