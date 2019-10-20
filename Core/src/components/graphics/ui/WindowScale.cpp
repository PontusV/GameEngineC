#include "WindowScale.h"
#include "components/RectTransform.h"
#include "graphics/Window.h"
#include "maths/Vector2.h"

using namespace Core;

WindowScale::WindowScale(bool stretchWidth, float widthScale, bool stretchHeight, float heightScale)
	: stretchWidth(stretchWidth), widthScale(widthScale), stretchHeight(stretchHeight), heightScale(heightScale) {
}

WindowScale::WindowScale() {
}


WindowScale::~WindowScale() {
}


void WindowScale::awake() {
	onWindowResize();
}

void WindowScale::onWindowResize() {
	RectTransform* transform = owner.getComponent<RectTransform>();
	if (transform) {
		Vector2 resolution = window->getResolution();
		if (stretchWidth)
			transform->setWidth(resolution.x * widthScale);
		if (stretchHeight)
			transform->setHeight(resolution.y * heightScale);
	}
}