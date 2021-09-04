#ifndef WINDOW_SCALE_H
#define WINDOW_SCALE_H
#include "UIBehaviour.h"
#include "WindowScale.generated.h"
namespace Core {
	/* Scales the size of a RectTransform component that is attached to the same Entity. */
	CLASS() WindowScale : public UIBehaviour {
		GENERATED_BODY()
	public:
		WindowScale(bool stretchWidth, float widthScale, bool stretchHeight, float heightScale);
		WindowScale();
		~WindowScale();

		void initialize() override;
		void onWindowResize() override;


		PROPERTY()
		bool stretchWidth = false;
		PROPERTY()
		bool stretchHeight = false;
		PROPERTY()
		float widthScale = 1; // The width of the RectTransform will be this multiplied by the Window width
		PROPERTY()
		float heightScale = 1; // The height of the RectTransform will be this multiplied by the Window height
	};
}
#endif