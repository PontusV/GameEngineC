#ifndef CHILD_MANAGER_H
#define CHILD_MANAGER_H

#include "Component.h"
#include "Entity.h"
#include <vector>
#include <cstddef>

namespace Core {

	class ChildManager : public Component {
	public:
		ChildManager() {}
		~ChildManager();

		/* Adds entity to collection if no reference to the child already exists; otherwise the reference held to the child will be replace with the parameter value. */
		void childAdded(Handle entity);
		void childRemoved(Entity entity);

		std::size_t getChildCount();
		Handle getChild(std::size_t index);
		std::vector<Handle>& getChildren();

	private:
		std::vector<Handle> children;
	};
}
#endif