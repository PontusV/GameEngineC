# CppMirror

C++ Reflection and Serialization of Reflected Types.

CppMirror adds Reflection Data and Functions to native C++ classes and structs. The ReflectionParser goes through the target file and all its included files and generates a new header for each class and struct that declare themselves as reflected. Classes and structs may declare themselves as reflected with the use of the macros defined in ReflectionMacros.h. This new header contains a definition of a GENERATED_BODY(...) macro which may be added in the class definition to add reflection.

Requires C++ 17 to compile.
Uses LibClang to parse source code.

ReflectionParser has to be built in Debug mode before building ReflectionExample.
ReflectionExample makes use of the ReflectionParser.exe that is built into the CppMirror/Debug folder

# How to add to your Visual Studio project
Build the ReflectionParser project to create a ReflectionParser.exe

In your project go to Properties->Build Events->Pre-Build Event

In Command Line add: location\of\ReflectionParser.exe location\of\target\header.h

For example: $(ProjectDir)exe\ReflectionParser.exe $(ProjectDir)main.cpp

You also need to include some headers. Get the include folder from CppMirror/ReflectionExample

# How to use

To see how to reflect your classes and structs go take a look at the ReflectionExample project source code (Header.h and Header2.h).
ReflectionExample.cpp also gives examples on how the reflected types may be used.

#include "include/ReflectionParser/ReflectionMacros.h" may be written in your main header/cpp file, before any other file that use these macros are included, in order to define the macros at the start of the program. I am not sure if there is any way to avoid writing the whole path to the macro header. You may move the ReflectionMacros.h file next to your main header/cpp file to shorten the include path.
