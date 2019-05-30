#include "Color.h"
using namespace Core;

void Color::serialize(std::ostream& os) const {
	os.write((char*)&r, sizeof(r));		// Color r
	os.write((char*)&g, sizeof(g));		// Color g
	os.write((char*)&b, sizeof(b));		// Color b
	os.write((char*)&a, sizeof(a));		// Color a
}

void Color::deserialize(std::istream& is) {
	is.read((char*)&r, sizeof(r));		// Color r
	is.read((char*)&g, sizeof(g));		// Color g
	is.read((char*)&b, sizeof(b));		// Color b
	is.read((char*)&a, sizeof(a));		// Color a
}