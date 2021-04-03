#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Renderer.h"
#include "SpriteRenderer.generated.h"

namespace Core {
	CLASS() SpriteRenderer : public Renderer {
		GENERATED_BODY()
	public:
		SpriteRenderer();
		~SpriteRenderer();

		void setFlipX(bool value);
		bool getFlipX();

		void setFlipY(bool value);
		bool getFlipY();

	private:
		PROPERTY()
		bool flipX = false;
		PROPERTY()
		bool flipY = false;
	};
}
#endif