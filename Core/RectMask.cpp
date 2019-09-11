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
			sprite->clip(getComponentID(), vertices);
		}
	}
}

void RectMask::onChildRemoved(Handle entity) {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		for (Sprite* sprite : entity.getComponentsDownwards<Sprite>()) {
			sprite->removeClip(getComponentID());
		}
	}
}

void RectMask::onChildChanged(Handle entity) {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		auto vertices = rect->getVertices();
		for (Sprite* sprite : entity.getComponentsDownwards<Sprite>()) {
			if (!sprite->hasMaskFromSender(getComponentID()))
				sprite->clip(getComponentID(), vertices);
		}
	}
}

void RectMask::updateMask() {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		auto vertices = rect->getVertices();
		for (Sprite* sprite : owner.getComponentsInChildren<Sprite>()) {
			sprite->reclip(getComponentID(), vertices);
		}
	}
}