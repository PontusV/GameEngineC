#pragma once
#include "dllexport.h"

namespace Core {
	class DLLEXPORT ICamera {
	public:
		virtual float getX() = 0;
		virtual float getY() = 0;
		virtual void setPosition(float x, float y) = 0;
		virtual void moveX(float value) = 0;
		virtual void moveY(float value) = 0;
		virtual void setRotation(float value) = 0;
		virtual void rotate(float value) = 0;
	};
}