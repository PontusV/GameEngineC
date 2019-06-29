#ifndef PANEL_H
#define PANEL_H
#include "Component.h"
#include "Panel.Panel.generated.h"

namespace Core {

	/* Clips all GraphicComponents contained by all children; panels remove graphics drawn by its children that resides outside of the panel. */
	CLASS() Panel : public Component {
		GENERATED_BODY()
	public:
		Panel();
		~Panel();
	};
}
#endif