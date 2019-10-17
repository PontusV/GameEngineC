#include "HierarchyView.h"
#include "RectTransform.h"
#include "VerticalLayoutGroup.h"
#include "HorizontalLayoutGroup.h"
#include "LayoutElement.h"
#include "Text.h"
#include "RectButton.h"
#include "RectSprite.h"
#include "EditorPanel.h"
#include "Input.h"
#include "SceneManager.h"
#include <algorithm>
using namespace Core;


HierarchyView::HierarchyView(ComponentHandle editor) : editor(editor) {}
HierarchyView::~HierarchyView() {}


void HierarchyView::onEnable() {
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (rect) {
		LayoutElement* element = owner.getComponent<LayoutElement>();
		element->setFlexibleSize(Vector2(1, 1));
		element->setFlexibleSizeEnabled(true);
		element->setMinSize(Vector2(0, 0));
		element->setMinSizeEnabled(true);

		VerticalLayoutGroup* group = owner.getComponent<VerticalLayoutGroup>();
		group->childForceExpandHeight = false;
		group->childForceExpandWidth = true;
		group->shrinkableChildHeight = false;
		group->shrinkableChildWidth = true;
		group->spacing = 5;
		group->paddingTop = 10;
		group->paddingLeft = 0;
		group->paddingRight = 0;
	}

	refresh();
	timer = refreshTime;
}

std::size_t HierarchyView::getOrder(EntityHandle& entity, EntityHandle& parent, std::vector<Handle>& rootEntities) const {
	if (parent.isValid()) {
		std::size_t childCount = parent.getChildCount();
		for (std::size_t i = 0; i < childCount; i++) {
			if (parent.getChild(i) == entity) {
				return i;
			}
		}
	}
	else {
		std::size_t i = 0;
		for (Handle& root : rootEntities) {
			if (root == entity) return i;
			i++;
		}
	}
	return 0;
}

std::size_t HierarchyView::getDepth(Handle parent) const {
	std::size_t depth = 0;
	while (parent.isValid()) {
		parent = parent.getParent();
		depth++;
	}
	return depth;
}

std::vector<HierarchyEntry> HierarchyView::getAllEntities() {
	std::vector<HierarchyEntry> entities;
	Scene* editorScene = owner.getScene();
	for (const ScenePtr& scene : sceneManager->getAllScenes()) {
		if (scene.get() == editorScene) continue;
		std::vector<Handle> rootEntities = scene->getRootEntities();
		for (EntityHandle entity : scene->getAllEntities()) {
			if (entity.getEntityHideFlags() == HideFlags::HideInHierarchy) continue;
			EntityHandle parent = entity.getParent();
			std::size_t order = getOrder(entity, parent, rootEntities);
			std::size_t depth = getDepth(parent);
			entities.push_back(HierarchyEntry(entity, parent, order, depth));
		}
	}
	return entities;
}

void HierarchyView::target(EntityHandle entity) {
	auto it = listMap.find(currentTarget.getEntity());
	if (it != listMap.end()) {
		it->second.highlight.getComponent<RectSprite>()->setColor(Color(0, 0, 0, 0));
	}
	currentTarget = entity;
	it = listMap.find(currentTarget.getEntity());
	if (it != listMap.end()) {
		it->second.highlight.getComponent<RectSprite>()->setColor(highlightColor);
	}
}

void HierarchyView::onTargetEntityClick(EntityHandle entity) {
	editor.getComponent<EditorPanel>()->selectTarget(entity);
}

void HierarchyView::onDestroyEntityClick(EntityHandle entity) {
	destroyEntity(entity);
}

void HierarchyView::clearList() {
	for (auto& entry : listMap) {
		destroyEntity(entry.second.entry);
	}
	listMap.clear();
	list.clear();
}

void HierarchyView::addEntry(EntityHandle& entity, Handle& parent, std::size_t depth, RectTransform* rect) {
	static std::size_t entryHeight = 20;
	std::string name = entity.getEntityName();
	std::string entityName = "Hierarchy_entry_" + entity.getEntityName();
	Text text = Text(name, "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255));

	std::size_t childCount = entity.getChildCount();
	std::size_t height = entryHeight * (childCount + 1);
	std::size_t width = text.getSize().x + 12;

	EntityHandle entry = createEntity(entityName,
		RectTransform(0, 0, width, height, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	VerticalLayoutGroup* layoutGroup = entry.addComponent<VerticalLayoutGroup>();
	layoutGroup->childForceExpandWidth = true;
	layoutGroup->childForceExpandHeight = false;
	layoutGroup->shrinkableChildWidth = true;
	layoutGroup->shrinkableChildHeight = false;
	layoutGroup->spacing = 5;

	RectButton button = RectButton();
	button.colors[RectButton::ButtonState::DEFAULT] = Color(255, 255, 255, 0);
	button.colors[RectButton::ButtonState::HOVER_OVER] = Color(255, 255, 255, 80);
	button.colors[RectButton::ButtonState::PRESSED_DOWN] = Color(0, 0, 0, 80);
	button.onLeftClick = Core::bind(this, &HierarchyView::onTargetEntityClick, entity);
	button.onRightClick = Core::bind(this, &HierarchyView::onDestroyEntityClick, entity);
	EntityHandle highlight = createEntity(entityName + "_Background",
		RectSprite(Color(highlightColor.r, highlightColor.g, highlightColor.b, entity == currentTarget ? highlightColor.a : 0)),
		RectTransform(0, 0, 0, entryHeight, rect->getZ() + 0.01f, Alignment::LEFT)
	);
	EntityHandle buttonEntity = createEntity(entityName + "_Button",
		button,
		RectSprite(),
		RectTransform((depth + 1) * 10, 0, width, entryHeight, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	EntityHandle label = createEntity(entityName + "_Label",
		text,
		RectTransform(5, 0, width, text.getSize().y, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	highlight.setParent(entry);
	label.setParent(buttonEntity);
	buttonEntity.setParent(highlight);
	entry.setParent(parent);
	listMap[entity.getEntity()] = { entry, highlight };
}

bool HierarchyView::isDirty(HierarchyEntry entry) const {
	for (const HierarchyEntry& element : list) {
		if (element.entity == entry.entity) {
			return element.depth != entry.depth || element.order != entry.order || element.parent != entry.parent;
		}
	}
	return false;
}

std::vector<HierarchyEntry>::iterator contains(std::vector<HierarchyEntry>& entities, const Entity& entity) {
	for (auto it = entities.begin(); it != entities.end(); it++) {
		if (it->entity.getEntity() == entity) return it;
	}
	return entities.end();
}

void HierarchyView::refresh() {
	std::vector<HierarchyEntry> entities = getAllEntities();
	// Remove destroyed Entries
	auto it = list.begin();
	while (it != list.end()) {
		auto iterator = contains(entities, it->entity.getEntity());
		if (iterator != entities.end() && !isDirty(*it)) {
			it++;
			entities.erase(iterator);
		}
		else {
			destroyEntity(listMap[it->entity.getEntity()].entry);
			listMap.erase(it->entity.getEntity());
			list.erase(it);
		}
	}
	// Add new Entries
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (!rect) return;
	std::sort(entities.begin(), entities.end(), [](const HierarchyEntry& lhs, const HierarchyEntry& rhs) {
		return lhs.depth < rhs.depth;
	});
	for (HierarchyEntry& newEntry : entities) {
		if (newEntry.parent.isValid())
			addEntry(newEntry.entity, listMap[newEntry.parent.getEntity()].entry, newEntry.depth, rect);
		else
			addEntry(newEntry.entity, owner, newEntry.depth, rect);
		list.push_back(newEntry);
	}
}

void HierarchyView::onDisable() {
	clearList();
}

void HierarchyView::lateUpdate(float deltaTime) {
	timer -= deltaTime;
	if (timer <= 0) {
		timer = refreshTime;
		refresh();
	}
}