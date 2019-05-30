#ifndef CONTENT_SIZE_FITTER
#define CONTENT_SIZE_FITTER
#include "LayoutController.h"
namespace Core {
	/* Controls the size of the RectTransform that attached to the same Entity. Resizes the RectTransform for the content to fit. */
	class ContentSizeFitter : public LayoutController {
	public:
		enum Mode {
			UNCONSTRAINED = 0,
			MINSIZE,
			PREFERREDSIZE
		};
		ContentSizeFitter();
		~ContentSizeFitter();


		Mode horizontalFit = Mode::UNCONSTRAINED;
		Mode verticalFit = Mode::UNCONSTRAINED;

	private:
		void updateLayout();
	};
}
#endif