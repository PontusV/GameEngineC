#ifndef POPUP_MANAGER_H
#define POPUP_MANAGER_H
#include <cstddef>
namespace Editor {
	class LevelEditor;

	class PopupManager {
	public:
		PopupManager();
		~PopupManager();

		void tick(LevelEditor* editor);

		void openCreateScene();
		void openCreateProject();
		void openCreateEntity(std::size_t rootEntityID = 0);
		void openDeleteEntity(std::size_t entityID);
		void openSaveChanges();
	private:
		bool open_create_scene_popup,
			open_create_project_popup,
			open_create_entity_popup,
			open_delete_entity_popup,
			open_save_changes_popup;
		std::size_t createEntityParentID;
		std::size_t deleteEntityID;
	};
}
#endif