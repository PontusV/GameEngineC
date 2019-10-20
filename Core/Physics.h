#ifndef PHYSICS_H
#define PHYSICS_H

#include "ComponentGroup.h"
#include "ComponentArray.h"
#include "Transform.h"
#include "ParentEntity.h"
#include "ComponentTypeInfo.h"

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