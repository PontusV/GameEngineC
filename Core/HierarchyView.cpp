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
using namespace Core;


HierarchyView::HierarchyView(ComponentHandle editor) : editor(editor) {}
HierarchyView::~HierarchyView() {}


void HierarchyView::onMouseButtonPressed(int buttoncode, int mods) {
	EntityHandle target = input->getLastClicked();
	if (target.getEntity() != currentTarget.getEntity()) {
		currentTarget = target;
	}
}

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
		group->paddingRight = 10;
	}

	refresh();
	timer = refreshTime;
}

std::vector<EntityHandle> HierarchyView::getRootEntities(std::vector<EntityHandle>& allEntities) {
	std::vector<EntityHandle> entities;
	for (EntityHandle& entity : allEntities) {
		if (!entity.hasParent())
		entities.push_back(entity);
	}
	return entities;
}

std::vector<EntityHandle> HierarchyView::getAllEntities() {
	std::vector<EntityHandle> entities;
	Scene* editorScene = owner.getScene();
	for (const ScenePtr& scene : sceneManager->getAllScenes()) {
		if (scene.get() == editorScene) continue;
		for (EntityHandle entity : scene->getAllEntities()) {
			if (entity.getEntityHideFlags() == HideFlags::HideInHierarchy) continue;
			entities.push_back(entity);
		}
	}
	return entities;
}

void HierarchyView::onTargetEntityClick(EntityHandle entity) {
	editor.getComponent<EditorPanel>()->selectTarget(entity);
}

void HierarchyView::onDestroyEntityClick(EntityHandle entity) {
	destroyEntity(entity);
}

void HierarchyView::clearList() {
	for (auto& entry : list) {
		destroyEntity(entry.second);
	}
	list.clear();
}

std::vector<EntityHandle>::iterator contains(std::vector<EntityHandle>& entities, const Entity& entity) {
	for (auto it = entities.begin(); it != entities.end(); it++) {
		if (it->getEntity() == entity) return it;
	}
	return entities.end();
}

void HierarchyView::addEntry(EntityHandle& entity, Handle& parent, RectTransform* rect) {
	static std::size_t entryHeight = 20;
	std::string name = entity.getEntityName();
	std::string entityName = "Hierarchy_entry_" + std::to_string(list.size());
	Text text = Text(name, "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255));

	std::size_t childCount = entity.getChildCount();
	std::size_t height = entryHeight * (childCount + 1);
	std::size_t width = text.getSize().x + 12;

	EntityHandle entry = createEntity(entityName,
		RectTransform(0, 0, width, height, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	VerticalLayoutGroup* layoutGroup = entry.addComponent<VerticalLayoutGroup>();
	layoutGroup->childForceExpandWidth = false;
	layoutGroup->childForceExpandHeight = false;
	layoutGroup->shrinkableChildWidth = false;
	layoutGroup->shrinkableChildHeight = false;
	layoutGroup->paddingLeft = 10;
	layoutGroup->spacing = 5;

	RectButton button = RectButton();
	button.colors[RectButton::ButtonState::DEFAULT] = Color(255, 255, 255, 0);
	button.colors[RectButton::ButtonState::HOVER_OVER] = Color(255, 255, 255, 80);
	button.colors[RectButton::ButtonState::PRESSED_DOWN] = Color(0, 0, 0, 80);
	button.onLeftClick = Core::bind(this, &HierarchyView::onTargetEntityClick, entity);
	button.onRightClick = Core::bind(this, &HierarchyView::onDestroyEntityClick, entity);
	EntityHandle buttonEntity = createEntity(entityName + "_Button",
		button,
		RectSprite(),
		RectTransform(0, 0, width, entryHeight, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	EntityHandle label = createEntity(entityName + "_Label",
		text,
		RectTransform(5, 0, width, text.getSize().y, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	label.setParent(buttonEntity);
	buttonEntity.setParent(entry);
	entry.setParent(parent);
	list.push_back(std::make_pair(entity.getEntity(), entry));
	// Add children
	for (std::size_t i = 0; i < childCount; i++) {
		EntityHandle child = entity.getChild(i);
		if (child.getEntityHideFlags() == HideFlags::HideInHierarchy) continue;
		addEntry(child, entry, rect);
	}
}

void HierarchyView::refresh() {
	std::vector<EntityHandle> entities = getAllEntities();
	// Remove destroyed Entries
	auto it = list.begin();
	while (it != list.end()) {
		auto iterator = contains(entities, it->first);
		if (iterator != entities.end()) {
			it++;
			entities.erase(iterator);
		}
		else {
			destroyEntity(it->second);
			list.erase(it);
		}
	}
	std::vector<EntityHandle> rootEntities = getRootEntities(entities);
	// Add new Entries
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (!rect) return;
	for (EntityHandle& entity : rootEntities) {
		addEntry(entity, owner, rect);
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