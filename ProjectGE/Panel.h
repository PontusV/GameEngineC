#ifndef PANEL_H
#define PANEL_H
#include "BoxComponent.h"
//#include "ChildCollection.h"

namespace Core {

	/* Clips all GraphicComponents contained by all children; panels remove graphics drawn by its children that resides outside of the panel. */
	class Panel : public BoxComponent {
	public:

		Panel(unsigned int width, unsigned int height);
		Panel();
		~Panel();
		
		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);
	};
}
#endif