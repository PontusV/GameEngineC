#ifndef CURSOR_TYPE_H
#define CURSOR_TYPE_H

#include <clang-c/Index.h>
#include <string>
#include <cstddef>

class Cursor;

/* A Handle for CXType. */
class CursorType {
public:
	CursorType(const CXType& handle);
	~CursorType();

	/* Returns true for both lvalue and rvalue reference */
	bool isReference() const;
	bool isPointer() const;
	bool isConst() const;
	bool isArray() const;
	bool isEnum() const;
	long long getArraySize() const;
	/* Returns the amount of pointers the Entity is using in the type. */
	std::size_t getPointerCount() const;

	/* Returns a cursor to the Declarations of the Type of the Entity that this cursor is pointing at. */
	Cursor getDeclaration() const;

	/* Returns the kind of the type. */
	CXTypeKind getKind() const;
	/* Returns the name of the Type with all modifiers, array, reference and pointer symbols. */
	std::string getSpelling() const;
	/* Returns the name of the Type with all modifies, reference and pointer symbols. Array symbols are removed. */
	std::string getFullName() const;
	/* Returns the name of the Type without modifiers, array, reference and pointer symbols. */
	std::string getName() const;

	int getArgumentCount() const;
	CursorType getArgumentType(unsigned index) const;

	/* Clang's type system explicitly models typedefs and all the ways a specific type can be represented.
	   The canonical type is the underlying type with all the "sugar" removed.
	   For example, if 'T' is a typedef for 'int', the canonical type for 'T' would be 'int'.
	*/
	CursorType getCanonicalType() const;

private:
	std::size_t getNameStartIndex() const;
	std::size_t getNameEndIndex() const;

private:
	CXType handle;
};
#endif