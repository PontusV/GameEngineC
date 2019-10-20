#include "EditorPanel.h"
#include "Inspector.h"
#include "HierarchyView.h"
#include "components/graphics/Text.h"
#include "components/graphics/RectSprite.h"
#include "components/graphics/ui/RectButton.h"
#include "components/graphics/ui/LayoutElement.h"
#include "components/graphics/ui/HorizontalLayoutGroup.h"
#include "components/graphics/ui/VerticalLayoutGroup.h"
#include "components/graphics/ui/ScrollRect.h"
#include "components/graphics/ui/ScrollBar.h"
#include "components/graphics/ui/RectMask.h"
#include "components/engine/ObjectData.h"
#include "scene/Scene.h"
#include "input/Input.h"
#include <GLFW/glfw3.h>
using namespace Core;


EditorPanel::EditorPanel() {}
EditorPanel::~EditorPanel() {}



void EditorPanel::start() {
	Scene* sceneUI = owner.getComponent<ObjectData>()->getScene();
	// ----------------------------------------------------Inspector-----------------------------------------------------------------
	int textPadding = 5;
	int backgroundPadding = 3;
	Text inspectorLabelText = Text("Inspector", "resources/fonts/segoeui.ttf", 16, Color(255, 255, 255, 255));
	Text hierarchyLabelText = Text("Hierarchy", "resources/fonts/segoeui.ttf", 16, Color(255, 255, 255, 255));
	Vector2 inspectorLabelSize = inspectorLabelText.getSize();
	int labelRectWidth = inspectorLabelSize.x + textPadding * 2;
	int labelRectHeight = inspectorLabelSize.y + textPadding * 2;
	// Tabs
	EntityHandle tabContainer = sceneUI->createEntity("Tab_Container",
		//HorizontalLayoutGroup(),
		RectTransform(0, 0, 0, 0, 1.04f, Alignment::TOP_LEFT)
	);
	HorizontalLayoutGroup* tabsLayoutGroup = tabContainer.addComponent<HorizontalLayoutGroup>();
	tabsLayoutGroup->spacing = 5;
	tabContainer.setParent(owner);
	// Inspector Tab
	inspectorTab = sceneUI->createEntity("Inspector_Tab",
		RectSprite(Color(100, 100, 100, 255)),
		//RectButton(),
		//LayoutElement(),
		RectTransform(0, 0, labelRectWidth, labelRectHeight, 1.05f, Alignment::TOP_LEFT)
	);
	RectButton* inspectorButton = inspectorTab.addComponent<RectButton>();
	inspectorButton->colors[RectButton::ButtonState::DEFAULT] = Color(80, 80, 80);
	inspectorButton->colors[RectButton::ButtonState::HOVER_OVER] = Color(100, 100, 100);
	inspectorButton->colors[RectButton::ButtonState::PRESSED_DOWN] = Color(40, 40, 40);
	inspectorButton->colors[RectButton::ButtonState::DISABLED] = Color(120, 120, 120);
	inspectorButton->disable();
	LayoutElement* labelRectLE = inspectorTab.addComponent<LayoutElement>();
	labelRectLE->setMinSize(Vector2(labelRectWidth, labelRectHeight));
	labelRectLE->setMinSizeEnabled(true);
	labelRectLE->setFlexibleSize(Vector2(0.0f, 0.0f));
	labelRectLE->setFlexibleSizeEnabled(true);
	inspectorTab.setParent(tabContainer);
	// Inspector label
	EntityHandle inspectorLabel = sceneUI->createEntity("Inspector_label",
		inspectorLabelText,
		RectTransform(textPadding, textPadding, labelRectWidth, labelRectHeight, 1.1f, Alignment::TOP_LEFT)
	);
	inspectorLabel.setParent(inspectorTab);
	// Hierarchy Tab
	Vector2 hierarchyLabelSize = hierarchyLabelText.getSize();
	labelRectWidth = hierarchyLabelSize.x + textPadding * 2;
	labelRectHeight = hierarchyLabelSize.y + textPadding * 2;
	hierarchyTab = sceneUI->createEntity("Hierarchy_Tab",
		RectSprite(Color(100, 100, 100, 255)),
		//RectButton(),
		//LayoutElement(),
		RectTransform(0, 0, labelRectWidth, labelRectHeight, 1.05f, Alignment::TOP_LEFT)
	);
	RectButton* heirarchyButton = hierarchyTab.addComponent<RectButton>();
	heirarchyButton->colors[RectButton::ButtonState::DEFAULT] = Color(80, 80, 80);
	heirarchyButton->colors[RectButton::ButtonState::HOVER_OVER] = Color(100, 100, 100);
	heirarchyButton->colors[RectButton::ButtonState::PRESSED_DOWN] = Color(40, 40, 40);
	heirarchyButton->colors[RectButton::ButtonState::DISABLED] = Color(120, 120, 120);
	labelRectLE = hierarchyTab.addComponent<LayoutElement>();
	labelRectLE->setMinSize(Vector2(labelRectWidth, labelRectHeight));
	labelRectLE->setMinSizeEnabled(true);
	labelRectLE->setFlexibleSize(Vector2(0.0f, 0.0f));
	labelRectLE->setFlexibleSizeEnabled(true);
	hierarchyTab.setParent(tabContainer);
	// Hierarchy label
	EntityHandle hierarchyLabel = sceneUI->createEntity("Hierarchy_label",
		hierarchyLabelText,
		RectTransform(textPadding, textPadding, labelRectWidth, labelRectHeight, 1.1f, Alignment::TOP_LEFT)
	);
	hierarchyLabel.setParent(hierarchyTab);

	// Inspector background
	EntityHandle content = sceneUI->createEntity("EditorPanel_Content",
		RectTransform(0, 0, 0, 0, 0)
	);
	HorizontalLayoutGroup* contentGroup = content.addComponent<HorizontalLayoutGroup>();
	contentGroup->childForceExpandHeight = true;
	contentGroup->childForceExpandWidth = true;
	contentGroup->shrinkableChildHeight = true;
	contentGroup->shrinkableChildWidth = true;
	contentGroup->spacing = 0;
	content.setParent(owner);
	EntityHandle scrollPanel = sceneUI->createEntity("EditorPanel_Scroll_Panel",
		//Inspector(),
		//HierarchyView(),
		RectMask(),
		LayoutElement(),
		VerticalLayoutGroup(),
		RectSprite(Color(175, 0, 0, 255)),
		RectTransform(0, 0, 0, 0, 1.05f, Alignment::TOP_LEFT)
	);
	ScrollRect* scrollRect = scrollPanel.addComponent<ScrollRect>();
	scrollRect->paddingBottom = 10;
	HierarchyView* hierarchyView = scrollPanel.addComponent(HierarchyView(ComponentHandle(this)));
	Inspector* inspectorView = scrollPanel.addComponent<Inspector>();
	hierarchyView->disable();
	inspectorPanel = ComponentHandle(inspectorView);
	hierarchyPanel = ComponentHandle(hierarchyView);
	scrollPanel.setParent(content);

	EntityHandle scrollBar = createEntity("Inspector_Scroll_Bar",
		ScrollBar(scrollPanel),
		RectTransform(0, 0, 20, 500, 1.06f)
	);
	LayoutElement* scrollBarElement = scrollBar.addComponent<LayoutElement>();
	scrollBarElement->setMinSize(Vector2(20, 0));
	scrollBarElement->setMinSizeEnabled(true);
	scrollBarElement->setFlexibleSize(Vector2(0, 1));
	scrollBarElement->setFlexibleSizeEnabled(true);
	scrollBar.setParent(content);

	inspectorButton->onLeftClick = Core::bind(this, &EditorPanel::selectTab, Tab::Inspector); // Display Inspector View
	heirarchyButton->onLeftClick = Core::bind(this, &EditorPanel::selectTab, Tab::Hierarchy); // Display Heirarchy View
}

void EditorPanel::selectTab(Tab tab) {
	Inspector* inspectorView = inspectorPanel.getComponent<Inspector>();
	HierarchyView* hierarchyView = hierarchyPanel.getComponent<HierarchyView>();

	ComponentHandle hierarchyPanel;
	if (tab == Tab::Inspector) {
		inspectorView->enable();
		hierarchyView->disable();
		inspectorTab.getComponent<RectButton>()->disable();
		hierarchyTab.getComponent<RectButton>()->enable();
	}
	else if (tab == Tab::Hierarchy) {
		inspectorView->disable();
		hierarchyView->enable();
		inspectorTab.getComponent<RectButton>()->enable();
		hierarchyTab.getComponent<RectButton>()->disable();
	}
	currentTab = tab;
}

bool isTargetable(EntityHandle target) {
	return target.refresh() && target.getEntityHideFlags() != HideFlags::HideInInspector;
}

void EditorPanel::onMouseButtonPressed(int buttoncode, int mods) {
	if (buttoncode != GLFW_MOUSE_BUTTON_LEFT) return;
	EntityHandle target = input->getLastLeftClicked();
	if (!isTargetable(target)) return;
	if (target.getEntity() != currentTarget.getEntity())
		selectTarget(target);
}

void EditorPanel::selectTarget(EntityHandle target) {
	currentTarget = target;
	inspectorPanel.getComponent<Inspector>()->inspect(target);
	hierarchyPanel.getComponent<HierarchyView>()->target(target);
}