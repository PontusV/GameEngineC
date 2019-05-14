#ifndef RECTANGLE_COMPONENT_H
#define RECTANGLE_COMPONENT_H

#include "Sprite.h"
#include "Color.h"

namespace Core {
	class RectSprite : public Sprite {
	public:

		RectSprite(Color color, unsigned char layerIndex = 0);
		RectSprite();
		~RectSprite();

		// Serializable
		virtual void serialize(std::ostream& os) const;
		virtual void deserialize(std::istream& is);
	};
}
#endif