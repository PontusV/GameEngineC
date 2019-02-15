#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include <iostream>
namespace GameEngine {

	/* Interface for classes which need to be saved to an outstream. */
	class Serializable {
	public:
		Serializable();
		~Serializable();
		virtual void serialize(std::ostream &os) const = 0;
		virtual void deserialize(std::istream &is) = 0;
		//virtual void read(std::istream &is) = 0; //Objects need different arguments for loading so this virtual function cannot exist
	//private:
		//static const long serialVersionUID = 0; //Determines if a de-serialized file is compatible with this class
	};
}
#endif