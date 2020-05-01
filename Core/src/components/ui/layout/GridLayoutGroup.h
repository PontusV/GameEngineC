#ifndef GRID_LAYOUT_GROUP_H
#define GRID_LAYOUT_GROUP_H
#include "LayoutController.h"
#include "utils/Alignment.h"
#include <vector>
#include "GridLayoutGroup.generated.h"

namespace Core {

	CLASS() GridLayoutGroup : public LayoutController {
		GENERATED_BODY()
	public:
		GridLayoutGroup();
		~GridLayoutGroup();


		void updateLayout() override;

		PROPERTY()
		float cellSize;
		//Alignment startCorner
		//Alignment startAxis
		//unsigned char constraint; // Fixed Row count, Fixed Column Count, Flexible
	};
}
#endif