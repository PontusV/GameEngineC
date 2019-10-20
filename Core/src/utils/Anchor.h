#ifndef ANCHOR_H
#define ANCHOR_H
#include "Alignment.h"
#include "maths/Vector2.h"
#include <iostream>
namespace Core {

	/* Converts Alignment to Vector2 */
	class Anchor : public Vector2 {
	public:
		Anchor(const Alignment& alignment)	: Vector2((float)(alignment % 3) / 2, (float)((int)alignment / 3) / 2) {}
		Anchor(const Vector2& vec)		: Vector2(vec) {}
		Anchor& operator=(const Alignment& alignment) {
			x = (float)(alignment % 3) / 2;
			y = (float)((int)alignment / 3) / 2;
			return *this;
		}
		Anchor& operator=(const Vector2& vec) {
			x = vec.x;
			y = vec.y;
			return *this;
		}
	};
}
#endif