#ifndef PHYSICS_H
#define PHYSICS_H

#include "entity/component/ComponentGroup.h"
#include "entity/component/ComponentArray.h"
#include "entity/component/ComponentTypeInfo.h"
#include "components/Transform.h"
#include "components/entity/ParentEntity.h"

namespace Core {

	class Physics
	{
	public:
		Physics();
		~Physics();

		void update(float dt);

	private:
		ComponentGroup<Transform> rootTransforms = ComponentGroup<Transform>({typeof(ParentEntity)});
	};
}
#endif