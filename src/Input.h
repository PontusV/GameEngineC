#ifndef INPUT_MANAGER
#define INPUT_MANAGER
#include <map>
#include <string>
#include <vector>

union SDL_Event;
class Engine;
class KeyListener;

class Input {
public:
	Input(Engine* engine);
	~Input();
	void processInputEvent(const SDL_Event& event) const;
	//Observer
	void addKeyListener(KeyListener* listener);
	void addKeyBind(int keyCode, std::string buttonName);

	//Poll
	//const bool getButtonDown(std::string buttonName) const;

private:
	Engine* engine;
	std::vector<KeyListener*> keyListeners;
	std::map<int, std::string> keyBinds;

	void keyPressed(int) const;
	void keyReleased(int) const;

	const std::string getButtonName(int keyCode) const;
	//TODO: List of contexts, A context contains a list of keybinds.
	//Input types: Actions (happens once on either buttonDown or buttonUp), States, Ranges
	//std::map<int, std::string> keyBindsPressed;
	//std::map<int, std::string> keyBindsReleased;
};
#endif