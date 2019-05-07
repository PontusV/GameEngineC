#ifndef LAYOUT_CONTROLLER_H
#define LAYOUT_CONTROLLER_H
#include "Script.h"
namespace Core {
	class LayoutController : public Script {
	protected:
		LayoutController();
	public:
		virtual ~LayoutController() = 0;


		float paddingTop, paddingRight, paddingBottom, paddingLeft;
		float spacing;
		// Child Alignment?
	};
}
#endif