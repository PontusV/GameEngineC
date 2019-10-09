#ifndef CHILD_MANAGER_H
#define CHILD_MANAGER_H

#include "Component.h"
#include "Entity.h"
#include "Handle.h"
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
		void onChildAdded(Handle entity);
		void onChildRemoved(Handle entity);

		std::size_t getChildCount();
		Handle getChild(std::size_t index);
		std::vector<Handle>& getChildren();

	private:
		//PROPERTY()
		std::vector<Handle> children;
	};
}
#endif