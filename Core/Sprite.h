#ifndef SPRITE_H
#define SPRITE_H

#include "BoxComponent.h"
#include "Color.h"
#include <glm/glm.hpp>
#include "Sprite.Sprite.generated.h"

namespace Core {
	/* 2D Graphic. */
	CLASS() Sprite : public Component {
		GENERATED_BODY()
	public:
		virtual ~Sprite() = 0; // Abstract

		const Color& getColor() const;
		void setColor(Color value);

		const std::vector<glm::vec2>& getClipMaskVertices() const;
		/* Cuts away Drawable area that is outside of the rect parameter. */
		void clip(glm::vec2 clipVertices[4]);
		void resetClipping();
		bool isClipEnabled() const;

	protected:
		Sprite(Color color = {255, 255, 255, 255});

	private:
		PROPERTY()
		Color color;

		bool clipEnabled = false;
		std::vector<glm::vec2> clipMaskVertices;
	};
}
#endif