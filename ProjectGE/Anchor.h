#ifndef ANCHOR_H
#define ANCHOR_H
#include "Alignment.h"
#include <glm/glm.hpp>
#include <iostream>
namespace Core {

	/* Converts Alignment to glm::vec2 */
	class Anchor : public glm::vec2 {
	public:
		Anchor(const Alignment& alignment) : glm::vec2((float)(alignment % 3) / 2, (float)((int)alignment / 3) / 2) {}
		Anchor& operator=(const Alignment& alignment) {
			x = (float)(alignment % 3) / 2;
			y = (float)((int)alignment / 3) / 2;
			return *this;
		}
	};
}
#endif