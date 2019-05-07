#ifndef SPRITE_H
#define SPRITE_H

#include "BoxComponent.h"
#include "Color.h"
#include <glm/glm.hpp>

namespace Core {
	/* Enables clipping; removes graphic drawn outside the given rect. */
	class Sprite : public BoxComponent {
	public:
		virtual ~Sprite() = 0; // Abstract

		void					setLayerIndex(unsigned char index);
		const unsigned char&	getLayerIndex() const;

		const Color&			getColor() const;
		void					setColor(Color value);

		const std::vector<glm::vec2>& getClipMaskVertices() const;
		/* Cuts away Drawable area that is outside of the rect parameter. */
		void					clip(glm::vec2 clipVertices[4]);
		void					resetClipping();
		bool					isClipEnabled() const;

		// Serializable
		virtual void			serialize(std::ostream& os) const;
		virtual void			deserialize(std::istream& is);

	protected:
		Sprite(unsigned char layerIndex = 0, unsigned int width = 0, unsigned int height = 0, Color color = {255, 255, 255, 255});

	private:
		Color					color;
		unsigned char			layerIndex;
		bool					clipEnabled;
		std::vector<glm::vec2>	clipMaskVertices;
		//glm::vec4		drawRect;
	};
}
#endif