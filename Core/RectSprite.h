#ifndef RECTANGLE_COMPONENT_H
#define RECTANGLE_COMPONENT_H

#include "Sprite.h"
#include "Color.h"
#include "RectSprite.RectSprite.generated.h"

namespace Core {
	CLASS() RectSprite : public Sprite {
		GENERATED_BODY()
	public:
		RectSprite(Color color, unsigned char layerIndex = 0);
		RectSprite();
		~RectSprite();
	};
}
#endif