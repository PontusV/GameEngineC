#ifndef BORDER_COMPONENT_H
#define BORDER_COMPONENT_H

#include "Sprite.h"
#include "Color.h"
#include <cstddef>

namespace Core {
	class Border : public Sprite {
	public:

		Border(std::size_t thickness, Color color, bool inner = false, unsigned int layerIndex = 0, bool top = true, bool right = true, bool bottom = true, bool left = true);
		Border();
		~Border();

		bool sideEnabled(std::size_t side) const;
		bool isInner() const;
		void setInner(bool value);
		const std::size_t& getBorderThickness() const;

		// Serializable
		virtual void serialize(std::ostream& os) const;
		virtual void deserialize(std::istream& is);
	private:
		bool top, right, bottom, left;
		bool inner;
		std::size_t borderThickness;
		Color color;
		// padding!
	};
}
#endif