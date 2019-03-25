#ifndef BOX_COMPONENT_H
#define BOX_COMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>

namespace Core {
	/* A type of component with a size. Calculates offset from Transform component, depending on Transform.anchor
	 * Abstract class. Requires a Transform component in the same Entity to function. */
	class BoxComponent : public Component
	{
	public:
		virtual ~BoxComponent();
		// Get
		const glm::vec2&	getOffset();
		const glm::ivec2&	getSize() const;
		// Set
		void				setSize(unsigned int width, unsigned int height);
		void				setSize(glm::ivec2 size);

		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);

	protected:
		BoxComponent(unsigned int width, unsigned int height);
		BoxComponent(glm::ivec2 size);
		BoxComponent();

	private:
		glm::ivec2	size;
		glm::vec2	offset; // Offset from Transform.position. Shows where TOP_LEFT vertex is located.
		bool		initPosition;
	};
}
#endif