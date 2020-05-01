#include "RectMask.h"
#include "components/graphics/Sprite.h"
#include "components/RectTransform.h"

using namespace Core;


RectMask::RectMask() {
}

RectMask::~RectMask() {
}


void RectMask::start() {
	onTransformChanged();
}

void RectMask::onTransformChanged() {
	updateMask();
}

void RectMask::onChildAdded(EntityHandle entity) {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		auto vertices = rect->getVertices();
		for (Sprite* sprite : entity.getComponentsDownwards<Sprite>(true)) {
			sprite->clip(getComponentID(), vertices);
			childSprites.push_back(ComponentHandle(sprite));
		}
	}
}

void RectMask::onChildRemoved(EntityHandle entity) {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		for (Sprite* sprite : entity.getComponentsDownwards<Sprite>(true)) {
			sprite->removeClip(getComponentID());
			bool removed = false;
			for (auto it = childSprites.begin(); it != childSprites.end(); it++) {
				if (it->getComponent()->getComponentID() == sprite->getComponentID()) {
					it = childSprites.erase(it);
					removed = true;
					break;
				}
			}
			if (!removed) {
				std::cout << "RectMask::onChildRemoved::ERROR Failed to find sprite in childSprites collection" << std::endl;
				throw std::invalid_argument("RectMask::onChildRemoved::ERROR Failed to find sprite in childSprites collection");
			}
		}
	}
}

void RectMask::onChildChanged(EntityHandle entity) {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		auto vertices = rect->getVertices();
		for (Sprite* sprite : entity.getComponentsDownwards<Sprite>(true)) {
			if (!sprite->hasMaskFromSender(getComponentID())) {
				sprite->clip(getComponentID(), vertices);
				childSprites.push_back(ComponentHandle(sprite));
			}
		}
	}
}

void RectMask::updateMask() {
	if (RectTransform* rect = owner.getComponent<RectTransform>()) {
		auto vertices = rect->getVertices();
		for (ComponentHandle& sprite : childSprites) {
			sprite.getComponent<Sprite>()->reclip(getComponentID(), vertices);
		}
	}
}