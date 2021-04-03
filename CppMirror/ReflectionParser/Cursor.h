#ifndef CURSOR_H
#define CURSOR_H

#include <clang-c/Index.h>
#include <string>
#include <vector>
#include "CursorType.h"
#include <ReflectionParser/ReflectionTypes.h>

/* A Handle for CXCursor. */
class Cursor {
public:
	Cursor(const CXCursor& handle);
	~Cursor();

	CXCursorKind getKind() const;

	/* The lexical parent of a cursor is the cursor in which the given cursor was actually written. */
	Cursor getLexicalParent() const;
	Cursor getTemplateSpecialization() const;

	/* Returns the name of the Entity pointed by this cursor. */
	std::string getSpelling() const;
	std::string getDisplayName() const;
	std::string getMangledName() const;
	std::string getUSR() const;

	std::string getSourceFile() const;

	/* Returns true if the cursor is pointing on a definition. */
	bool isDefinition() const;
	bool isConstMethod() const;
	bool isStaticMethod() const;
	bool isVirtualMethod() const;
	bool isPureVirtualMethod() const;
	bool isVariadicMethod() const;

	CursorType getType() const;
	CursorType getReturnType() const;
	CursorType getTypedefType() const;

	int getArgumentCount() const;
	Cursor getArgument(unsigned index) const;
	std::vector<Cursor> getArguments() const;

	CX_StorageClass getStorageClass() const;
	CX_CXXAccessSpecifier getAccessSpecifier() const;

	std::vector<Cursor> getChildren() const;
	/* Returns a vector of Annotate Attributes for the Entity that this cursor is pointing towards. */
	std::vector<Mirror::Annotation> getAnnotateAttributes() const;
	/* Returns a vector of VariableType for the Entity that this cursor is pointing towards. */
	std::vector<Mirror::VariableType> getTemplateParameterTypes() const;

private:
	CXCursor handle;
};
#endif