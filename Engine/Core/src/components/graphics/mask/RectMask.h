#ifndef RECT_MASK_H
#define RECT_MASK_H
#include "Mask.h"
#include <cstddef>
#include "RectMask.generated.h"

namespace Core {

	/* A Mask without a texture. */
	CLASS() RectMask : public Mask {
		GENERATED_BODY()
	public:
		RectMask();
		~RectMask();

		void setPadding(std::size_t value);
		std::size_t getPadding();

	private:
		PROPERTY()
		std::size_t padding;
	};
}
#endif