#ifndef RECTANGLE_COMPONENT_H
#define RECTANGLE_COMPONENT_H

#include "Sprite.h"
#include "Color.h"

namespace Core {
	class Rect : public Sprite {
	public:

		Rect(int width, int height, Color color, unsigned char layerIndex = 0);
		Rect();
		~Rect();

		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);
	};
}
#endif