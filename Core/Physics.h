#ifndef PHYSICS_H
#define PHYSICS_H

#include "ComponentGroup.h"
#include "ComponentArray.h"
#include "Transform.h"
#include "ParentEntity.h"
#include "ComponentTypeInfo.h"

namespace Core {

	struct RootTransforms : public ComponentGroup<Transform> {
		RootTransforms() : ComponentGroup<Transform>({typeof(ParentEntity)}) {}
		ComponentArray<Transform>&	transforms = getComponentArray<Transform>();
	};

	class Physics
	{
	public:
		Physics();
		~Physics();

		void update(float dt);

	private:
		RootTransforms rootTransforms;
	};
}
#endif