#ifndef PARENT_ENTITY_H
#define PARENT_ENTITY_H

#include "Component.h"
#include "Handle.h"

namespace Core {

	class ParentEntity : public Component {
		REGISTER_COMPONENT_TYPE(6);
	public:

		ParentEntity(Handle parent);
		ParentEntity();
		~ParentEntity();

		void	setParent(Handle parent);
		Handle*	getParent();

	private:
		Handle parent;
	};
}
#endif