#include "RectMask.h"
#include "Sprite.h"
#include "RectTransform.h"

using namespace Core;


RectMask::RectMask() {
}

RectMask::~RectMask() {
}


void RectMask::onTransformChanged() {
	updateMask();
}

void RectMask::onChildAdded(Handle entity) {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		auto vertices = rect->getVertices();
		for (Sprite* sprite : entity.getComponentsDownwards<Sprite>()) {
			std::size_t maskIndex = sprite->clip(vertices);
			std::size_t componentID = sprite->getComponentID();
			maskMap[componentID] = maskIndex;
		}
	}
}

void RectMask::onChildRemoved(Handle entity) {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		for (Sprite* sprite : entity.getComponentsDownwards<Sprite>()) {
			std::size_t componentID = sprite->getComponentID();
			sprite->removeClip(maskMap[componentID]);
		}
	}
}

void RectMask::onChildChanged(Handle entity) {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		auto vertices = rect->getVertices();
		for (Sprite* sprite : entity.getComponentsDownwards<Sprite>()) {
			// Adds mask if the component was not already given one from this component
			std::size_t componentID = sprite->getComponentID();
			auto it = maskMap.find(componentID);
			if (it == maskMap.end()) {
				std::size_t maskIndex = sprite->clip(vertices);
				std::size_t componentID = sprite->getComponentID();
				maskMap[componentID] = maskIndex;
			}
		}
	}
}

void RectMask::updateMask() {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		auto vertices = rect->getVertices();
		for (Sprite* sprite : owner.getComponentsInChildren<Sprite>()) {
			std::size_t componentID = sprite->getComponentID();
			std::size_t maskIndex = maskMap[componentID];
			sprite->reclip(maskIndex, vertices);
		}
	}
}