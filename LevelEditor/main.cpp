#include "LevelEditor.h"

#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	LevelEditor editor;
	int exit = editor.initiate();

	return exit;
}

int main(int argc, char *argv[]) {
	LevelEditor editor;
	int exit = editor.initiate();

	return exit;
}