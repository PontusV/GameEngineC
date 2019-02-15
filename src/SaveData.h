#ifndef SAVE_DATA_H
#define SAVE_DATA_H

#include <iostream>

namespace GameEngine {
	class SaveData {
	public:
		~SaveData();
		//Save
		virtual void writeObject(std::ostream& os) const = 0;
	protected:
		SaveData();
	};
}
#endif