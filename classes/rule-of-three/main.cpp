/*
 * main.cpp — the Rule of Three in action.
 *
 * Demonstrates that the class's copies are DEEP: after mutating a1,
 * a2 is left untouched (independent buffers), and at the end both
 * objects clean up their own allocation with no double-free.
 *
 * It includes only the interface (array.hpp); the linker connects
 * these calls to the bodies compiled from array.cpp.
 *
 * Expected output:
 *     [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
 *     Copy constructor called
 *     [20, 1, 4, 9, 16, 25, 36, 49, 64, 81]
 *     [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
 */

#include "array.hpp"

int main() {
    Array a1;
    a1.PrintArray(); // [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]

    // SUBTLE BUT IMPORTANT: despite the `=`, this is NOT assignment.
    // `Array a2 = a1;` constructs a brand-new object, so it calls the
    // COPY CONSTRUCTOR (prints "Copy constructor called"), not the
    // copy assignment operator. The `=` here is initialization syntax.
    Array a2 = a1;

    // This commented line WOULD call the copy ASSIGNMENT operator,
    // because a2 already exists at this point:
    // a2 = a1;             // → "Copy assignment operator"

    // Mutate a1's buffer. Thanks to the deep copy, a2 is unaffected —
    // they own separate buffers. (With a shallow copy, this would
    // also change a2.)
    a1.Set(0, 10);
    a1.Set(0, 20);

    a1.PrintArray(); // [20, 1, 4, 9, ...]  — a1 changed
    a2.PrintArray(); // [0, 1, 4, 9, ...]   — a2 untouched: proof
                     //                       the deep copy worked
    return 0;
    // Both destructors run here, each delete[]-ing its own buffer.
    // No double-free precisely because the copies were deep.
}