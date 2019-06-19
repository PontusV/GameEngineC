#ifndef CONTENT_SIZE_FITTER
#define CONTENT_SIZE_FITTER
#include "LayoutController.h"
#include "ContentSizeFitter.ContentSizeFitter.generated.h"
namespace Core {
	/* Controls the size of the RectTransform that attached to the same Entity. Resizes the RectTransform for the content to fit. */
	CLASS() ContentSizeFitter : public LayoutController {
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