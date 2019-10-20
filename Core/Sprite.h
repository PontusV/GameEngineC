#ifndef SPRITE_H
#define SPRITE_H

#include "RectTransform.h"
#include "Color.h"
#include "Maths/Vector2.h"
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
		void setSortingLayer(std::size_t sortingLayer);
		const std::size_t& getSortingLayer() const;

		const std::vector<std::array<Vector2, 4>>& getMasks() const;
		/* Cuts away Drawable area that is outside of the rect parameter. */
		void clip(ComponentID sender, const std::array<Vector2, 4> & mask);
		void reclip(ComponentID sender, const std::array<Vector2, 4> & mask);
		void removeClip(ComponentID sender);
		void resetClipping();
		bool isClipEnabled() const;

		bool hasMaskFromSender(ComponentID sender);

	protected:
		Sprite(Color color = {255, 255, 255, 255}, std::size_t sortingLayer = 0);

	private:
		std::size_t getMaskSenderIndex(ComponentID sender);

	private:
		PROPERTY()
		Color color;
		PROPERTY()
		std::size_t sortingLayer = 0;

		std::vector<std::array<Vector2, 4>> masks;
		std::vector<ComponentID> maskSenders;
	};
}
#endif