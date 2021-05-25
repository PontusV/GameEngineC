#ifndef MENUBAR_H
#define MENUBAR_H
namespace Editor {
	class LevelEditor;

	class Menubar {
	public:
		Menubar();
		~Menubar();

		void tick(LevelEditor* editor, bool editMode);
	private:
	};
}
#endif