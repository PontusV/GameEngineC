#ifndef SPRITE_MASK_H
#define SPRITE_MASK_H
#include "Mask.h"
#include "graphics/data/Texture2D.h"
#include "SpriteMask.generated.h"

namespace Core {

	/* A Mask with a texture. */
	CLASS() SpriteMask : public Mask {
		GENERATED_BODY()
	public:
		SpriteMask();
		~SpriteMask();

		void setTexture(Texture2D value);
		const Texture2D& getTexture() const;

	private:
		PROPERTY()
		Texture2D texture;
	};
}
#endif