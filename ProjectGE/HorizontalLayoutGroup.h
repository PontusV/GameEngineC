#ifndef HORIZONTAL_LAYOUT_GROUP_H
#define HORIZONTAL_LAYOUT_GROUP_H
#include "LayoutController.h"
namespace Core {
	class HorizontalLayoutGroup : public LayoutController {
	public:
		HorizontalLayoutGroup();
		~HorizontalLayoutGroup();

		void update(float deltaTime) override;


		bool childForceExpandWidth;
		bool childForceExpandHeight;
	};
}
#endif