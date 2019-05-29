#include "WindowScale.h"
#include "RectTransform.h"
#include "Window.h"

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
		glm::vec2 resolution = window->getResolution();
			transform->setWidth(resolution.x * widthScale);
		if (stretchHeight)
			transform->setHeight(resolution.y * heightScale);
	}
}