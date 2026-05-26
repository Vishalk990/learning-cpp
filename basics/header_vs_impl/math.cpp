/*
 * math.cpp — IMPLEMENTATION
 * -------------------------
 *
 * This .cpp provides the BODY (definition) of the function declared
 * in math.h. It is compiled independently into an object file
 * (math.o), and the linker combines that object file with main.o to
 * produce the final executable.
 *
 * Why include "math.h" here even though this is the file defining
 * `sum`?
 *
 *   1. CONSISTENCY CHECK. If the declaration in math.h ever drifts
 *      from the definition here (different return type, different
 *      parameters, missing `const`), the compiler catches the
 *      mismatch because both forms are now visible in the same
 *      translation unit. Without this include, the divergence would
 *      only surface much later at the link step.
 *
 *   2. Any other declarations or types math.cpp needs from this
 *      module are imported in one go.
 *
 * Building this project by hand:
 *
 *     g++ -c math.cpp -o math.o      # compile to object file
 *     g++ -c main.cpp -o main.o      # compile to object file
 *     g++ math.o main.o -o app       # link both into one executable
 *
 * Or in one shot (g++ runs all stages):
 *
 *     g++ math.cpp main.cpp -o app
 *
 * Either way, each .cpp is compiled INDEPENDENTLY — the compiler
 * sees one translation unit at a time. That is why math.cpp does not
 * need to know about main.cpp, and main.cpp does not need source
 * access to math.cpp; the header is the only thing they share. This
 * is called "separate compilation" and is the reason large C++
 * projects don't have to rebuild everything when a single .cpp file
 * changes.
 */

#include "math.h"

// DEFINITION — the body the linker will match against any call to
// `sum` from any other translation unit.
int sum(int a, int b) {
    return a + b;
}