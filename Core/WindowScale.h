#ifndef WINDOW_SCALE_H
#define WINDOW_SCALE_H
#include "UIBehaviour.h"
namespace Core {
	/* Scales the size of a RectTransform component that is attached to the same Entity. */
	class WindowScale : public UIBehaviour {
	public:
		WindowScale(bool stretchWidth, float widthScale, bool stretchHeight, float heightScale);
		WindowScale();
		~WindowScale();

		void awake() override;
		void onWindowResize() override;


		bool stretchWidth = false;
		bool stretchHeight = false;
		float widthScale = 1; // The width of the RectTransform will be this multiplied by the Window width
		float heightScale = 1; // The height of the RectTransform will be this multiplied by the Window height
	};
}
#endif