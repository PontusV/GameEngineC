#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include <iostream>
namespace Core {

	/* Interface for classes with the ability to be read from instream and written to outstream. */
	class Serializable {
	public:
		Serializable();
		~Serializable();
		virtual void serialize(std::ostream& os) const = 0;
		virtual void deserialize(std::istream& is) = 0;
	};
}
#endif