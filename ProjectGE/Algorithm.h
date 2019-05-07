#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Texture2D.h"
#include "Transform.h"
#include "Interactable.h"
#include "TransformMaths.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <vector>

namespace Core {
	namespace maths {

		struct RectTransform {
			Transform transform;
			glm::vec2 size;
			glm::vec2 offset; // size * transform.anchor
		};
		struct TexturedRectTransform : public RectTransform {
			Texture2D texture;
			std::size_t id;
		};

		/* Returns true if the position is inside the rect. */
		bool hitCheck(float posX, float posY, const RectTransform& rect) {
			glm::vec2 localPosition = rect.transform.getWorldToLocalMatrix() * glm::vec2(posX, posY);
			return (
				localPosition.x > rect.offset.x &&
				localPosition.x < rect.offset.x + rect.size.x &&
				localPosition.y > rect.offset.y &&
				localPosition.y < rect.offset.y + rect.size.y
			);
		}

		/* Returns the id of the TexturedRectTransform that is infront if there are any at the position. Returns 0 if none was found. */
		std::size_t hitDetect(const float xPoint, const float yPoint, const std::vector<TexturedRectTransform>& rects, bool alpha = false) {
			std::vector<TexturedRectTransform> interactables;

			for (const TexturedRectTransform& rect : rects) {
				// Simple hit detection
				if (hitCheck(xPoint, yPoint, rect))
				{
					if (!alpha || rect.texture.getAlphaAtPoint((int)(xPoint - rect.transform.getX()), (int)(yPoint - rect.transform.getY())) > 0.0f) // Expensive
						interactables.push_back(rect);
				}
			}

			if (interactables.size() == 0) return 0;
			// Sort Interactables by transform.z to see which is infront
			std::sort(interactables.begin(), interactables.end(), [](TexturedRectTransform& l, TexturedRectTransform& r) {
				return l.transform.getZ() > r.transform.getZ();
			});
			return interactables[0].id;
		}

		bool isInsideWindow(int cameraX, int cameraY, int windowWidth, int windowHeight, RectTransform& rect) {
			int relativeX = (int)(rect.transform.getX() + rect.offset.x - cameraX); // Offset
			int relativeY = (int)(rect.transform.getY() + rect.offset.y - cameraY); // Offset
			return (relativeX + rect.size.x > 0 && relativeX < windowWidth &&
					relativeY + rect.size.y > 0 && relativeY < windowHeight);
		}
	}
}

#endif