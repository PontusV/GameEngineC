#ifndef RECTANGLE_COMPONENT_H
#define RECTANGLE_COMPONENT_H

#include "GraphicComponent.h"
#include <glm/glm.hpp>

namespace Core {
	class Rect : public GraphicComponent {
		REGISTER_COMPONENT_TYPE_WITH_BASE(5, GraphicComponent);
	public:

		Rect(int width, int height, glm::vec4 color, unsigned short layerIndex = 0);
		Rect();
		~Rect();

		const glm::vec4& getColor() const;

		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);

	private:
		glm::vec4 color;
	};
}
#endif