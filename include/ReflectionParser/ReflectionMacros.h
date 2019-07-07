#ifndef REFLECTION_MACROS_H
#define REFLECTION_MACROS_H

#ifdef REFLECTION_PARSER
#define CLASS(...) class __attribute__((annotate("Reflect,"#__VA_ARGS__)))
#define STRUCT(...) struct __attribute__((annotate("Reflect,"#__VA_ARGS__)))
#define PROPERTY(...) __attribute__((annotate("Reflect,"#__VA_ARGS__)))
#define FUNCTION(...) __attribute__((annotate("Reflect,"#__VA_ARGS__)))
#define GENERATED_BODY(...)
#else
#define CLASS(...) class
#define STRUCT(...) struct
#define PROPERTY(...)
#define FUNCTION(...)
#endif

// Annotations
#define Update
#define Category

#endif