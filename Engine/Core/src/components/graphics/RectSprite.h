#ifndef RECT_SPRITE_H
#define RECT_SPRITE_H

#include "Graphic.h"

#include "RectSprite.generated.h"

namespace Core {
	/* Untextured rect sprite */
	CLASS() RectSprite : public Graphic {
		GENERATED_BODY()
	public:
		RectSprite(std::size_t sortingOrder, Color color, Shader shader);
		RectSprite(std::size_t sortingOrder, Color color);
		RectSprite();
		~RectSprite();
	};
}

#endif