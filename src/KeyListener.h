#ifndef KEY_LISTENER
#define KEY_LISTENER

#include <string>

class KeyListener
{
public:
	KeyListener();
	~KeyListener();

	virtual void keyPressed(std::string buttonName) = 0;
	virtual void keyReleased(std::string buttonName) = 0;
	//void keyTyped(keyEvent e);
};
#endif