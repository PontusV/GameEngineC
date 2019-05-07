#ifndef COLOR_H
#define COLOR_H
#include "Serializable.h"
#include <iostream>
namespace Core {
	/* Holds color values between 0-255 */
	class Color : public Serializable {
	public:
		Color() {}
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}

		void serialize(std::ostream& os) const override;
		void deserialize(std::istream& is) override;

		unsigned char r, g, b, a;
	};
}
#endif