#ifndef CONTENT_SIZE_FITTER
#define CONTENT_SIZE_FITTER
#include "LayoutController.h"
#include "ContentSizeFitter.generated.h"
namespace Core {
	/* Controls the size of the RectTransform that is attached to the same Entity. Resizes the RectTransform for the content to fit. */
	CLASS(Abstract) ContentSizeFitter : public LayoutController {
		GENERATED_BODY()
	public:
		enum Mode {
			UNCONSTRAINED = 0,
			MINSIZE,
			PREFERREDSIZE
		};
		ContentSizeFitter();
		~ContentSizeFitter();


		//PROPERTY() // ENUM
		Mode horizontalFit = Mode::UNCONSTRAINED;
		//PROPERTY() // ENUM
		Mode verticalFit = Mode::UNCONSTRAINED;

	private:
		void updateLayout();
	};
}
#endif