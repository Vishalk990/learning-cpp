/*
 * main.cpp — CONSUMER
 * -------------------
 *
 * This file USES `sum` without knowing or caring how it is
 * implemented. All it needs is the DECLARATION (the signature), and
 * that comes from `#include "math.h"`. The actual machine code for
 * `sum` lives in math.cpp's object file and is stitched in by the
 * linker.
 *
 * What the toolchain does for THIS file:
 *
 *   1. PREPROCESSOR.  Textually pastes the contents of math.h into
 *      this file, giving main.cpp visibility of `int sum(int, int);`.
 *      (It also pastes <iostream> for `std::cout`.)
 *
 *   2. COMPILER.  Parses and type-checks. The call `sum(3, 1)` is
 *      checked against the declaration in math.h. The body of sum
 *      is IRRELEVANT at this stage — the compiler emits a placeholder
 *      "external symbol named sum" and moves on. Output: main.o.
 *
 *   3. LINKER.  Given main.o and math.o, resolves that placeholder
 *      against the actual definition compiled from math.cpp. Output:
 *      the final executable.
 *
 * Why this matters in practice:
 *   - You can edit, change, and re-compile math.cpp without touching
 *     main.cpp — as long as the header signatures stay stable.
 *   - In a project with hundreds of .cpp files, only the .cpp files
 *     that actually changed need recompilation. The header decouples
 *     interface from implementation; this is why C++ uses separate
 *     compilation.
 *
 * Note: this file does NOT `#include "math.cpp"`. You include
 * headers, not implementations — including a .cpp would paste its
 * function definitions into this translation unit too, and the
 * linker would then see two definitions of `sum` and complain.
 */

#include "math.h"
#include <iostream>

using namespace std;

int main() {
    // The compiler resolves `sum` against the declaration in math.h
    // (it does not need the body to type-check this line). The
    // linker resolves it against the definition in math.cpp's
    // object file.
    cout << sum(3, 1) << endl;     // prints 4
    return 0;
}