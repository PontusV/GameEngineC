#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <vector>
#include <string>

typedef std::size_t EntityID;

namespace Editor {

	struct EntityData {
		EntityID id;
		std::string name;
	};

	struct EntityHierarchy {
		EntityHierarchy() : entity(EntityData{ 0, std::string() }), children({}) {}
		EntityHierarchy(EntityData entity, std::vector<EntityHierarchy> children) : entity(entity), children(children) {}
		EntityData entity;
		std::vector<EntityHierarchy> children;
	};
	
	struct SceneData {
		std::string name;
		std::vector<EntityHierarchy> roots;
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

		std::size_t getActiveSceneIndex();
		void setActiveSceneIndex(std::size_t sceneIndex);

		void onSceneAdded(std::size_t sceneIndex);
		void onSceneRemoved(std::size_t sceneIndex);
		void onSceneChanged(std::size_t sceneIndex);
		void onEntityChanged(EntityID entityID);

		std::size_t getSceneCount();
		std::string getSceneName(std::size_t sceneIndex);
	private:
		std::size_t getRootIndex(std::size_t sceneIndex, EntityID entityID);
		bool setRootIndex(std::size_t sceneIndex, EntityID entityID, std::size_t index);
		std::size_t getSceneIndexByName(std::string name) const;
	private:
		LevelEditor* editor;
		GameView* gameView;
		UndoRedoManager* undoRedoManager;
		std::vector<SceneData> sceneOrder;
		std::size_t activeSceneIndex;
	};
}

#endif