#include "Script.h"

using namespace Core;


Input* Script::input;
Window* Script::window;

Script::Script() : started(false), enabled(true) {
}

Script::~Script() {
}

void Script::enable() {
	enabled = true;
}

void Script::disable() {
	started = false;
	enabled = false;
}