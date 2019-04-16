#ifndef BORDER_COMPONENT_H
#define BORDER_COMPONENT_H

#include "GraphicComponent.h"
#include <cstddef>
#include <glm/glm.hpp>

namespace Core {
	class Border : public GraphicComponent {
	public:

		Border(unsigned int width, unsigned int height, std::size_t thickness, glm::vec4 color, bool inner = false, unsigned int layerIndex = 0, bool top = true, bool right = true, bool bottom = true, bool left = true);
		Border();
		~Border();

		bool				sideEnabled(std::size_t side) const;
		bool				isInner() const;
		void				setInner(bool value);
		const std::size_t&	getBorderThickness() const;
		const glm::vec4&	getColor() const;

		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);
	private:
		bool top, right, bottom, left;
		bool inner;
		std::size_t borderThickness;
		glm::vec4 color;
	};
}
#endif