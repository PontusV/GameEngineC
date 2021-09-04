#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <vector>
#include <string>

typedef std::size_t EntityID;

namespace Editor {

	struct EntityHierarchyNode {
		EntityHierarchyNode() {}
		EntityHierarchyNode(EntityID entityID, std::string name, std::vector<EntityHierarchyNode> children) : entityID(entityID), name(name), children(children) {}
		EntityID entityID = 0;
		std::string name;
		std::vector<EntityHierarchyNode> children;
	};
	
	struct EntityHierarchyRootNode : EntityHierarchyNode {
		std::string filePath;
		std::vector<EntityHierarchyNode> children;
	};

	class LevelEditor;
	class GameView;
	class UndoRedoManager;

	class Hierarchy {
	public:
		Hierarchy(LevelEditor* editor, GameView* gameView, UndoRedoManager* undoRedoManager);
		~Hierarchy();

		/* Renders ImGui window */
		void tick(EntityID target);
		/* Updates scene and root orders */
		void update();
		/* Initializes for the currently loaded Engine DLL */
		void initiate();
		/* Removes all currently added scenes */
		void clear();
		bool isDirty() const;
		void setDirty(bool value);

		EntityID getActiveScene();
		void setActiveScene(EntityID rootEntityID);

		void onEntitiesChanged(EntityID entityID);

		std::size_t getSceneCount();
	private:
		LevelEditor* editor;
		GameView* gameView;
		UndoRedoManager* undoRedoManager;
		std::vector<EntityHierarchyRootNode> scenes;
		EntityID activeScene; // rootEntityID
		bool dirty = false;
	};
}

#endif