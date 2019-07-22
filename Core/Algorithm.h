#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Texture2D.h"
#include "RectTransform.h"
#include "TransformMaths.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <vector>
#include <array>

namespace Core {
	namespace maths {
		struct RectTransformEntry {
			RectTransform rect;
			Texture2D texture;
			std::size_t id;
			std::vector<std::array<glm::vec2, 4>> masks;
		};

		/* Returns true if the position is inside the rect. */
		bool hitCheck(float posX, float posY, const RectTransform& transform) {
			glm::vec2 localPosition = transform.getWorldToLocalMatrix() * glm::inverse(transform.getLocalModelMatrix()) * glm::vec2(posX, posY);
			Rect rect = transform.getRect();

			return (
				localPosition.x > rect.x &&
				localPosition.x < rect.x + rect.w &&
				localPosition.y > rect.y &&
				localPosition.y < rect.y + rect.h
			);
		}

		float dot(glm::vec2 u, glm::vec2 v) {
			return u.x * v.x + u.y * v.y;
		}

		bool hitCheck(float posX, float posY, const std::array<glm::vec2, 4>& vertices) {
			glm::vec2 AB = glm::vec2(vertices[1].x - vertices[0].x, vertices[1].y - vertices[0].y);
			glm::vec2 AM = glm::vec2(posX - vertices[0].x, posY - vertices[0].y);
			glm::vec2 AD = glm::vec2(vertices[3].x - vertices[0].x, vertices[3].y - vertices[0].y);
			float dotAMAB = dot(AM, AB);
			float dotAMAD = dot(AM, AD);
			return 0 <= dotAMAB && dotAMAB <= dot(AB, AB) && 0 <= dotAMAD && dotAMAD < dot(AD, AD);
		}

		/* Returns true if all Rectangles were hit. */
		bool hitCheckCollection(float posX, float posY, const std::vector<std::array<glm::vec2, 4>>& vertices) {
			for (const std::array<glm::vec2, 4>& rectangle : vertices) {
				if (!hitCheck(posX, posY, rectangle)) {
					return false;
				}
			}
			return true;
		}

		/* Returns the id of the TexturedRectTransform that is infront if there are any at the position. Returns 0 if none was found. */
		std::size_t hitDetect(const float xPoint, const float yPoint, const std::vector<RectTransformEntry>& rectTransforms, bool alpha = false) {
			std::vector<RectTransformEntry> interactables;

			for (const RectTransformEntry& rectTransform : rectTransforms) {
				// Simple hit detection
				if (hitCheck(xPoint, yPoint, rectTransform.rect) && hitCheckCollection(xPoint, yPoint, rectTransform.masks)) {
					if (!alpha || rectTransform.texture.getAlphaAtPoint((int)(xPoint - rectTransform.rect.getPosition().x), (int)(yPoint - rectTransform.rect.getPosition().y)) > 0.0f) // Expensive
						interactables.push_back(rectTransform);
				}
			}

			if (interactables.size() == 0) return 0;
			// Sort Interactables by transform.z to see which is infront
			std::sort(interactables.begin(), interactables.end(), [](RectTransformEntry& l, RectTransformEntry& r) {
				return l.rect.getZ() > r.rect.getZ();
			});
			return interactables[0].id;
		}

		bool isInsideWindow(int cameraX, int cameraY, int windowWidth, int windowHeight, const RectTransform& rect) {
			// TODO: Check every corner or if a rect line crosses the window
			// Currently only checks if pivot point is inside window
			glm::vec2 rectPosition = rect.getPosition(); // Pivot point in world space, TODO: Convert to screen space

			int relativeX = (int)(rectPosition.x - cameraX); // Offset
			int relativeY = (int)(rectPosition.y - cameraY); // Offset
			return (relativeX > 0 && relativeX < windowWidth &&
					relativeY > 0 && relativeY < windowHeight);
		}
	}
}

#endif