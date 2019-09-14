#ifndef DROP_DOWN_SCROLL
#define DROP_DOWN_SCROLL
#include "DropDown.h"
#include "DropDown.generated.h"
namespace Core {
	class DropDownScroll : public DropDown {
		GENERATED_BODY()
	public:
		void open() override;

		int boxHeight;
	};
}
#endif