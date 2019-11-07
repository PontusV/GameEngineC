//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "LevelEditor.h"

int main(int argc, char *argv[]) {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	LevelEditor editor;
	int exit = editor.initiate();

	//int* test = new int;
	//*test = 10;
	//_CrtDumpMemoryLeaks();
	return exit;
}