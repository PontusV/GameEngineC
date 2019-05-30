#ifndef GRID_LAYOUT_GROUP_H
#define GRID_LAYOUT_GROUP_H
#include "LayoutController.h"
#include "Alignment.h"
#include <vector>
namespace Core {

	class GridLayoutGroup : public LayoutController {
		GridLayoutGroup();
		~GridLayoutGroup();


		void updateLayout() override;

		float cellSize;
		//Alignment startCorner
		//Alignment startAxis
		//unsigned char constraint; // Fixed Row count, Fixed Column Count, Flexible
	};
}
#endif