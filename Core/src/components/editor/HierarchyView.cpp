#include "HierarchyView.h"
#include "components/RectTransform.h"
#include "components/ui/input/Button.h"
#include "components/ui/input/RectButton.h"
#include "components/ui/layout/VerticalLayoutGroup.h"
#include "components/ui/layout/HorizontalLayoutGroup.h"
#include "components/ui/layout/LayoutElement.h"
#include "components/graphics/Image.h"
#include "components/graphics/Text.h"
#include "components/graphics/RectSprite.h"
#include "components/entity/ChildManager.h"
#include "HierarchySceneMover.h"
#include "HierarchyEntityMover.h"
#include "HierarchyOrderRect.h"
#include "EditorPanel.h"
#include "input/Input.h"
#include "scene/SceneManager.h"
#include "engine/ResourceManager.h"
#include "maths/MatrixTransform.h"
#include <algorithm>
using namespace Core;


const std::size_t SCENE_ENTRY_HEIGHT = 20;
const std::size_t ENTITY_ENTRY_HEIGHT = 16;
const std::size_t ORDER_RECT_HEIGHT = 8;

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
		group->spacing = 0;
		group->paddingTop = 10;
		group->paddingLeft = 0;
		group->paddingRight = 0;
	}

	timer = refreshTime;
}

std::size_t HierarchyView::getRootIndex(const Handle& handle) {
	auto it = rootMap.find(handle.getScene()->getName());
	if (it == rootMap.end()) {
		std::cout << "HierarchyView::getRootIndex::ERROR The Scene does not exist in the rootMap.";
		return 0;
	}
	std::vector<Entity>& roots = it->second;
	auto entityIt = std::find(roots.begin(), roots.end(), handle.getEntity());
	if (entityIt == roots.end()) {
		roots.push_back(handle.getEntity());
		return roots.size() - 1;
	}
	return entityIt - roots.begin();
}

void HierarchyView::setRootIndex(const Handle& handle, std::size_t index) {
	auto it = rootMap.find(handle.getScene()->getName());
	if (it == rootMap.end()) return;
	std::vector<Entity>& roots = it->second;
	if (index >= roots.size()) index = roots.size();
	auto entityIt = std::find(roots.begin(), roots.end(), handle.getEntity());
	if (entityIt != roots.end()) {
		auto eraseIt = roots.erase(entityIt);
		if (index > eraseIt - roots.begin()) index--;
	}
	roots.insert(roots.begin() + index, handle.getEntity());
}

std::vector<HierarchyEntry> HierarchyView::getAllEntities() {
	std::vector<HierarchyEntry> entities;
	Scene* editorScene = owner.getScene();
	for (const ScenePtr& scene : sceneManager->getAllScenes()) {
		if (scene.get() == editorScene) continue;
		for (EntityHandle entity : scene->getAllEntities()) {
			if (entity.getEntityHideFlags() == HideFlags::HideInHierarchy) continue;
			EntityHandle parent = entity.getParent();
			std::size_t order = entity.hasParent() ? entity.getSiblingIndex() : getRootIndex(entity);
			std::size_t depth = entity.getDepth();
			entities.push_back(HierarchyEntry(entity, parent, order, depth));
		}
	}
	return entities;
}

void HierarchyView::target(EntityHandle entity) {
	auto it = listMap.find(currentTarget.getEntity());
	if (it != listMap.end()) {
		it->second.getChild(0).getComponent<RectSprite>()->setColor(Color(0, 0, 0, 0));
	}
	currentTarget = entity;
	it = listMap.find(currentTarget.getEntity());
	if (it != listMap.end()) {
		it->second.getChild(0).getComponent<RectSprite>()->setColor(highlightColor);
	}
}

void HierarchyView::onTargetEntityClick(EntityHandle entity) {
	editor.getComponent<EditorPanel>()->selectTarget(entity);
}

void HierarchyView::onDestroyEntityClick(EntityHandle entity) {
	destroyEntity(entity);
}

void HierarchyView::clearList() {
	for (auto& entry : sceneMap) {
		destroyEntity(entry.second);
	}
	sceneMap.clear();
	sceneContentMap.clear();
	for (auto& entry : listMap) {
		destroyEntity(entry.second);
	}
	listMap.clear();
	contentMap.clear();
	list.clear();
}

EntityHandle HierarchyView::createOrderRect(std::string name, EntityHandle entity, std::size_t order, RectTransform* rect) {
	return createEntity(name + "_Order_Rect",
		HierarchyOrderRect(entity, order),
		RectSprite(Color(0, 0, 0, 0)),
		RectTransform(0, 20, 0, ORDER_RECT_HEIGHT, rect->getZ() + 0.3f, Alignment::CENTER)
	);
}

void HierarchyView::createEntityEntry(HierarchyEntry& entry, RectTransform* rect) {
	EntityHandle& entity = entry.entity;
	EntityHandle parent = entry.parent.isValid() ? contentMap[entry.parent.getEntity()] : sceneContentMap.at(entity.getScene()->getName());
	std::size_t depth = entry.depth + 1;
	std::size_t& order = entry.order;

	std::string name = entity.getEntityName();
	std::string entityName = "Hierarchy_entry_" + name;
	Text text = Text(name, "resources/fonts/segoeui.ttf", 15, Color(255, 255, 255));
	std::size_t width = text.getSize().x + 12;

	VerticalLayoutGroup layoutGroup = VerticalLayoutGroup();
	layoutGroup.childForceExpandWidth = true;
	layoutGroup.childForceExpandHeight = false;
	layoutGroup.shrinkableChildWidth = true;
	layoutGroup.shrinkableChildHeight = false;
	layoutGroup.spacing = 0;
	EntityHandle entryHandle = createEntity(entityName,
		layoutGroup,
		RectTransform(0, 0, width, 0, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	entryHandle.setParent(parent);
	EntityHandle entryLine = createEntity(entityName + "_Line",
		RectSprite(Color(highlightColor.r, highlightColor.g, highlightColor.b, entity == currentTarget ? highlightColor.a : 0)),
		RectTransform(0, 0, 0, ENTITY_ENTRY_HEIGHT, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	entryLine.setParent(entryHandle);
	RectButton button = RectButton();
	button.colors[RectButton::ButtonState::DEFAULT] = Color(255, 255, 255, 0);
	button.colors[RectButton::ButtonState::HOVER_OVER] = Color(255, 255, 255, 80);
	button.colors[RectButton::ButtonState::PRESSED_DOWN] = Color(0, 0, 0, 80);
	button.onLeftClick = Core::bind(this, &HierarchyView::onTargetEntityClick, entity);
	button.onRightClick = Core::bind(this, &HierarchyView::onDestroyEntityClick, entity);
	EntityHandle buttonEntity = createEntity(entityName + "_Button", // TODO: Force expand width. Use RectTransform Anchors? Check Unity Engine https://docs.unity3d.com/ScriptReference/RectTransform.html
		button,
		RectSprite(),
		HierarchyEntityMover(ComponentHandle(this), entity),
		RectTransform(0, 0, 0, ENTITY_ENTRY_HEIGHT, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	buttonEntity.setParent(entryLine);
	Shader figureShader = ResourceManager::getInstance().loadShader("resources/shaders/figure");
	Button collapsibleButton = Button(Image("resources/images/ui/arrow.png", figureShader, Color(150, 150, 150)), Image("resources/images/ui/arrow.png", figureShader, Color(0, 0, 0)), Image("resources/images/ui/arrow.png", figureShader, Color(255, 255, 255)));
	//collapsibleButton.onLeftClick = Core::bind(this, &Inspector::collapse, collapsibleContent, collapsibleIcon);
	EntityHandle collapsibleIcon = createEntity(entityName + "_Collapsible_Icon",
		collapsibleButton,
		Image("resources/images/ui/arrow.png", figureShader),
		RectTransform(depth * 20, 0, 12, 12, rect->getZ() + 0.21f, Alignment::CENTER, maths::radians(90))
	);
	collapsibleIcon.setParent(entryLine);
	EntityHandle label = createEntity(entityName + "_Label",
		text,
		RectTransform(depth * 20 + 10, 0, width, text.getSize().y, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	label.setParent(buttonEntity);

	// Order Rects
	createOrderRect(entityName, entry.entity, order, rect).setParent(entryHandle);

	EntityHandle contentHandle = createEntity(entityName + "_Content",
		layoutGroup,
		RectTransform(0, 0, 0, 0, 0, Alignment::TOP_LEFT)
	);
	contentHandle.setParent(entryHandle);

	listMap[entity.getEntity()] = entryHandle;
	contentMap[entity.getEntity()] = contentHandle;
	std::size_t index = 0;

	// Sort new entry
	auto it = list.begin();
	while (it != list.end()) {
		const HierarchyEntry& rhs = *it;
		if (depth < rhs.depth || depth == rhs.depth && order < rhs.order) break;
		it++;
	}
	list.insert(it, entry);
}

void HierarchyView::createSceneEntry(std::string name, Scene* scene, RectTransform* rect) {
	static const std::size_t depth = 0;
	std::size_t order = sceneMap.size();

	std::string entityName = "Hierarchy_entry_" + name;
	Text text = Text(name, "resources/fonts/segoeui.ttf", 15, Color(0, 0, 0));
	std::size_t width = text.getSize().x + 12;

	VerticalLayoutGroup layoutGroup = VerticalLayoutGroup();
	layoutGroup.childForceExpandWidth = true;
	layoutGroup.childForceExpandHeight = false;
	layoutGroup.shrinkableChildWidth = true;
	layoutGroup.shrinkableChildHeight = false;
	layoutGroup.spacing = 0;
	EntityHandle entryHandle = createEntity(entityName,
		layoutGroup,
		RectTransform(0, 0, width, 0, rect->getZ() + 0.1f, Alignment::LEFT)
	);
	entryHandle.setParent(owner);
	EntityHandle header = createEntity(entityName + "_Header",
		HierarchySceneMover(ComponentHandle(this), scene),
		RectSprite(Color(200, 200, 200, 255)),
		RectTransform(0, 0, width, SCENE_ENTRY_HEIGHT, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	header.setParent(entryHandle);
	EntityHandle label = createEntity(entityName + "_Label",
		text,
		RectTransform(5, 0, width, text.getSize().y, rect->getZ() + 0.2f, Alignment::LEFT)
	);
	label.setParent(header);
	// Order Rect
	createOrderRect(entityName, EntityHandle(Entity(Entity::INVALID_ID), scene), 0, rect).setParent(entryHandle);

	EntityHandle contentHandle = createEntity(entityName + "_Content",
		layoutGroup,
		RectTransform(0, 0, 0, 0, 0, Alignment::TOP_LEFT)
	);
	contentHandle.setParent(entryHandle);

	sceneMap.insert(std::make_pair(name, entryHandle));
	sceneContentMap.insert(std::make_pair(name, contentHandle));
}

std::vector<HierarchyEntry>::iterator contains(std::vector<HierarchyEntry>& entities, const Entity& entity) {
	for (auto it = entities.begin(); it != entities.end(); it++) {
		if (it->entity.getEntity() == entity) return it;
	}
	return entities.end();
}

std::vector<std::pair<std::string, ScenePtr>>::iterator contains(std::vector<std::pair<std::string, ScenePtr>>& scenes, const std::string& sceneName) {
	for (auto it = scenes.begin(); it != scenes.end(); it++) {
		if (it->first == sceneName) return it;
	}
	return scenes.end();
}

bool HierarchyView::isDirty(HierarchyEntry entry) {
	for (const HierarchyEntry& element : list) {
		if (element.entity == entry.entity) {
			if (element.depth != entry.depth || element.order != entry.order || element.parent != entry.parent) return true;
			if (entry.parent.isValid()) {
				auto it = contains(list, entry.parent.getEntity());
				if (it == list.end()) return true;
				return isDirty(*it);
			}
		}
	}
	return false;
}

void HierarchyView::refresh() {
	RectTransform* rect = owner.getComponent<RectTransform>();
	if (!rect) return;
	// Scenes
	std::vector<std::pair<std::string, ScenePtr>> scenes = sceneManager->getAllScenesAsPairs();
	// Remove destroyed Scenes
	for (auto sceneIt = sceneMap.begin(); sceneIt != sceneMap.end();) {
		auto sceneIterator = contains(scenes, sceneIt->first);
		if (sceneIterator != scenes.end()) {
			scenes.erase(sceneIterator);
			sceneIt++;
		}
		else {
			EntityHandle& entry = sceneIt->second;
			for (auto it = list.begin(); it != list.end();) {
				if (it->entity.getScene()->getName() == sceneIt->first) {
					listMap.erase(it->entity.getEntity());
					it = list.erase(it);
				}
				else {
					it++;
				}
			}
			destroyEntity(entry);
			auto rootMapIt = rootMap.find(sceneIt->first);
			rootMap.erase(rootMapIt);
			destroyEntity(sceneIt->second);
			sceneMap.erase(sceneIt);
			sceneContentMap.erase(sceneIt->first);
		}
	}
	// Add new scenes
	for (std::pair<std::string, ScenePtr>& scenePair : scenes) {
		createSceneEntry(scenePair.first, scenePair.second.get(), rect);
		rootMap.insert(std::make_pair(scenePair.first, std::vector<Entity>{}));
	}
	// Entities
	std::vector<HierarchyEntry> entities = getAllEntities();
	// Remove destroyed Entries
	auto it = list.begin();
	while (it != list.end()) {
		auto iterator = contains(entities, it->entity.getEntity());
		if (iterator != entities.end() && !isDirty(*iterator)) {
			it++;
			iterator = entities.erase(iterator);
		}
		else {
			if (iterator != entities.end() && it->depth == 0 && iterator->depth != 0) { // Check if root converting to child
				auto rootMapIt = rootMap.find(it->entity.getScene()->getName());
				if (rootMapIt != rootMap.end()) {
					std::vector<Entity>& roots = rootMapIt->second;
					auto rootIt = std::find(roots.begin(), roots.end(), it->entity.getEntity());
					if (rootIt != roots.end()) {
						roots.erase(rootIt);
					}
					else {
						std::cout << "HierarchyView::refresh::ERROR The Entity did not have a root order.";
					}
				}
				else {
					std::cout << "HierarchyView::refresh::ERROR The Scene does not exist in the rootMap.";
				}
			}
			destroyEntity(listMap[it->entity.getEntity()]);
			listMap.erase(it->entity.getEntity());
			contentMap.erase(it->entity.getEntity());
			it = list.erase(it);
		}
	}
	// Add new Entries
	std::sort(entities.begin(), entities.end(), [](const HierarchyEntry& lhs, const HierarchyEntry& rhs) {
		if (lhs.depth < rhs.depth) return true;
		if (lhs.depth > rhs.depth) return false;
		return lhs.order < rhs.order;
	});
	for (HierarchyEntry& newEntry : entities) {
		createEntityEntry(newEntry, rect);
		// Updating dirty order map
		Entity parent(0);
		if (newEntry.parent.isValid())
			parent = newEntry.parent.getEntity();

		auto it = dirtyOrderMap.find(parent);
		if (it != dirtyOrderMap.end()) {
			if (it->second > newEntry.order)
				it->second = newEntry.order;
		}
		else {
			dirtyOrderMap.insert(std::make_pair(parent, newEntry.order));
		}
	}
}

void HierarchyView::onDisable() {
	clearList();
}

void HierarchyView::updateOrder() {
	for (auto orderIt = dirtyOrderMap.begin(); orderIt != dirtyOrderMap.end(); orderIt++) {
		// Update order
		const Entity& entity = orderIt->first;
		std::size_t& order = orderIt->second;

		for (HierarchyEntry& entry : list) {
			if (entry.order < order && entry.entity.getEntity() != entity) continue;
			EntityHandle entityEntry = getEntryHandle(entry.entity.getEntity());
			if (!entityEntry.isValid())
				std::cout << "HierarchyView::UpdateOrder::ERROR Entity does not have any Entry" << std::endl;
			else if (entityEntry.hasParent())
				entityEntry.setSiblingIndex(entry.order);
			else std::cout << entityEntry.getEntityName() << std::endl;
		}

		// Force update layout
		auto contentMapIt = contentMap.find(entity);
		if (contentMapIt != contentMap.end()) {
			if (LayoutGroup* layout = contentMapIt->second.getComponent<LayoutGroup>()) {
				layout->setDirty();
			}
		}
	}
	dirtyOrderMap.clear();
}

void HierarchyView::lateUpdate(float deltaTime) {
	updateOrder();
	timer -= deltaTime;
	if (timer <= 0) {
		timer = refreshTime;
		refresh();
	}
}

EntityHandle HierarchyView::getEntryHandle(const Entity& entity) {
	auto entryIt = listMap.find(entity);
	if (entryIt == listMap.end())
		return EntityHandle();
	return entryIt->second;
}