#ifndef DROP_DOWN_SCROLL_H
#define DROP_DOWN_SCROLL_H
#include "DropDown.h"
#include "DropDownScroll.generated.h"
namespace Core {
	CLASS() DropDownScroll : public DropDown {
		GENERATED_BODY()
	public:
		DropDownScroll();
		DropDownScroll(Text text);
		~DropDownScroll();
		void open() override;

	public:
		int boxHeight;
	};
}
#endif