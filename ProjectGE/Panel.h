#ifndef PANEL_H
#define PANEL_H
#include "BoxComponent.h"
#include "GraphicComponent.h"
//#include "ChildCollection.h"

namespace Core {

	/* Clips all GraphicComponents contained by all children; panels remove graphics drawn by its children that resides outside of the panel. */
	class Panel : public BoxComponent {
		REGISTER_COMPONENT_TYPE_WITH_BASE(8, BoxComponent);
	public:

		Panel(unsigned int width, unsigned int height);
		Panel();
		~Panel();

		void init();
		void end();

		/* Returns all GraphicComponents from child Entities. */
		//std::vector<GraphicComponent*> getChildGraphics();
		
		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);

	private:
		//ChildCollection<GraphicComponent> childGraphics;
	};
}
#endif