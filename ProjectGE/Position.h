#ifndef POSITION_H
#define POSITION_H
#include "Serializable.h"

namespace GameEngine {
	class Position : public Serializable {
	public:
		Position(float x, float y);
		Position(std::istream& is);
		Position();
		~Position();

		//Save & Load
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

		//Functions for edit or retrieval of values
		float getX() const;
		float getY() const;
		void setX(float x);
		void setY(float y);

	private:
		float x;
		float y;
	};
}
#endif