#include "EditorPanel.h"
#include "Inspector.h"
#include "HierarchyView.h"
#include "Text.h"
#include "RectSprite.h"
#include "RectButton.h"
#include "LayoutElement.h"
#include "HorizontalLayoutGroup.h"
#include "ObjectData.h"
#include "Scene.h"
#include "Input.h"
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
	EntityHandle inspectorLabelRect = sceneUI->createEntity("Inspector_Tab",
		RectSprite(Color(100, 100, 100, 255)),
		//RectButton(),
		//LayoutElement(),
		RectTransform(0, 0, labelRectWidth, labelRectHeight, 1.05f, Alignment::TOP_LEFT)
	);
	RectButton* inspectorButton = inspectorLabelRect.addComponent<RectButton>();
	inspectorButton->colors[RectButton::ButtonState::DEFAULT] = Color(100, 100, 100);
	inspectorButton->colors[RectButton::ButtonState::HOVER_OVER] = Color(120, 120, 120);
	inspectorButton->colors[RectButton::ButtonState::PRESSED_DOWN] = Color(80, 80, 80);
	LayoutElement* labelRectLE = inspectorLabelRect.addComponent<LayoutElement>();
	labelRectLE->setMinSize(Vector2(labelRectWidth, labelRectHeight));
	labelRectLE->setMinSizeEnabled(true);
	labelRectLE->setFlexibleSize(Vector2(0.0f, 0.0f));
	labelRectLE->setFlexibleSizeEnabled(true);
	inspectorLabelRect.setParent(tabContainer);
	// Inspector label
	EntityHandle inspectorLabel = sceneUI->createEntity("Inspector_label",
		inspectorLabelText,
		RectTransform(textPadding, textPadding, labelRectWidth, labelRectHeight, 1.1f, Alignment::TOP_LEFT)
	);
	inspectorLabel.setParent(inspectorLabelRect);
	// Hierarchy Tab
	Vector2 hierarchyLabelSize = hierarchyLabelText.getSize();
	labelRectWidth = hierarchyLabelSize.x + textPadding * 2;
	labelRectHeight = hierarchyLabelSize.y + textPadding * 2;
	EntityHandle hierarchyLabelRect = sceneUI->createEntity("Hierarchy_Tab",
		RectSprite(Color(100, 100, 100, 255)),
		//RectButton(),
		//LayoutElement(),
		RectTransform(0, 0, labelRectWidth, labelRectHeight, 1.05f, Alignment::TOP_LEFT)
	);
	RectButton* heirarchyButton = hierarchyLabelRect.addComponent<RectButton>();
	heirarchyButton->colors[RectButton::ButtonState::DEFAULT] = Color(100, 100, 100);
	heirarchyButton->colors[RectButton::ButtonState::HOVER_OVER] = Color(120, 120, 120);
	heirarchyButton->colors[RectButton::ButtonState::PRESSED_DOWN] = Color(80, 80, 80);
	labelRectLE = hierarchyLabelRect.addComponent<LayoutElement>();
	labelRectLE->setMinSize(Vector2(labelRectWidth, labelRectHeight));
	labelRectLE->setMinSizeEnabled(true);
	labelRectLE->setFlexibleSize(Vector2(0.0f, 0.0f));
	labelRectLE->setFlexibleSizeEnabled(true);
	hierarchyLabelRect.setParent(tabContainer);
	// Hierarchy label
	EntityHandle hierarchyLabel = sceneUI->createEntity("Hierarchy_label",
		hierarchyLabelText,
		RectTransform(textPadding, textPadding, labelRectWidth, labelRectHeight, 1.1f, Alignment::TOP_LEFT)
	);
	hierarchyLabel.setParent(hierarchyLabelRect);

	// Inspector background
	EntityHandle content = sceneUI->createEntity("EditorPanel_Content",
		//Inspector(),
		//HierarchyView(),
		HorizontalLayoutGroup(),
		RectSprite(Color(175, 0, 0, 255)),
		RectTransform(0, 0, 0, 0, 1.05f, Alignment::TOP_LEFT)
	);
	HierarchyView* hierarchyView = content.addComponent<HierarchyView>();
	Inspector* inspectorView = content.addComponent<Inspector>();
	hierarchyView->disable();
	inspectorPanel = ComponentHandle(inspectorView);
	hierarchyPanel = ComponentHandle(hierarchyView);
	content.setParent(owner);
	inspectorButton->clickFunction = Core::bind(this, &EditorPanel::selectTab, Tab::Inspector); // Display Inspector View
	heirarchyButton->clickFunction = Core::bind(this, &EditorPanel::selectTab, Tab::Hierarchy); // Display Heirarchy View
}

void EditorPanel::selectTab(Tab tab) {
	Inspector* inspectorView = inspectorPanel.getComponent<Inspector>();
	HierarchyView* hierarchyView = hierarchyPanel.getComponent<HierarchyView>();

	ComponentHandle hierarchyPanel;
	if (tab == Tab::Inspector) {
		inspectorView->enable();
		hierarchyView->disable();
	}
	else if (tab == Tab::Hierarchy) {
		inspectorView->disable();
		hierarchyView->enable();
	}
	currentTab = tab;
}