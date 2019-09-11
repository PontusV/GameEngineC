#ifndef RECT_MASK_H
#define RECT_MASK_H
#include "Behaviour.h"
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

		void onTransformChanged() override;
		void onChildAdded(Handle entity) override;
		void onChildRemoved(Handle entity) override;
		void onChildChanged(Handle entity) override;

	private:
		void updateMask();
	};
}
#endif