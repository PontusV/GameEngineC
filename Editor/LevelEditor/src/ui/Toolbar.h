#ifndef TOOLBAR_H
#define TOOLBAR_H
namespace Editor {
	class LevelEditor;

	class Toolbar {
	public:
		Toolbar();
		~Toolbar();

		void tick(LevelEditor* editor);
	private:
	};
}
#endif