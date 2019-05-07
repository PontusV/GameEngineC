#ifndef GRID_LAYOUT_GROUP_H
#define GRID_LAYOUT_GROUP_H
#include "LayoutController.h"
#include "Alignment.h"
#include <vector>
namespace Core {
	/* Has a set amount of colums and an indefinate amount of rows. Each column may be assigned different widths. All cells have the same height. */
	class VerticalGridLayoutGroup : public LayoutController {
	public:
		VerticalGridLayoutGroup();
		~VerticalGridLayoutGroup();


		float cellHeights;
		std::vector<float> cellWidths;
		//Alignment startAxis; Acceptable values: Up, Right, Left, Down
		//Alignment startCorner;
	};
	
	/* Has a set amount of rows and an indefinate amount of colums. Each row may be assigned different heights. All cells have the same width. */
	class HorizontalGridLayoutGroup : public LayoutController {
	public:
		HorizontalGridLayoutGroup() {}
		~HorizontalGridLayoutGroup() {}


		std::vector<float> cellHeights;
		float cellWidth;
		//Alignment startAxis; Acceptable values: Up, Right, Left, Down
		//Alignment startCorner;
	};
}
#endif