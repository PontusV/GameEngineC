#ifndef CHILD_MANAGER_H
#define CHILD_MANAGER_H

#include "entity/component/Component.h"
#include "entity/handle/Handle.h"
#include "entity/Entity.h"
#include <vector>
#include <cstddef>
#include "ChildManager.generated.h"

namespace Core {

	CLASS(hideInInspector) ChildManager : public Component {
		GENERATED_BODY()
	public:
		ChildManager() {}
		~ChildManager();

		/* Adds entity to collection if no reference to the child already exists; otherwise the reference held to the child will be replace with the parameter value. */
		bool onChildAdded(Handle entity);
		bool onChildRemoved(Handle entity);

		std::size_t getChildCount();
		Handle getChild(std::size_t index);
		std::vector<Handle>& getChildren();

	private:
		//PROPERTY()
		std::vector<Handle> children;
	};
}
#endif