#ifndef PARENT_ENTITY_H
#define PARENT_ENTITY_H

#include "Component.h"
#include "Handle.h"

namespace Core {

	class ParentEntity : public Component {
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