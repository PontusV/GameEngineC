#ifndef RECT_MASK_H
#define RECT_MASK_H
#include "components/Behaviour.h"
#include "entity/handle/ComponentHandle.h"
#include <cstddef>
#include <map>
#include <array>
#include "RectMask.generated.h"

namespace Core {

	/* Clips all GraphicComponents contained by all children; panels remove graphics drawn by its children that resides outside of the panel. */
	/* A Mask without a texture. */
	CLASS() RectMask : public Behaviour {
		GENERATED_BODY()
	public:
		RectMask();
		~RectMask();

		void start() override;
		void onTransformChanged() override;
		void onChildAdded(EntityHandle entity) override;
		void onChildRemoved(EntityHandle entity) override;
		void onChildChanged(EntityHandle entity) override;

	private:
		void updateMask();
		std::vector<ComponentHandle> childSprites;
	};
}
#endif