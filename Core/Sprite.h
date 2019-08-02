#ifndef SPRITE_H
#define SPRITE_H

#include "BoxComponent.h"
#include "RectTransform.h"
#include "Color.h"
#include <glm/glm.hpp>
#include <cstddef>
#include <vector>
#include <array>
#include "Sprite.generated.h"

namespace Core {
	/* 2D Graphic. */
	CLASS() Sprite : public Component {
		GENERATED_BODY()
	public:
		virtual ~Sprite() = 0; // Abstract

		const Color& getColor() const;
		void setColor(Color value);

		const std::vector<std::array<glm::vec2, 4>>& getMasks() const;
		/* Cuts away Drawable area that is outside of the rect parameter. */
		std::size_t clip(const std::array<glm::vec2, 4> & mask);
		void reclip(std::size_t index, const std::array<glm::vec2, 4> & mask);
		void removeClip(std::size_t index);
		void resetClipping();
		bool isClipEnabled() const;

	protected:
		Sprite(Color color = {255, 255, 255, 255});

	private:
		PROPERTY()
		Color color;
		std::vector<std::array<glm::vec2, 4>> masks;
	};
}
#endif