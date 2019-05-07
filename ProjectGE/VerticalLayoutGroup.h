#ifndef VERTICAL_LAYOUT_GROUP_H
#define VERTICAL_LAYOUT_GROUP_H
#include "LayoutController.h"
namespace Core {
	class VerticalLayoutGroup : public LayoutController {
	public:
		VerticalLayoutGroup();
		~VerticalLayoutGroup();

		void update(float deltaTime);

		bool childForceExpandWidth;
		bool childForceExpandHeight;
	};
}
#endif