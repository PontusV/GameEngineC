#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Transform.h"
#include "Interactable.h"
#include <glm/glm.hpp>
#include <vector>

namespace Core {
	namespace maths {

		struct RectTransform {
			Transform transform;
			glm::vec2 size;
			glm::vec2 offset;
		};
		struct TexturedRectTransform {
			Texture2D texture;
			RectTransform rectTransform;
			int id;
		};

		bool hitCheck(float xPoint, float yPoint, const RectTransform& rect) {
			return (
				xPoint > rect.transform.getX() + rect.offset.x &&
				xPoint < rect.transform.getX() + rect.offset.x + rect.size.x &&
				yPoint > rect.transform.getY() + rect.offset.y &&
				yPoint < rect.transform.getY() + rect.offset.y + rect.size.y);
		}

		/* Returns the id of the TexturedRectTransform that is infront if there are any at the position. Returns -1 if none was found. */
		int hitDetect(const float xPoint, const float yPoint, const std::vector<TexturedRectTransform>& rects, bool alpha = false) {
			std::vector<TexturedRectTransform> interactables;

			for (const TexturedRectTransform& rect : rects) {
				// Simple hit detection
				if (hitCheck(xPoint, yPoint, rect.rectTransform))
				{
					if (!alpha || rect.texture.getAlphaAtPoint((int)(xPoint - rect.rectTransform.transform.getX()), (int)(yPoint - rect.rectTransform.transform.getY())) > 0.0f) // Expensive
						interactables.push_back(rect);
				}
			}

			if (interactables.size() == 0) return -1;
			// Sort Interactables by transform.z to see which is infront
			std::sort(interactables.begin(), interactables.end(), [](TexturedRectTransform& l, TexturedRectTransform& r) {
				return l.rectTransform.transform.getZ() < r.rectTransform.transform.getZ();
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